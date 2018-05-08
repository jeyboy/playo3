#include "hotkey_settings.h"

HotkeySettings::HotkeySettings() {}

void HotkeySettings::fromJson(const QJsonObject & settings) {
    _disabled = settings.value(SETTINGS_DISABLE_HOTKEYS_KEY).toBool();
    _slide_percent = settings.value(SETTINGS_HOTKEY_SLIDE_PERCENT_KEY).toDouble(0.05);

    _humanize_hotkey_text.insert(HOTKEY_NEXT, QLatin1String("Activate next item"));
    _humanize_hotkey_text.insert(HOTKEY_NEXT_AND_DELETE, QLatin1String("Remove current and activate next"));
    _humanize_hotkey_text.insert(HOTKEY_PREV, QLatin1String("Activate prev item"));
    _humanize_hotkey_text.insert(HOTKEY_PLAY, QLatin1String("Play/pause"));
    _humanize_hotkey_text.insert(HOTKEY_STOP, QLatin1String("Stop"));
    _humanize_hotkey_text.insert(HOTKEY_OPEN_SETTINGS, QLatin1String("Active tab settings"));
    _humanize_hotkey_text.insert(HOTKEY_POS_SLIDE_FORWARD, "Move position slider at " + QString::number(_slide_percent * 100) + "% forward");
    _humanize_hotkey_text.insert(HOTKEY_POS_SLIDE_BACKWARD, "Move position slider at " + QString::number(_slide_percent * 100) + "% backward");
    _humanize_hotkey_text.insert(HOTKEY_VOL_SLIDE_FORWARD, "Move volume slider at " + QString::number(_slide_percent * 100) + "% forward");
    _humanize_hotkey_text.insert(HOTKEY_VOL_SLIDE_BACKWARD, "Move volume slider at " + QString::number(_slide_percent * 100) + "% backward");

    setHotKeys(settings.value(SETTINGS_HOTKEYS_KEY).toObject());
}

void HotkeySettings::toJson(QJsonObject & settings) {
    settings.insert(SETTINGS_HOTKEYS_KEY, _hotkeys);
    settings.insert(SETTINGS_DISABLE_HOTKEYS_KEY, _disabled);
    settings.insert(SETTINGS_HOTKEY_SLIDE_PERCENT_KEY, _slide_percent);
}

QList<HotkeyModelItem *> HotkeySettings::hotKeys() const { //TODO: rewrite on iterator usage
    QVector<QVariant> itemVals;
    QList<HotkeyModelItem *> ret;
    int iKey;

    foreach(QString key, _hotkeys.keys()) {
        iKey = key.toInt();
        itemVals = QVector<QVariant>();
        itemVals.append(_humanize_hotkey_text.value(iKey));
        itemVals.append(_hotkeys.value(key));
        itemVals.append(iKey);
        ret.append(new HotkeyModelItem(itemVals));
    }

    return ret;
}

void HotkeySettings::setHotKeys(QList<HotkeyModelItem *> hotkeyList) { //TODO: rewrite on iterator usage
    foreach(HotkeyModelItem * item, hotkeyList)
        _hotkeys.insert(QString::number(item -> data(2).toInt()), QJsonValue::fromVariant(item -> data(1)));
}

void HotkeySettings::setHotKeys(const QJsonObject & hotkeysHash) {
    if (hotkeysHash.empty()) {
        _hotkeys.insert(QString::number(HOTKEY_NEXT), QJsonValue::fromVariant("Ctrl+Down"));
        _hotkeys.insert(QString::number(HOTKEY_NEXT_AND_DELETE), QJsonValue::fromVariant("Ctrl+Delete"));
        _hotkeys.insert(QString::number(HOTKEY_PREV), QJsonValue::fromVariant("Ctrl+Up"));
        _hotkeys.insert(QString::number(HOTKEY_PLAY), QJsonValue::fromVariant("Media Play"));
        _hotkeys.insert(QString::number(HOTKEY_STOP), QJsonValue::fromVariant("Media Stop"));
        _hotkeys.insert(QString::number(HOTKEY_OPEN_SETTINGS), QJsonValue::fromVariant("Ctrl+Q"));

        _hotkeys.insert(QString::number(HOTKEY_POS_SLIDE_FORWARD), QJsonValue::fromVariant("Ctrl+Alt+Right"));
        _hotkeys.insert(QString::number(HOTKEY_POS_SLIDE_BACKWARD), QJsonValue::fromVariant("Ctrl+Alt+Left"));
        _hotkeys.insert(QString::number(HOTKEY_VOL_SLIDE_FORWARD), QJsonValue::fromVariant("Ctrl+Shift+Right"));
        _hotkeys.insert(QString::number(HOTKEY_VOL_SLIDE_BACKWARD), QJsonValue::fromVariant("Ctrl+Shift+Left"));
    }
    else _hotkeys = hotkeysHash;
}
