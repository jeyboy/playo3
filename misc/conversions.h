#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <QtCore>

QString timeToStr(qint64 time) {
    return QString("%1:%2").arg(time / 60).arg(time % 60, 2, 10, QChar('0'));
}

#endif // CONVERSIONS_H
