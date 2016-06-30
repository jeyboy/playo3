#include "isource.h"

#include <qmenu.h>

#include "dialogs/relations_dialog.h"
#include "dockbars.h"

using namespace Core;

QToolButton * ISource::initButton(QWidget * parent) {
    if (!isConnectable()) return button;

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

    if (isConnected()) {
        button -> setIcon(QIcon(QStringLiteral(":/add_%1_on").arg(name().toLower())));
        button -> setToolTip(name());
        button -> setPopupMode(QToolButton::InstantPopup);

        QMenu * menu = new QMenu(button);
        menu -> addAction(QStringLiteral("Disconect"), this, SLOT(disconnectUser()));
        menu -> addAction(QStringLiteral("Open your tab"), this, SLOT(openTab()));

        if (isSociable())
            menu -> addAction(QStringLiteral("Open friend/group tab"), this, SLOT(openRelationTab()));

        if (hasRecomendations())
            menu -> addAction(QStringLiteral("Open recommendations"), this, SLOT(openRecomendations()));

        button -> setMenu(menu);
    } else {
        button -> setIcon(QIcon(QStringLiteral(":/add_%1").arg(name().toLower())));
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
            Presentation::BarCreationNames(QString(name() % " [") % dialog.getName() % QStringLiteral("]"),
            uidStr(dialog.getId())),
            Models::Params(siteType(), dialog.getId()), 0, true, true
        );
}
