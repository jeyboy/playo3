#include "hotkey_settings.h"

HotkeySettings::HotkeySettings() {
    _humanizeHotkeyText.insert(HOTKEY_NEXT, QStringLiteral("Activate next item"));
    _humanizeHotkeyText.insert(HOTKEY_NEXT_AND_DELETE, QStringLiteral("Remove current and activate next"));
    _humanizeHotkeyText.insert(HOTKEY_PREV, QStringLiteral("Activate prev item"));
    _humanizeHotkeyText.insert(HOTKEY_PLAY, QStringLiteral("Play/pause"));
    _humanizeHotkeyText.insert(HOTKEY_STOP, QStringLiteral("Stop"));
    _humanizeHotkeyText.insert(HOTKEY_OPEN_SETTINGS, QStringLiteral("Active tab settings"));
    _humanizeHotkeyText.insert(HOTKEY_POS_SLIDE_FORWARD, QStringLiteral("Move position slider at 10 % forward"));
    _humanizeHotkeyText.insert(HOTKEY_POS_SLIDE_BACKWARD, QStringLiteral("Move position slider at 10 % backward"));
    _humanizeHotkeyText.insert(HOTKEY_VOL_SLIDE_FORWARD, QStringLiteral("Move volume slider at 10 % forward"));
    _humanizeHotkeyText.insert(HOTKEY_VOL_SLIDE_BACKWARD, QStringLiteral("Move volume slider at 10 % backward"));
}

void HotkeySettings::fromJson(const QJsonObject & settings) {
    setHotKeys(settings.value(QStringLiteral("hotkeys")).toObject());
}

void HotkeySettings::toJson(QJsonObject & settings) {
    settings.insert(QStringLiteral("hotkeys"), _hotkeys);
}

QList<HotkeyModelItem *> HotkeySettings::hotKeys() const { //TODO: rewrite on iterator usage
    QVector<QVariant> itemVals;
    QList<HotkeyModelItem *> ret;
    int iKey;

    foreach(QString key, _hotkeys.keys()) {
        iKey = key.toInt();
        itemVals = QVector<QVariant>();
        itemVals.append(_humanizeHotkeyText.value(iKey));
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
