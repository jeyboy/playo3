#include "data_store.h"

using namespace Playo3;

DataStore::DataStore(const QString & name) : filename(name), _state(load()) { }
DataStore::~DataStore() {
//    save();
}

bool DataStore::load() {
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = qUncompress(loadFile.readAll());
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    json = Json::Obj::fromQJson(loadDoc.object());

    return true;
}
bool DataStore::save() {
    QFile saveFile(filename);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonDocument saveDoc(json);
    saveFile.write(qCompress(saveDoc.toJson(), 9));

    return true;
}

void DataStore::append(const QString & key, const QString & subkey, const QString & value) {
    Json::Obj subObj;
    subObj[subkey] = value;
    json.arr(key).append(subObj);
}
