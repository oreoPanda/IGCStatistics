#ifndef IGCLOADER_H
#define IGCLOADER_H

#include "igctypes.h"
#include <QObject>

#define RECORDTYPE(t) startsWith(t)

#define TIMEOFFSET 1
#define TIMETOKENS 3
#define TIMETOKLEN 2

#define LATOFFSET TIMEOFFSET + TIMETOKENS * TIMETOKLEN
#define LATDEGLEN 2
#define COORDMINLEN 2
#define COORDDECMINLEN 3
#define LONOFFSET LATOFFSET + LATDEGLEN + COORDMINLEN + COORDDECMINLEN + 1
#define LONDEGLEN 3

#define COORDOFFSET(i) (i==0 ? LATOFFSET : LONOFFSET)
#define DEGLEN(i) (i==0 ? LATDEGLEN : LONDEGLEN)

#define DESIGOFFSET(i) (i==0 ? LATOFFSET + LATDEGLEN + COORDMINLEN + COORDDECMINLEN : LONOFFSET + LONDEGLEN + COORDMINLEN + COORDDECMINLEN)
#define DESIG(i) (i==0 ? 'S' : 'W')

#define ALTLEN 5
#define ALTPRESSOFFSET DESIGOFFSET(1) + 2
#define ALTGNSSOFFSET ALTPRESSOFFSET + ALTLEN

class IGCLoader : public QObject
{
    Q_OBJECT
public:
    explicit IGCLoader(QObject *parent = 0);

signals:
    void parsed(const QList<Fix> & fixes);

public slots:
    void load(const QString file);
    void parse();

private:
    QStringList lines;
    QList<Fix> fixrecords;
};

#endif // IGCLOADER_H
