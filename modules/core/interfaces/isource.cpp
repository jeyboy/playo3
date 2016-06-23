#include "isource.h"

#include "dialogs/relations_dialog.h"
#include "dockbars.h"

using namespace Core;

void ISource::openTab() {
    QString user_id = userID();

    if (connectUser(connection_new))
        Presentation::Dockbars::obj().createLinkedDocBar(
            Presentation::BarCreationNames(QString(name() % " [YOU]"), uidStr(user_id)),
            Models::Params(siteType(), user_id), 0, true, true, 0, true
        );
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
