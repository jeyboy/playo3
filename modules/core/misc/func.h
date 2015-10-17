#ifndef FUNC
#define FUNC

struct Func {
    inline Func() { }
    inline Func(QObject * receiver, const char * respSlot) {
        obj = receiver;
        slot = respSlot;
    }
    inline ~Func() {}

    QObject * obj;
    const char * slot;
};

#endif // FUNC
