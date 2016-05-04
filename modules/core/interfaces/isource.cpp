#include "isource.h"

#include "dialogs/relations_dialog.h"
#include "dockbars.h"

using namespace Core;

void ISource::openTab(const QString & userID) {
    if (connectUser(connection_manual))
        Presentation::Dockbars::obj().createLinkedDocBar(
            Presentation::BarCreationNames(QString(name() % " [YOU]"), uidStr(userID)),
            Models::Params(siteType(), userID), 0, true, true, 0, true
        );
}

void ISource::openRecomendations(const QString & userID) {
    Presentation::Dockbars::obj().createDocBar(
        QStringLiteral("Rec for YOU"),
        Models::Params(siteType(), userID, rel_user), 0, true, true
    );
}

void ISource::openRelationTab(Web::Sociable * currApi) {
    RelationsDialog dialog(currApi, Settings::obj().anchorWidget());
    if (dialog.exec() == QDialog::Accepted)
        Presentation::Dockbars::obj().createLinkedDocBar(
            Presentation::BarCreationNames(QString(name() % " [") % dialog.getName() % QStringLiteral("]"),
            uidStr(dialog.getId())),
            Models::Params(siteType(), dialog.getId(), rel_user), 0, true, true
        );
}
