#ifndef I_EQUALIZABLE
#define I_EQUALIZABLE

#include <qmap.h>
#include <qstringbuilder.h>

#include "ispectrumable.h"

#define LOW_INTERVAL 16.352
#define HIGH_INTERVAL 15804

class EqualizablePreset {
    QMap<float, QString> bands;

    QString toStr(float val) { return QString::number(roundf(val)); }
public:
    EqualizablePreset(float octaveOffset = 1.33) { // 1/3 octave offset
        Q_ASSERT(octaveOffset > 1);

        float res = LOW_INTERVAL;
        while(res < HIGH_INTERVAL) {
            bands.insert(res, toStr(res));
            res *= octaveOffset;
        }
    }

    QMap<float, QString> bandsList() const { return bands; }
};

class IEqualizable : public ISpectrumable {
    Q_OBJECT
protected:
    IEqualizable(QObject * parent = 0);

    virtual bool registerEQ() = 0;
    virtual bool unregisterEQ() = 0;
    virtual bool equalizable() { return true; }

    bool eqInUse;
    QString current_preset;
    QMap<QString, EqualizablePreset> presets;
    QMap<int, int> eqBandsGains;
public:
    virtual ~IEqualizable() {}

    inline QMap<int, int> eqGains() const { return eqBandsGains; }
    inline void eqGains(QMap<int, int> gains) { eqBandsGains = gains; }
    inline QMap<float, QString> bands() const { return presets[current_preset].bandsList(); }
public slots:
    inline void activateEQ(bool activate) {
        if ((eqInUse = activate)) registerEQ();
        else unregisterEQ();
    }
};

#endif // I_EQUALIZABLE
