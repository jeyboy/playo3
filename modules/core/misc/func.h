#ifndef FUNC
#define FUNC

#include <qstring.h>
#include <qlist.h>
#include <qobject.h>

struct Func {
    inline Func() { }
    inline Func(QObject * receiver, const char * respSlot) : obj(receiver) {
        QString slt = QString(respSlot).mid(1);

        QStringList parts = slt.split('(', QString::SkipEmptyParts);

        if (parts.length() == 1)
            slot = parts[0];
        else {
            slot = parts.at(0);
            args = parts.at(1).section(')', 0, 0).split(',', QString::SkipEmptyParts);

            for(QStringList::Iterator it = args.begin(); it != args.end(); it++)
                (*it) = (*it).trimmed();
        }

        delete [] respSlot;
    }
    inline ~Func() {}

    QObject * obj;
    QString slot;
    QStringList args;
};

#endif // FUNC
