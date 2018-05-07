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

        if (qAbs(bands.lastKey() - HIGH_INTERVAL) > 1500)
            bands.insert(HIGH_INTERVAL, toStr(HIGH_INTERVAL));
    }

    QMap<float, QString> bandsList() const { return bands; }
    float presetBase() { return preset_base; }
};

class IEqualizable : public ISpectrumable {
    Q_OBJECT
protected:
    IEqualizable(QWidget * parent = 0);

    virtual bool processEqSetGain(const int & /*band*/, const float & /*gain*/) { return false; }
    virtual bool registerEQ() { return false; }
    virtual bool unregisterEQ() { return false; }
    virtual bool isSupportEqualizer() { return false; }

    bool eq_in_use;
    QString current_preset_type;
    QMap<QString, EqualizablePreset> presets;
    QMap<int, float> eqBandsGains;
public:
    virtual ~IEqualizable() {}

    void eqBand(const int & band, const float & gain) {
        processEqSetGain(band, gain);
        eqBandsGains.insert(band, gain);
    }
    inline QMap<int, float> eqGains() const { return eqBandsGains; }
    inline void eqGains(const QMap<int, float> & gains, const bool & attach = false) {
        if (attach) {
            for(QMap<int, float>::ConstIterator gain = gains.constBegin(); gain != gains.constEnd(); gain++)
                eqBand(gain.key(), gain.value());
        }
        else eqBandsGains = gains;
    }
    inline QMap<float, QString> bands() const { return presets[current_preset_type].bandsList(); }
    inline float currentPresetBase() { return presets[current_preset_type].presetBase(); }
    inline QString currentPresetType() const { return current_preset_type; }
    inline bool eqInUse() { return eq_in_use; }
    inline QStringList presetTypesList() const { return presets.keys(); }
signals:
    void presetTypeChanged();
public slots:
    inline void changePresetType(const QString & new_preset_type) {
        current_preset_type = new_preset_type;
        eqBandsGains.clear();
        if (eq_in_use) registerEQ();
        emit presetTypeChanged();
    }
    inline void activateEQ(const bool & activate) {
        if ((eq_in_use = activate)) registerEQ();
        else unregisterEQ();
    }
};

#endif // I_EQUALIZABLE
