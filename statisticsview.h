#ifndef STATISTICSVIEW_H
#define STATISTICSVIEW_H

#include "data.h"
#include <QMainWindow>
#include <QtCharts>

using namespace QtCharts;

namespace Ui {
class StatisticsView;
}

class StatisticsView : public QMainWindow
{
    Q_OBJECT

public:
    explicit StatisticsView(QWidget *parent = 0);
    ~StatisticsView();

public slots:
    void compareModeToggled(bool checked);
    void change_shown_Adata(const QString & dataname);
    void change_shown_Bdata(const QString & dataname);
    void plotData(QLineSeries * const data);

signals:
    void requestData(DataType);

private:
    Ui::StatisticsView *ui;

    /*A list with the names of data*/
    QStringList datanames;

    /*the two Y-axes*/
    QAbstractAxis *Yleft;
    QAbstractAxis *Yright;

    /*Pointers to the active date series*/
    QLineSeries *activedata;
    QLineSeries *comparedata;

    /*private functions for displaying series*/
    void change_shown_data(const QString & dataname);
    void deactivate(QLineSeries * & active);
};

#endif // STATISTICSVIEW_H
