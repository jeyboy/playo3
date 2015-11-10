#ifndef I_EQUALIZABLE
#define I_EQUALIZABLE

#include <qmap.h>
#include <qstringbuilder.h>

#include "ispectrumable.h"

#define LOW_INTERVAL 32 //16.352
#define HIGH_INTERVAL 16744 //15804

class EqualizablePreset {
    QMap<float, QString> bands;
    float preset_base;

    QString toStr(float val) { return QString::number(roundf(val)); }
public:
    EqualizablePreset() {}
    EqualizablePreset(float octaveOffset) { // 1/3 octave offset
        Q_ASSERT(octaveOffset > 1);
        preset_base = octaveOffset;

        float res = LOW_INTERVAL;
        while(res <= HIGH_INTERVAL) {
            bands.insert(res, toStr(res));
            res *= octaveOffset;
        }

        if (qAbs(res - HIGH_INTERVAL) < 1500)
            bands.insert(HIGH_INTERVAL, toStr(HIGH_INTERVAL));
    }

    QMap<float, QString> bandsList() const { return bands; }
    float presetBase() { return preset_base; }
};

class IEqualizable : public ISpectrumable {
    Q_OBJECT
protected:
    IEqualizable(QObject * parent = 0);

    virtual bool processEqSetGain(int band, float gain) = 0;
    virtual bool registerEQ() = 0;
    virtual bool unregisterEQ() = 0;
    virtual bool equalizable() { return true; }

    bool eq_in_use;
    QString current_preset_type;
    QMap<QString, EqualizablePreset> presets;
    QMap<int, float> eqBandsGains;
public:
    virtual ~IEqualizable() {}

    void eqBand(int band, float gain) {
        processEqSetGain(band, gain);
        eqBandsGains.insert(band, gain);
    }
    inline QMap<int, float> eqGains() const { return eqBandsGains; }
    inline void eqGains(QMap<int, float> gains) { eqBandsGains = gains; }
    inline QMap<float, QString> bands() const { return presets[current_preset_type].bandsList(); }
    inline float currentPresetBase() { return presets[current_preset_type].presetBase(); }
    inline QString currentPresetType() const { return current_preset_type; }
    inline bool eqInUse() { return eq_in_use; }
    inline QStringList presetTypesList() const { return presets.keys(); }
signals:
    void presetTypeChanged();
public slots:
    inline void changePresetType(QString newPresetType) {
        current_preset_type = newPresetType;
        emit presetTypeChanged();
    }
    //TODO: add changing of preset
    inline void activateEQ(bool activate) {
        if ((eq_in_use = activate)) registerEQ();
        else unregisterEQ();
    }
};

#endif // I_EQUALIZABLE
