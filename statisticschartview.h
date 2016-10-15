#ifndef STATISTICSCHARTVIEW_H
#define STATISTICSCHARTVIEW_H

#include <QtCharts>
#include <QChartView>

using namespace QtCharts;

class StatisticsChartView : public QChartView
{
public:
    StatisticsChartView(QWidget *parent = Q_NULLPTR);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

#endif // STATISTICSCHARTVIEW_H
