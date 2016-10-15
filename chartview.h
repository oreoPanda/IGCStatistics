#ifndef STATISTICSCHARTVIEW_H
#define STATISTICSCHARTVIEW_H

#include <QtCharts>
#include <QChartView>

using namespace QtCharts;

class ChartView : public QChartView
{
public:
    ChartView(QWidget *parent = Q_NULLPTR);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

#endif // STATISTICSCHARTVIEW_H
