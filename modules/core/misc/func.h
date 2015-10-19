#ifndef FUNC
#define FUNC

#include <string.h>

struct Func {
    inline Func() { }
    inline Func(QObject * receiver, const char * respSlot, bool return_reference = false) : obj(receiver), ret_reference(return_reference) {
        char * entry = strchr(respSlot, '(');
        if (entry) {
            int len = entry - respSlot;
            char * substr = new char[len];
            strncpy(substr, respSlot + 1, len - 1); substr[len - 1] = '\0';
            slot = substr;
            delete [] respSlot;
        } else
            slot = respSlot;
    }
    inline ~Func() {
        delete [] slot;
    }

    QObject * obj;
    const char * slot;
    bool ret_reference;
};

#endif // FUNC
