#ifndef FUNC_CONTAINER_H
#define FUNC_CONTAINER_H

#include <QObject>

struct FuncContainer {
    FuncContainer() {}

    FuncContainer(const QObject * receiver, const char * recSlot) {
        obj = receiver;
        slot = recSlot;
    }

    const QObject * obj;
    const char * slot;
};

#endif // FUNC_CONTAINER_H
