#ifndef STATISTICSVIEW_H
#define STATISTICSVIEW_H

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
    void requestAltitudeA();
    void requestAltitudeDeltaA();
    void requestDistanceA();
    void requestDistanceDeltaA();
    void requestFlightA();
    void requestSpeedA();
    void requestAltitudeB();
    void requestDistanceB();
    void requestIntervalB();
    void requestSpeedB();
    void plotData(QLineSeries * const data);
    void seriesDestroyed(QObject * obj);

signals:
    void requestAltitudeData();
    void requestAltitudeDelta();
    void requestDistanceData();
    void requestDistanceDelta();
    void requestIntervalData();
    void requestFlightData();
    void requestSpeedData();

private:
    Ui::StatisticsView *ui;
    /*The chart, which owns all Line Series*/
    QChart *chart;
    /*Pointers to the LineSeries of chart, kept for switching mechanism (A set)*/
    QLineSeries *activeA;
    QLineSeries *altitudesA;
    QLineSeries *altitudedeltaA;
    QLineSeries *distancesA;
    QLineSeries *distancedeltaA;
    QLineSeries *flightA;
    QLineSeries *speedsA;
    /*These ones are for the second data sets in compare mode (B set)*/
    QLineSeries *activeB;
    QLineSeries *altitudesB;
    QLineSeries *distancesB;
    QLineSeries *intervalB;
    QLineSeries *speedsB;
    void deactivate(QLineSeries * & active);
};

#endif // STATISTICSVIEW_H
