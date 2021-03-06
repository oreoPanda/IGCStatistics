#include "igctypes.h"
#include "igcloader.h"
#include "statisticsview.h"
#include "statistic.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IGCLoader parser;
    StatisticsView w;
    Statistic stat;

    QObject::connect(&parser, SIGNAL(parsed(const QList<Fix> &)), &stat, SLOT(set_GPSData(const QList<Fix> &)) );

    parser.load("JonasEDAN400k.igc");
    parser.parse();

    //TODO check out commands strace and ptrace in bash

    QObject::connect(&w, SIGNAL(requestAltitudeData()), &stat, SLOT(altitudeStats()) );
    QObject::connect(&w, SIGNAL(requestAltitudeDelta()), &stat, SLOT(altitudeDeltaStats()) );
    QObject::connect(&w, SIGNAL(requestDistanceData()), &stat, SLOT(distanceStats()) );
    QObject::connect(&w, SIGNAL(requestDistanceDelta()), &stat, SLOT(distanceDeltaStats()) );
    QObject::connect(&w, SIGNAL(requestIntervalData()), &stat, SLOT(intervalStats()) );
    QObject::connect(&w, SIGNAL(requestFlightData()), &stat, SLOT(flightStats()) );
    QObject::connect(&w, SIGNAL(requestSpeedData()), &stat, SLOT(speedStats()) );
    //QObject::connect(&w, SIGNAL(requestSpeedData()), &stat, SLOT(calculate_intervalData()) );
    QObject::connect(&stat, SIGNAL(data(QLineSeries * const)), &w, SLOT(plotData(QLineSeries * const)) );

    w.show();

    return a.exec();
}
