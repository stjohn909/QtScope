#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <scopedrawarea.h>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setupRealtimeDataDemo(QCustomPlot* customPlot);

private slots:
    void on_drawSineWave_clicked();
    void on_turn_bg_purple_clicked();
    void realtimeDataSlot();


private:
    Ui::MainWindow *ui;
    QString demoName;
    QTimer dataTimer;


protected:
    void set_background();
    void set_foreground();

signals:

};
#endif // MAINWINDOW_H
