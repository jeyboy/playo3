#ifndef IDEVICEABLE_H
#define IDEVICEABLE_H

#include <qhash.h>
#include <qvariant.h>

class IDeviceable {
protected:
    virtual bool deviceable() const { return true; }
public:
    virtual QHash<QString, QVariant> deviceList() { return {}; }
    virtual QVariant currDevice() { return QVariant(); }
    virtual bool setDevice(const QVariant & /*device*/) { return false; }
};

#endif // IDEVICEABLE_H
