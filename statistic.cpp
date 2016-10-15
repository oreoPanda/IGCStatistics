#include "statistic.h"

/*TODO rethink time, the log time could be used on X axis, not the time since first logged point*/

Statistic::Statistic(QObject *parent)
    : QObject(parent)  //TODO check calls constructor of parent?
{

}

Statistic::~Statistic()
{
    //TODO check destructor of QObject is called automatically?
}

/*slot to set gps data from an igc file*/
void Statistic::set_GPSData(const QList<Fix> & fixlist)
{
    fixrecords = fixlist;
}

/*slot to be used with UI*/
void Statistic::getData(DataType t)
{
    switch(t)
    {
    case ALTITUDE:  emit data(calculate_altitudeData());break;
    case DISTANCE:  emit data(calculate_distanceData());break;
    case FLIGHT:    emit data(calculate_flightData());break;
    case INTERVAL:  emit data(calculate_intervalData());break;
    case SPEED:     emit data(calculate_speedData());break;

    case ALTITUDEDELTA:{
        if(this->altitudedelta.empty() ){
            calculate_altitudeData();
        }
        emit data(altitudedelta);
        break;
    }
    case DISTANCEDELTA:{
        if(this->distancedelta.empty() ){
            calculate_distanceData();
        }
        emit data(distancedelta);
        break;
    }
    }
}

/*private calculation functions that return a line series. Control over its deletion is handed over to the signal receiver*/
/*These series are always measurements between two points, so they contain one point less than the original data from file
    file points 0 to 1 are in series point 0
    file points 1 to 2 are in series point 1
    ...
    file points 2999 to 3000 are in series point 2999*/
const QList<QPointF> & Statistic::calculate_altitudeData()//TODO think of choosing an altitude type TODO what if fixrecords is still empty?
{
    if( !this->altitude.empty() ){
        return this->altitude;
    }

    ushort time = 0;

    for(int i = STEPSIZE; i < fixrecords.size(); i+=STEPSIZE)
    {
        time += fixrecords.at(i).time - fixrecords.at(i-STEPSIZE).time;
        this->altitude << QPointF(time, fixrecords.at(i).altitude.pressureAlt);
        this->altitudedelta << QPointF(time, fixrecords.at(i).altitude.pressureAlt - fixrecords.at(i-STEPSIZE).altitude.pressureAlt);
    }

    return altitude;
}

/*TODO CHECK: distancedeltaseries should contain point 0, value at index 0 and point 1, value at index 1*/
/*TODO CHECK: all series should be shorter than the fixrecord by one*/
const QList<QPointF> & Statistic::calculate_distanceData()
{
    if( !distance.empty() ){
        return this->distance;
    }

    ushort time = 0;
    double distance = 0;
    double delta = 0;

    for(int i = STEPSIZE; i < fixrecords.size(); i++)
    {
        time += (fixrecords.at(i).time - fixrecords.at(i-STEPSIZE).time);
        delta = fixrecords.at(i).position - fixrecords.at(i-STEPSIZE).position;
        distance += delta;
        this->distance << QPointF(time, distance);
        this->distancedelta << QPointF(time, delta);
    }

    /*return pointer to new object so that the static pointer won't point to invalid memory some time*/
    return this->distance;
}

const QList<QPointF> & Statistic::calculate_flightData()
{
    for(int i = 0; i < fixrecords.size(); i++)
    {
        this->flight << QPointF(fixrecords.at(i).position.longitude, fixrecords.at(i).position.latitude);
    }

    return this->flight;
}

/*The interval is given with index of B record on X axis, not time like all the others*/
const QList<QPointF> & Statistic::calculate_intervalData()
{
    if( !this->interval.empty() ){
        return this->interval;
    }
    for(int i = STEPSIZE; i < fixrecords.size(); i++)
    {
        ushort seconds = fixrecords.at(i).time - fixrecords.at(i-STEPSIZE).time;
        this->interval << QPoint(i, seconds);
    }

    return this->interval;
}

/*TODO CHECK: Here again: I is one ahead of the index of the two delta series*/
const QList<QPointF> & Statistic::calculate_speedData()
{
    if( !this->speed.empty() ){
        return this->speed;
    }

    ushort time = 0;

    if(this->altitudedelta.empty() ){
        calculate_altitudeData();
    }
    if(this->distancedelta.empty() ){
        calculate_distanceData();
    }
    for(int i = STEPSIZE; i < fixrecords.size(); i++)
    {
        ushort t = fixrecords.at(i).time - fixrecords.at(i-1).time;
        short a = this->altitudedelta.at(i-STEPSIZE).y();
        double d = this->distancedelta.at(i-STEPSIZE).y() * 1000;
        /*Take the altitude change into account for a more exact speed calculation: Find the length of the hypotenuse*/
        double hypotenuse = sqrt(pow(a, 2) + pow(d, 2));
        /*Calculate the speed across the hypotenuse in km/h*/
        double speed = hypotenuse / t * 3.6;

        time += t;
        this->speed << QPointF(time, speed);
    }

    return this->speed;
}
