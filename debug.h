#ifndef DEBUG_H
#define DEBUG_H

#include <Qt>

void debugif(bool expr)
{
    if(expr)
    {
        qDebug("Breakpoint");
    }
}

#endif // DEBUG_H
