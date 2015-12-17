#ifndef FUNC
#define FUNC

#include <qstring.h>
#include <qlist.h>
#include <qobject.h>

struct Func {
    inline Func() { }
    inline Func(QObject * receiver, const char * respSlot, void * userData = 0) : obj(receiver), user_data(userData) {
        char * entry = strchr(respSlot, '(');
        int len =  entry ? (entry - respSlot) : strlen(respSlot);
        strncpy(slot, respSlot + 1, len - 1); slot[len - 1] = '\0';

        if (entry) {
            char * entry2 = strchr(entry, ')');

            if (entry2) {
                len = entry2 - entry;
                strncpy(arg, entry + 1, len - 1); arg[len - 1] = '\0';
            }
        } else arg[0] = '\0';

        delete [] respSlot;
    }
    inline ~Func() {}

    QObject * obj;
    char slot[512];
    char arg[512];
    void * user_data;
};

#endif // FUNC
