#ifndef CALL_INITIATOR
#define CALL_INITIATOR

#include <qobject.h>

struct CallInitiator {
    inline CallInitiator() { }
    inline CallInitiator(const QObject * receiver, const char * respSlot) : obj(receiver), slot(respSlot) {}
    inline ~CallInitiator() {}

    bool operator == (CallInitiator & oth) { return obj == oth.obj && slot == oth.slot; }

    const QObject * obj;
    const char * slot;
};

#endif // CALL_INITIATOR
