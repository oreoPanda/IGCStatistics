#include "igcloader.h"
#include "debug.h"
#include <QFile>
#include <QTextStream>

IGCLoader::IGCLoader(QObject *parent) : QObject(parent)
{

}

void IGCLoader::load(const QString file)
{
    QFile source(file);
    if(!source.open(QIODevice::ReadOnly))
    {
        return;
    }
    QTextStream in(&source);
    QString line;

    forever
    {
        line = in.readLine(IGC_LINE_LEN);   //TODO IGC line len is 76 excluding CRLF. Does readLine include or exclude CRLF in len?
        if(!line.isNull())
        {
            this->lines << line;
        }
        else break;
    }
}

void IGCLoader::parse()
{
    QString line;
    //QString fix("B1023544352219S00609854EV007580000000001500000000"); //B record string
    double lastlat = 0;
    double lastlong = 0;

    for(int l = 0; l < lines.size(); l++)
    {
        struct Fix point;
        double degrees;
        ushort deg;
        ushort min;
        ushort decmin;
        char designator;

        line = lines.at(l);
        if(line.RECORDTYPE('B'))
        {
            //TODO evaluate bool
            bool ok = true;
            /*Read the UTC timestamp TODO evaluate bool ok*/
            point.time.hours = line.mid(TIMEOFFSET, TIMETOKLEN).toUShort(&ok);
            point.time.minutes = line.mid(TIMEOFFSET+TIMETOKLEN,TIMETOKLEN).toUShort(&ok);
            point.time.seconds = line.mid(TIMEOFFSET+2*TIMETOKLEN,TIMETOKLEN).toUShort(&ok);

            /*Read the GPS latitude and longitude*/
            for(int i = 0; i < 2; i++)
            {
                deg = line.mid(COORDOFFSET(i),DEGLEN(i)).toUShort(&ok);
                min = line.mid(COORDOFFSET(i)+DEGLEN(i),COORDMINLEN).toUShort(&ok);
                decmin = line.mid(COORDOFFSET(i)+DEGLEN(i)+COORDMINLEN,COORDDECMINLEN).toUShort(&ok);
                designator = line.at(DESIGOFFSET(i)).toLatin1();
                /*calculate degrees*/
                degrees = deg + (min + static_cast<double>(decmin)/1000) / 60;
                /*prepend negative sign if necessary and store in point*/
                if(i==0){
                    point.position.latitude = designator==DESIG(i) ? -1*degrees : degrees;
                    if((degrees - lastlat) > 0.01){
                        qDebug("Break");
                    }
                    lastlat = degrees;
                }else if(i==1){
                    point.position.longitude = designator==DESIG(i) ? -1*degrees : degrees;
                    if(fabs(degrees - lastlong) > 0.01){
                        qDebug("Break");
                    }
                    lastlong = degrees;
                }
            }

            /*Skip the fix validity and read the pressure and GNSS altitude*/
            point.altitude.pressureAlt = line.mid(ALTPRESSOFFSET, ALTLEN).toLong(&ok);
            point.altitude.GNSSAlt = line.mid(ALTGNSSOFFSET, ALTLEN).toLong(&ok);

            /*append the point to the list of fix records*/
            fixrecords.append(point);
        }
    }

    emit parsed(fixrecords);
}
