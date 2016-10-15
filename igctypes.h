#ifndef IGCTYPES_H
#define IGCTYPES_H

#include <cmath>
#include <qglobal.h>

#define IGC_LINE_LEN 76
#define EARTH_RADIUS 6371   //earth's radius in kilometers

//#define BREAKPOINT(expr) expr ? print something, breakpoint here : go on

struct Altitude{
    long pressureAlt;
    long GNSSAlt;
};

/*TODO make this more developer-friendly: a += operator so you can add degrees, minutes and seconds or an add function with three args*/
struct Coordinate{
    double latitude;
    double longitude;
    double latitudeRad() const
    {
        return this->latitude * (M_PI/180);
    }

    double longitudeRad() const
    {
        return this->longitude * (M_PI/180);
    }

    /*calculates the distance to coordinate subtrahend in kilometers*/
    double operator-(const Coordinate subtrahend) const
    {
        double deltalat = fabs(this->latitudeRad() - subtrahend.latitudeRad()); //absolute latitude delta
        double deltalong = fabs(this->longitudeRad() - subtrahend.longitudeRad());  //absolute longitude delta
        double sin1 = sin(static_cast<double>(deltalat/2));
        double sin2 = sin(static_cast<double>(deltalong/2));
        double sq1 = pow(sin1, 2);       //first squared expression in haversine formula
        double sq2 = pow(sin2, 2);      //second squared expression in haversine formula

        /*Haversine formula for the angle between two points on earth in radians*/
        double angle = 2*asin( sqrt( sq1 + cos(this->latitudeRad()) * cos(subtrahend.latitudeRad()) * sq2 ) );

        /*calculate an actual distance from that angle*/
        return angle * EARTH_RADIUS;
    }
};

struct Timestamp{
    ushort hours;
    ushort minutes;
    ushort seconds;
    /*Calculates the elapsed time between this timestamp and another in seconds*/
    ushort operator-(const Timestamp subtrahend) const
    {
        return (this->hours - subtrahend.hours)*60*60
             + (this->minutes - subtrahend.minutes)*60
             + (this->seconds - subtrahend.seconds);
    }
};

struct Fix{
    Timestamp time;
    Coordinate position;
    Altitude altitude;
};

#endif // IGCTYPES_H
