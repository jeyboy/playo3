#ifndef FUNC
#define FUNC

#include <qstring.h>
#include <qlist.h>
#include <qpointer.h>

struct Func {
    inline Func() { }
    inline Func(QObject * receiver, const char * respSlot, void * userData = 0) : obj(QPointer<QObject>(receiver)), user_data(userData) {
        const char * entry = strchr(respSlot, '(');
        int len =  entry ? (entry - respSlot) : strlen(respSlot);
        strncpy(slot, respSlot + 1, len - 1); slot[len - 1] = '\0';

        if (entry) {
            const char * entry2 = strchr(entry, ')');

            if (entry2) {
                len = entry2 - entry;

                const char * comma_entry = strchr(entry, ',');

                if (comma_entry)
                    len = comma_entry - entry;

                strncpy(arg, entry + 1, len - 1); arg[len - 1] = '\0';
            }
        } else arg[0] = '\0';
    }
    inline ~Func() {}

    QPointer<QObject> obj;
    char slot[512];
    char arg[512];
    void * user_data;
};

#endif // FUNC
