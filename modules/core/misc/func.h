#ifndef FUNC
#define FUNC

#include <qstring.h>
#include <qlist.h>
#include <qobject.h>

struct Func {
    inline Func() { }
    inline Func(QObject * receiver, const char * respSlot) : obj(receiver), arg(0) {
        char * entry = strchr(respSlot, '(');
        int len =  entry ? (entry - respSlot) : strlen(respSlot);
        char * substr = new char[len];
        strncpy(substr, respSlot + 1, len - 1); substr[len - 1] = '\0';
        slot = substr;

        if (entry) {
            char * entry2 = strchr(entry, ')');

            if (entry2) {
                len = entry2 - entry;
                char * argstr = new char[len];
                strncpy(argstr, entry + 1, len - 1); argstr[len - 1] = '\0';
                arg = argstr;
            }
        }

        qDebug() << receiver << slot << arg << respSlot;

        delete [] respSlot;
    }
    inline ~Func() {
        delete [] slot;
        delete [] arg;
    }

    QObject * obj;
    const char * slot;
    const char * arg;
};

#endif // FUNC
