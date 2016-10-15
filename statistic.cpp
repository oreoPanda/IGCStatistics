#include "statistic.h"

/*TODO rethink time, the log time could be used on X axis, not the time since first logged point*/

Statistic::Statistic(QObject *parent)
    : QObject(parent),
      altitudedeltaseries(nullptr),
      distancedeltaseries(nullptr)
{

}

/*slot to set gps data from an igc file*/
void Statistic::set_GPSData(const QList<Fix> & fixlist)
{
    fixrecords = fixlist;
}

/*public slots that are to be used with a UI*/
void Statistic::altitudeStats()
{
    emit data(calculate_altitudeData());
}

void Statistic::altitudeDeltaStats()
{
    if(!altitudedeltaseries){
        calculate_altitudeData();
    }
    emit data(altitudedeltaseries);
}

void Statistic::distanceStats()
{
    emit data(calculate_distanceData());
}

void Statistic::distanceDeltaStats()
{
    if(!distancedeltaseries){
        calculate_distanceData();
    }
    emit data(distancedeltaseries);
}

void Statistic::flightStats()
{
    emit data(calculate_flightData());
}

void Statistic::intervalStats() const
{
    emit data(calculate_intervalData());
}

void Statistic::speedStats()
{
    emit data(calculate_speedData());
}

/*private calculation functions that return a line series. Control over its deletion is handed over to the signal receiver*/
/*These series are always measurements between two points, so they contain one point less than the original data from file
    file points 0 to 1 are in series point 0
    file points 1 to 2 are in series point 1
    ...
    file points 2999 to 3000 are in series point 2999*/
QLineSeries * Statistic::calculate_altitudeData()//TODO think of choosing an altitude type TODO what if fixrecords is still empty?
{
    static QLineSeries *altitudeseries = new QLineSeries();
    if(altitudeseries->count() > 0){
        return altitudeseries;
    }
    this->altitudedeltaseries = new QLineSeries();

    ushort time = 0;

    for(int i = STEPSIZE; i < fixrecords.size(); i+=STEPSIZE)
    {
        time += fixrecords.at(i).time - fixrecords.at(i-STEPSIZE).time;
        *altitudeseries << QPointF(time, fixrecords.at(i).altitude.GNSSAlt);
        *altitudedeltaseries << QPointF(time, fixrecords.at(i).altitude.GNSSAlt - fixrecords.at(i-STEPSIZE).altitude.GNSSAlt);
    }

    return altitudeseries;
}

/*TODO CHECK: distancedeltaseries should contain point 0, value at index 0 and point 1, value at index 1*/
/*TODO CHECK: all series should be shorter than the fixrecord by one*/
QLineSeries * Statistic::calculate_distanceData()
{
    static QLineSeries * distanceseries = new QLineSeries();
    if(distanceseries->count() > 0){
        return distanceseries;
    }
    this->distancedeltaseries = new QLineSeries();

    ushort time = 0;
    double distance = 0;
    double delta = 0;

    for(int i = STEPSIZE; i < fixrecords.size(); i++)
    {
        time += (fixrecords.at(i).time - fixrecords.at(i-STEPSIZE).time);
        delta = fixrecords.at(i).position - fixrecords.at(i-STEPSIZE).position;
        distance += delta;
        *distanceseries << QPointF(time, distance);    //same as distanceseries->append(time, distance)
        *distancedeltaseries << QPointF(time, delta);
    }

    return distanceseries;
}

QLineSeries * Statistic::calculate_flightData()
{
    QLineSeries *flight = new QLineSeries();

    for(int i = 0; i < fixrecords.size(); i++)
    {
        *flight << QPointF(fixrecords.at(i).position.longitude, fixrecords.at(i).position.latitude);
    }

    return flight;
}

/*TODO CHECK: Here again: I is one ahead of the index of the two delta series*/
QLineSeries * Statistic::calculate_speedData()
{
    QLineSeries *speedseries = new QLineSeries();

    ushort time = 0;

    if(!this->altitudedeltaseries){
        calculate_altitudeData();
    }
    if(!this->distancedeltaseries){
        calculate_distanceData();
    }
    for(int i = STEPSIZE; i < fixrecords.size(); i++)
    {
        ushort t = fixrecords.at(i).time - fixrecords.at(i-1).time;
        short a = this->altitudedeltaseries->at(i-STEPSIZE).y();
        double d = this->distancedeltaseries->at(i-STEPSIZE).y() * 1000;
        /*Take the altitude change into account for a more exact speed calculation: Find the length of the hypotenuse*/
        double hypotenuse = sqrt(pow(a, 2) + pow(d, 2));
        /*Calculate the speed across the hypotenuse in km/h*/
        double speed = hypotenuse / t * 3.6;
        if(speed < 70 || speed > 130)
        {

        }

        time += t;
        *speedseries << QPointF(time, speed);
    }

    return speedseries;
}

QLineSeries * Statistic::calculate_intervalData() const
{
    QLineSeries * const intervalseries = new QLineSeries();

    for(int i = STEPSIZE; i < fixrecords.size(); i++)
    {
        ushort seconds = fixrecords.at(i).time - fixrecords.at(i-STEPSIZE).time;
        *intervalseries << QPoint(i, seconds);
    }

    return intervalseries;
}
