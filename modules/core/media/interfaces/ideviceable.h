#ifndef IDEVICEABLE_H
#define IDEVICEABLE_H

#include <qhash.h>
#include <qvariant.h>

class IDeviceable {
protected:
    virtual bool isSupportOutputDeviceChange() const { return false; }
public:
    QString defaultDeviceName() { return QStringLiteral("Default"); }

    virtual QHash<QString, QVariant> outputDeviceList() { return {}; }
    virtual QVariant currOutputDevice() { return QVariant(); }
    virtual bool setOutputDevice(const QString & /*device_name*/) { return false; }
};

#endif // IDEVICEABLE_H
