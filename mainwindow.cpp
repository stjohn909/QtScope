#include "mainwindow.h"
#include "ui_mainwindow.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#  include <QDesktopWidget>
#endif
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <array>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupRealtimeDataDemo(ui->scopeDrawArea);
    ui->scopeDrawArea->yAxis->setRange(-5, 5);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_turn_bg_purple_clicked()
{
    return;
}

void MainWindow::on_drawSineWave_clicked()
{
    //this->set_foreground();
    //this->update();
    return;
}

void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
  demoName = "VM205 Oscilloscope";

  customPlot->setNotAntialiasedElements(QCP::aeAll);
  customPlot->setBackground(QBrush(Qt::black, Qt::SolidPattern));
  //customPlot->setAutoFillBackground(true);
  QFont font;
  QColor gridColor = QColor(Qt::gray);  // White
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->grid()->setPen(QPen(gridColor));
  customPlot->yAxis->grid()->setPen(QPen(gridColor));


  customPlot->xAxis->setBasePen(QPen(Qt::white));
  customPlot->yAxis->setBasePen(QPen(Qt::white));
  customPlot->xAxis->setTickPen(QPen(Qt::white));
  customPlot->yAxis->setTickPen(QPen(Qt::white));
  customPlot->xAxis->setSubTickPen(QPen(Qt::white));
  customPlot->yAxis->setSubTickPen(QPen(Qt::white));

  customPlot->xAxis->setTickLabelColor(gridColor);
  customPlot->yAxis->setTickLabelColor(gridColor);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);

  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
  customPlot->addGraph(); // yellow line
  customPlot->graph(1)->setPen(QPen(QColor(200, 200, 0)));

  QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
  timeTicker->setTimeFormat("%s");
  customPlot->xAxis->setTicker(timeTicker);
  customPlot->xAxis->setRange(0, 800);
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->yAxis->setRange(0, 5);  // 800 samples, 5 volts max.

  // make left and bottom axes transfer their ranges to right and top axes.
  // this needs updating when vDiv or timeDiv changes.
  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  // In VM205, emit a refresh signal when data transfer is complete.
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

// graph update over time
void MainWindow::realtimeDataSlot()
{
    static QTime timeStart = QTime::currentTime();
    // Sine wave parameters
    double inputFreq = 1;  // fake input signal Hz
    inputFreq *= (2 * 3.14159);  // compute cycle for plot verification
    float timescale = 5;  // The VM205 has 7 time scales for X axis
    timescale = 800 / timescale;  // map time scale to samples
    inputFreq /= timescale;
    double peakVoltage = 4.0; // fake voltage amount as sine wave amplitude

    // Create some dummy data to plot.  The VM205 sends an 800-byte
    // uint_8 array from the hardware,
    std::array<double, 801> data0; //reflects scope sample rate.
    for (int i = 0; i < 801; i++)
    {
        // Time-based offset to see the sine wave changing.
        double offset0 = timeStart.msecsTo(QTime::currentTime()) / 20.0;
        // Since
        double sample = qSin(((double(i) +offset0) * inputFreq)) * peakVoltage;
        data0[i] = sample;
    }

    // Clear the graph data;
    ui->scopeDrawArea->graph(0)->data()->clear();

    // For now, this reads the dummy data.  In the scope app, it
    // should read a VM205 data packet.
    for (uint k = 0; k < 800; k++ )
    {
        // Simulate plotting the values of an 800-byte data array.
        // at a v/Div scale of 5 (set in the axis range).
        double key = double(k);
        ui->scopeDrawArea->graph(0)->addData(key, data0[k]);
    }

    ui->scopeDrawArea->replot();

}


