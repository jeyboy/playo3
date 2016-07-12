#include "isource.h"

#include "dockbars.h"
#include "settings.h"
#include "dialogs/relations_dialog.h"
#include "dialogs/packages_dialog.h"

using namespace Core;

QToolButton * ISource::initButton(QWidget * parent) {
    bool is_sociable = isSociable();
    bool is_packable = hasPacks();

    bool offline_sociable = is_sociable && hasOfflineSociable();
    bool offline_packable = is_packable && hasOfflinePacks();

    bool offline_respondable = offline_sociable || offline_packable;

    if (!isConnectable() && !offline_respondable) return button;

    if (button == 0) {
        if (!parent) {
            qDebug() << "PIPEC" << name();
            return 0;
        }
        button = new QToolButton(parent);
    } else {
        button -> setMenu(0);
        disconnect(button, SIGNAL(clicked()), this, SLOT(openTab()));
    }

    bool is_connected = isConnected();
    button -> setEnabled(true);
    button -> setIcon(QIcon(QStringLiteral(":/add_%1").arg(name().toLower())));

    if (is_connected || offline_respondable) {
        if(!menu)
            menu = new QMenu(button);
        else
            menu -> clear();

        if (is_connected) {
            button -> setIcon(QIcon(QStringLiteral(":/add_%1_on").arg(name().toLower())));

            menu -> addAction(QStringLiteral("Disconect"), this, SLOT(disconnectUser()));
            menu -> addAction(QStringLiteral("Open your tab"), this, SLOT(openTab()));

            if (hasUserRecomendations())
                menu -> addAction(QStringLiteral("Open recommendations"), this, SLOT(openRecomendations()));
        }
        else
            menu -> addAction(QStringLiteral("Connect"), this, SLOT(openTab()));

        if (is_sociable)
            menu -> addAction(QStringLiteral("Open friend/group tab"), this, SLOT(openRelationTab()));

        if (is_packable)
            menu -> addAction(QStringLiteral("Open package tab"), this, SLOT(openPackageTab()));

        button -> setToolTip(name());
        button -> setPopupMode(QToolButton::InstantPopup);
        button -> setMenu(menu);
    } else {
        button -> setToolTip(QStringLiteral("Connect to %1").arg(name()));
        connect(button, SIGNAL(clicked()), this, SLOT(openTab()));
    }

    return button;
}

void ISource::openTab() {
    if (connectUser()) {
        QString user_id = userID();

        Presentation::Dockbars::obj().createLinkedDocBar(
            Presentation::BarCreationNames(QString(name() % " [YOU]"), uidStr(user_id)),
            Models::Params(siteType(), user_id), 0, true, true, 0, true
        );
    }
}

void ISource::openRecomendations() {
    QString user_id = userID(pr_media_content);

    Presentation::Dockbars::obj().createDocBar(
        QStringLiteral("Rec for YOU"),
        Models::Params(siteType(), user_id, rec_user), 0, true, true
    );
}

void ISource::openRelationTab() {
    RelationsDialog dialog(this, Settings::obj().anchorWidget());
    if (dialog.exec() == QDialog::Accepted)
        Presentation::Dockbars::obj().createLinkedDocBar(
            Presentation::BarCreationNames(
                QString(name() % " [") % dialog.getName() % QStringLiteral("]"),
                uidStr(dialog.getId())
            ),
            Models::Params(siteType(), dialog.getId()), 0, true, true
        );
}

void ISource::openPackageTab() {
    PackagesDialog dialog(this, Settings::obj().anchorWidget());
    if (dialog.exec() == QDialog::Accepted)
        Presentation::Dockbars::obj().createLinkedDocBar(
            Presentation::BarCreationNames(
                QString(name() % " [") % dialog.getName() % QStringLiteral("]"),
                uidStr(dialog.getParams())
            ),
            Models::Params(siteType(), dialog.getParams(), rec_set), 0, true, true
        );
}
