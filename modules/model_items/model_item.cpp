#include "model_item.h"

namespace ModelItems {
//    #include "model_item.h"
//    #include "media/library.h"
//    #include "media/player.h"
//    #include "misc/rand.h"
//    #include <algorithm>

/////////////////////////////////////////////////////////

    ModelItem::ModelItem(int initState) {
        parentItem = 0;
        titlesCache = 0;
        state = new ModelItemState(initState);
        path = QString();
        title = QString("--(O_o)--");
        extension = QString();
        genreID = -1;
        duration = -1;
        size = -1;
        info = -1;
        bpm = 0;
    }

    ModelItem::ModelItem(QJsonObject * hash, ModelItem * parent) {
        parentItem = parent;
        titlesCache = 0;
        state = new ModelItemState(hash -> value("s").toInt());
        path = hash -> value("p").toString();
        title = hash -> value("t").toString();
        extension = hash -> value("e").toString();
        genreID = hash -> value("g").toInt(-1);
        duration = hash -> value("d").toString("");

        size = hash -> value("b").toInt(-1);
        info = hash -> value("a").toString("");
        bpm = hash -> value("m").toInt(0);

        if (parent != 0) {
           parent -> appendChild(this);
        }
    }

    ModelItem::ModelItem(const QString filePath, QString fileName, ModelItem * parent, int genre_id, QString itemDuration, int itemSize, QString itemInfo, int initState) {
        parentItem = parent;
        titlesCache = 0;
        size = itemSize;
        info = itemInfo;
        state = new ModelItemState(initState);

        title = fileName.replace("\n", " ");
        path = filePath;
        genreID = genre_id;
        bpm = 0;
        duration = itemDuration;
    //    extension = fileExtension;

        if (parent != 0) {
           parent -> appendChild(this);
        }
    }

    ModelItem::~ModelItem() {
        delete state;
        delete titlesCache;
    }

    QString ModelItem::fullPath() const {
    //    if (parentItem != 0) {
    //        return parentItem -> fullPath() + '/' + path;
    //    }

    //    return "file://";


        ModelItem * curr = parentItem;
        QString path_buff = path;

        while(curr != 0 && !curr -> path.isEmpty()) {
            path_buff = curr -> path + '/' + path_buff;
            curr = curr -> parentItem;
        }

    #ifdef Q_OS_LINUX
        path_buff = '/' + path_buff;
    //#elif defined(Q_OS_WINDOWS)

    #endif

        return path_buff;
    //    return path_buff.mid(1);
    }

    QString ModelItem::getDownloadTitle() const {
        QString ret = Library::instance() -> filenameFilter(getTitle());

        if (!extension.isEmpty())
            ret = ret + '.' + extension;

        return ret;
    }

    void ModelItem::openLocation() {
        if (!parent() -> fullPath().isEmpty())
            QDesktopServices::openUrl(parent() -> toUrl());
        else {
            QFileInfo info(fullPath());
            QDesktopServices::openUrl(QUrl::fromLocalFile(info.path()));
        }
    }

    QStringList ModelItem::getInfo() const {
        QStringList list;
        if (info.isEmpty())
            list.append("Processing ...");
        else
            list.append(info);
        list.append(duration);

        return list;
    }

    void ModelItem::accumulateUids(QHash<ModelItem*, QString> & store) {
        if (isFolder()) {
            foreach(ModelItem * item, *childItemsList())
                item -> accumulateUids(store);
        } else {
            QString currUid = toUID();
            if (!currUid.isEmpty()) {
                store.insert(this, currUid);
            }
        }
    }

    QUrl ModelItem::toUrl() {
        return QUrl::fromLocalFile(fullPath());
    }

    QJsonObject ModelItem::toJSON() {
        QJsonObject root = QJsonObject();

        root["t"] = title;
        root["s"] = state -> getFuncValue();
        root["p"] = path;

        if (!info.isEmpty())
            root["a"] = info;

        if (bpm != 0)
            root["m"] = bpm;

        if (size != -1)
            root["b"] = size;

        if (genreID != -1)
            root["g"] = genreID;

        if (!duration.isEmpty())
            root["d"] = duration;

        if (!extension.isNull())
            root["e"] = extension;

        return root;
    }

    ModelItem * ModelItem::toModelItem() {
        return dynamic_cast<ModelItem *>(this);
    }

    QString ModelItem::toPath() const {
        if (parentItem == 0)
            return "";
        else return parentItem -> toPath() + " " + QString::number(row());
    }


    ModelItem * ModelItem::parent() {
        return parentItem;
    }

    ModelItem *ModelItem::child(int) { return 0; }

    int ModelItem::childTreeCount() const { return 0; }

    int ModelItem::childCount() const { return 0; }

    void ModelItem::insertChild(int, ModelItem *) { }

    void ModelItem::appendChild(ModelItem *) {}

    bool ModelItem::removeChildren(int, int) { return false; }


    int ModelItem::column() const { return 0; }

    int ModelItem::columnCount() const { return 1; }


    int ModelItem::row() const {
         if (parentItem)
             return parentItem -> childItemsList() -> indexOf(const_cast<ModelItem *>(this));

         return 0;
    }


    QVariant ModelItem::data(int column) const {
        switch(column) {
            case TITLEID: return getTitle();
            case EXTENSIONID: return extension;
            case PATHID: return path;
            case FOLDERID: return isFolder();
            case TITLESCACHEID: return QVariant(*getTitlesCache());
            case STATEID: return getState() -> currStateValue();
            default: return QVariant();
        }
    }

    bool ModelItem::setData(int column, const QVariant &value) {
        if (column < 0 || column >= 2)
    //    if (column < 0 || column >= itemData.size())
            return false;

    //    itemData[column] = value;

        switch(column) {
            case TITLEID: { title = value.toString(); break; }
            case EXTENSIONID: { extension = value.toString(); break; }
            case PATHID: { path = value.toString(); break; }
            case STATEID: { setState(value.toInt(), false); break;}
        }

        return true;
    }

    ModelItemState *ModelItem::getState() const {
        return state;
    }

    void ModelItem::setState(int new_state, bool append_to_library) {
        if (state -> setBit(new_state) && append_to_library) {
            if (state -> isListened())
                Library::instance() -> addItem(this, STATE_LISTENED);
            else if (state -> isLiked())
                Library::instance() -> addItem(this, STATE_LIKED);
        }
    }


    void ModelItem::dropExpandProceedFlags() {}


    QHash<QString, ModelItem *> *ModelItem::foldersList() const { return 0; }
    int ModelItem::removeFolder(QString) { return -1; }

    void ModelItem::changeCheckedState(bool checked) {
        if (checked)
            getState() -> setChecked();
        else
            getState() -> unsetChecked();
    }

    bool ModelItem::cacheIsPrepared() const {
        return titlesCache != 0;
    }

    void ModelItem::setCache(QList<QString> * newCache) {
        titlesCache = newCache;
    }

    void ModelItem::addToCache(QString title) {
        if (!titlesCache -> contains(title))
            titlesCache -> append(title);
    }
    QList<QString> *ModelItem::getTitlesCache() const {
        return titlesCache;
    }

    QList<ModelItem *> * ModelItem::childItemsList() { return 0;}

    void ModelItem::shuffle() {
        if (isFolder()) {
            Rand::shuffle(childItemsList());

            foreach(ModelItem * item, *childItemsList())
                item -> shuffle();
        }
    }

}
