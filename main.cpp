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

    QObject::connect(&w, SIGNAL(requestData(DataType)), &stat, SLOT(getData(DataType)) );

    //QObject::connect(&w, SIGNAL(requestSpeedData()), &stat, SLOT(calculate_intervalData()) );
    QObject::connect(&stat, SIGNAL(data(const QList<QPointF> &)), &w, SLOT(plotData(const QList<QPointF> &)) );

    w.show();

    return a.exec();
}
