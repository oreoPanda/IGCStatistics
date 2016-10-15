#include "chartview.h"

ChartView::ChartView(QWidget *parent)
    :QChartView(parent)
{

}

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    static QPoint lastPos = event->pos();

    if(event->buttons() == Qt::LeftButton){
        chart()->scroll(lastPos.rx() - event->pos().rx(), event->pos().ry() - lastPos.ry());
        lastPos = event->pos();
    }else{
        lastPos = event->pos();
    }
    if(chart()->axisX()){
        //chart()->axisX()->setMin(0);
    }
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().ry() > 0){
        chart()->zoom(1.2);
    }else{
        chart()->zoom(0.8);
    }
    if(chart()->axisX()){
        //chart()->axisX()->setMin(0);
    }
}
