#include "data_store.h"
#include "settings.h"

using namespace Core;

DataStore::DataStore(const QString & name) {
    filename = APP_PATH(name);
    state = load();
}
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
    json = loadDoc.object();

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

void DataStore::clear() {
    json = QJsonObject();
}

QStringList DataStore::keys() {
    return json.keys();
}

QJsonValue DataStore::read(QString key) {
    return json.value(key);
}

void DataStore::write(const QString & key, const double & value) {
    json[key] = value;
}

void DataStore::write(const QString & key, const QJsonArray & value) {
    json[key] = value;
}
void DataStore::write(const QString & key, const QJsonObject & value) {
    json[key] = value;
}
void DataStore::write(const QString & key, const QString & value) {
    json[key] = value;
}
void DataStore::write(const QString & key, const int & value) {
    json[key] = value;
}
void DataStore::write(const QString & key, const QVariant & value) {
    json.insert(key, QJsonValue::fromVariant(value));
}

void DataStore::append(const QString & key, const QString & subkey, const QString & value) {
    QJsonObject subObj;
    subObj[subkey] = value;
    json[key].toArray().append(subObj);
}
