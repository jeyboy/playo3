#include "tab.h"

void Tab::init(CBHash params, QJsonObject * hash) {
    switch(params["t"]) {
        case VIEW_LIST: {
            view = (View *)new ListView(this, params, hash);
        break;}
        case VIEW_LEVEL_TREE: {
            view = (View *)new LevelTreeView(this, params, hash);
        break;}
        case VIEW_LEVEL_TREE_BREADCRUMB: {
            view = (View *)new LevelTreeBreadcrumbView(this, params, hash);
        break;}
        case VIEW_VK: {
            if (hash != 0 && hash -> contains("uid"))
                params.insert("uid", hash -> value("uid").toString().toInt());
            view = (View *)new VkView(this, params, hash);
        break;}
        case VIEW_SOUNDCLOUD: {
            if (hash != 0 && hash -> contains("uid"))
                params.insert("uid", hash -> value("uid").toString().toInt());
            view = (View *)new SoundcloudView(this, params, hash);
        break;}
        default: {
            view = (View *)new TreeView(this, params, hash);
        break;}
    }
//    view -> setResizeMode();

    this -> setLayout(new QBoxLayout(QBoxLayout::TopToBottom));
    this -> layout() -> addWidget(view);


    spinnerContainer = new QLabel;
    spinnerContainer -> setHidden(true);
    spinnerContainer -> setPixmap(QPixmap(":/sync"));
    this -> layout() -> addWidget(spinnerContainer);
    this -> layout() -> setAlignment(spinnerContainer, Qt::AlignCenter);


    this -> layout() -> setContentsMargins(0, 0, 0, 0);
    tabber = (QTabWidget *)parent();

    connect(view, SIGNAL(showSpinner()), this, SLOT(startRoutine()));
    connect(view, SIGNAL(hideSpinner()), this, SLOT(stopRoutine()));

    if (hash == 0 && (params["t"] == VIEW_VK))
        view -> getModel() -> refresh();
}

Tab::Tab(CBHash params, QWidget * parent) : QWidget(parent) {
    init(params);
}

Tab::Tab(QJsonObject hash, QWidget * parent) : QWidget(parent) {
    QJsonObject set = hash["set"].toObject();
    CBHash params = CBHash();

    foreach(QString key, set.keys()) {
        params.insert(key, set[key].toInt());
    }

    init(params, &hash);
}

Tab::~Tab() {
    delete view;
}

QString Tab::getName() const {
    int i = tabber -> indexOf((QWidget*)this);
    return tabber -> tabText(i).split('(').first(); //.section('(', 0, -1);
}
void Tab::setName(QString newName) {
    setNameWithCount(newName);
}

View * Tab::getView() const {
    return view;
}

void Tab::updateHeader(int /*new_count*/) {
    setNameWithCount(getName());
}

void Tab::setNameWithCount(QString name) {
    int i = tabber -> indexOf((QWidget*)this);
    tabber -> setTabText(i, name + "(" + QString::number(view -> getModel() -> itemsCount()) + ")");
}

QJsonObject Tab::toJSON(QString name) {
    QJsonObject res = view -> toJSON();
    res["n"] = name;
    if (Player::instance() -> currentPlaylist() == view) {
        res["pv"] = true;
        res["pp"] = Player::instance() -> playedItem() -> toPath();

        if (!Player::instance() -> playedItem() -> isRemote())
            res["pt"] = Player::instance() -> getPosition();
    }
    return res;
}

bool Tab::isEditable() const {
    return view -> isEditable();
}

void Tab::startRoutine() {
    view -> setHidden(true);
    spinnerContainer -> setHidden(false);
}
void Tab::stopRoutine() {
    spinnerContainer -> setHidden(true);
    view -> setHidden(false);
}

