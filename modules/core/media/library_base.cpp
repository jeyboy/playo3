#include "library_base.h"
#include "settings.h"

using namespace Core;
using namespace Media;

LibraryBase::LibraryBase() : QObject(), timeAmount(0), libraryPath(QCoreApplication::applicationDirPath() % QStringLiteral("/library/")) {
    saveTimer = new QTimer();
    QObject::connect(saveTimer, SIGNAL(timeout()), this, SLOT(saveCatalogs()));
    saveTimer -> start(Settings::obj().saveLibDelay());

    QDir dir(libraryPath);
    if (!dir.exists())
        dir.mkpath(".");
}

LibraryBase::~LibraryBase() {
    saveTimer -> stop();
    saveTimer -> deleteLater();
    save();

    qDeleteAll(catalogs);
}

void LibraryBase::saveCatalogs() {
    Logger::obj().write(QStringLiteral("Library"), QStringLiteral("Trying to save new data"));
    QtConcurrent::run(this, &LibraryBase::save);
    saveTimer -> setInterval(Settings::obj().saveLibDelay());
}

QChar LibraryBase::getCatalogName(const QString & name) {
    if (name.isEmpty()) return '_';
    return name.at(0);
}

QHash<QString, int> * LibraryBase::getCatalog(const QChar & letter) {
    if (catalogs.contains(letter))
        return catalogs.value(letter);
    else {
        QHash<QString, int> * res = load(letter);
        catalogs.insert(letter, res);
        return res;
    }
}

QHash<QString, int> * LibraryBase::getCatalog(const QString & name) {
    if (name.isEmpty()) return 0;
    return getCatalog(getCatalogName(name));
}

QHash<QString, int> * LibraryBase::load(const QChar letter) {
    QHash<QString, int> * res = new QHash<QString, int>();

    QFile f(libraryPath % QStringLiteral("cat_") % letter);
    if (f.open(QIODevice::ReadOnly)) {
        QByteArray ar;
        QString name;
        int state;

        while(!f.atEnd()) {
            ar = f.readLine();
            state = ar.mid(0, 1).toInt();
            name = QString::fromUtf8(ar.mid(1, ar.length() - 3));
            res -> insert(name, state);
        }

        f.close();
    }

    return res;
}

void LibraryBase::save() {
    if (saveBlock.tryLock()) {
        QHash<QString, int> * res;

        bool result;

        for(QHash<QChar, QList<QString> *>::iterator i = catalogs_state.begin(); i != catalogs_state.end();) {
            res = catalogs.value(i.key());

            if (i.value()) {
                result = fileDump(i.key(), *i.value(), QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);

                if(result) {
                    delete catalogs_state.value(i.key());
                    catalogs_state.insert(i.key(), 0);
                }
            } else {
                QList<QString> keys = res -> keys();
                result = fileDump(i.key(), keys, QIODevice::WriteOnly | QIODevice::Text);
            }

            if (result)
                i = catalogs_state.erase(i);
            else
                i++;
        }

        saveBlock.unlock();
    }
}

bool LibraryBase::fileDump(QChar key, QList<QString> & keysList, QFlags<QIODevice::OpenModeFlag> openFlags) {
    QHash<QString, int> * res = catalogs.value(key);

    QFile f(libraryPath % "cat_" % key);
    if (f.open(openFlags)) {
        QTextStream out(&f);
        out.setCodec("UTF-8");

        for(QList<QString>::const_iterator cat_i = keysList.cbegin(); cat_i != keysList.cend(); cat_i++)
            out << QString::number(res -> value(*cat_i)) << (*cat_i).toUtf8() << '\n';

        f.close();

        return true;
    }

    return false;
}

