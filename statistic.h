#ifndef STATISTIC_H
#define STATISTIC_H

#include "igctypes.h"

#include <QObject>
#include <QList>
#include <QtCharts>

#define STEPSIZE 1

//NOTE: QtCharts is not licensed under LGPLv3 (but GPLv3)
//NOTE: Qt Data Visualization can visualize data in 3D!

using namespace QtCharts;

class Statistic : public QObject
{
    Q_OBJECT
public:
    explicit Statistic(QObject *parent = 0);

signals:
    void data(QLineSeries * const s) const;

public slots:
    void set_GPSData(const QList<Fix> & fixlist);
    void altitudeStats();
    void altitudeDeltaStats();
    void distanceStats();
    void distanceDeltaStats();
    void flightStats();
    void intervalStats() const;
    void speedStats();

private:
    QStringList lines;
    QList<Fix> fixrecords;

    /*Series stored in this class for faster speed calculation*/
    QLineSeries *altitudedeltaseries;
    QLineSeries *distancedeltaseries;

    /*Calculation functions*/
    QLineSeries *calculate_altitudeData();
    QLineSeries *calculate_distanceData();
    QLineSeries *calculate_flightData();
    QLineSeries *calculate_intervalData() const;
    QLineSeries *calculate_speedData();
};

#endif // STATISTIC_H
