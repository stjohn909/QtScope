#ifndef SCOPEDRAWAREA_H
#define SCOPEDRAWAREA_H

#include <QWidget>
#include <QPainterPath>
#include <QTimer>
#include "/home/stjohn/qcustomplot/qcustomplot.h"

class ScopeDrawArea : public QWidget
{
    Q_OBJECT
public:
    explicit ScopeDrawArea(QWidget *parent = nullptr);
    void build_trace();
    void build_another_trace();
    void RealtimePlot(QCustomPlot *customPlot);

public slots:
    void realtimeDataSlot();

protected:
    QTimer dataTimer;
    QCustomPlot* plot;
    QString demoName;



signals:

};

#endif // SCOPEDRAWAREA_H
