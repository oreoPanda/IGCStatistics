#ifndef STATISTIC_H
#define STATISTIC_H

#include "igctypes.h"
#include "data.h"
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
    ~Statistic();

signals:
    void data(const QList<QPointF> & data) const;

public slots:
    void set_GPSData(const QList<Fix> & fixlist);
    void getData(DataType t);

private:
    QStringList lines;
    QList<Fix> fixrecords;

    /*All implemented data series*/
    QList<QPointF> altitude;
    QList<QPointF> altitudedelta;
    QList<QPointF> distance;
    QList<QPointF> distancedelta;
    QList<QPointF> flight;
    QList<QPointF> interval;
    QList<QPointF> speed;

    /*Calculation functions*/
    const QList<QPointF> & calculate_altitudeData();
    const QList<QPointF> & calculate_distanceData();
    const QList<QPointF> & calculate_flightData();
    const QList<QPointF> & calculate_intervalData();
    const QList<QPointF> & calculate_speedData();
};

#endif // STATISTIC_H
