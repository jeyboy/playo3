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


// TODO: update open tab for using icmd as params str
void ISource::openTab() {
    if (connectUser()) {
        QString user_id = userID();

        Presentation::Dockbars::obj().createLinkedDocBar(
            Presentation::BarCreationNames(QString(name() % " [YOU]"), uidStr(user_id)),
            Models::Params(sourceType(), Models::mpf_none, user_id), 0, true, true, 0, true
        );
    }
}

void ISource::openRecomendations() {
    QString user_id = userID(pr_media_content);

    Presentation::Dockbars::obj().createDocBar(
        QStringLiteral("Rec for YOU"),
        Models::Params(sourceType(), Models::mpf_none, user_id, rec_user), 0, true, true
    );
}

void ISource::openRelationTab() {
    RelationsDialog dialog(this, Settings::obj().anchorWidget());
    if (dialog.exec() == QDialog::Accepted) {
        QString dialog_params = dialog.getId();

        Presentation::Dockbars::obj().createLinkedDocBar(
            Presentation::BarCreationNames(
                QString(name() % " [") % dialog.getName() % QStringLiteral("]"),
                uidStr(dialog_params)
            ),
            Models::Params(sourceType(), Models::mpf_auto_play_next, dialog_params), 0, true, true
        );
    }
}

void ISource::openPackageTab() {
    PackagesDialog dialog(this, Settings::obj().anchorWidget());
    if (dialog.exec() == QDialog::Accepted) {
        QString dialog_params = dialog.getParams();
        QString uid_postfix;
        QVariant data;
        Cmd cmnd = Cmd(dialog_params); // we should use source type from cmd, because it contains additional flags clarification

        bool source_configurable = cmnd.attrs.hasQueryItem(CMD_SOURCE_CONFIGURABLE);
        bool stream_configurable = cmnd.attrs.hasQueryItem(CMD_STREAM_CONFIGURABLE);
        bool feeds_configurable = cmnd.attrs.hasQueryItem(CMD_FEEDS_CONFIGURABLE);

        Models::ParamFlags configs = (Models::ParamFlags)(
            (source_configurable ? Models::mpf_source_configurable : Models::mpf_none) |
            (feeds_configurable ? Models::mpf_feeds_configurable : Models::mpf_none) |
            (stream_configurable ? Models::mpf_stream_configurable : Models::mpf_none)
        );

        if (stream_configurable) {
            data = streamConfiguration(cmnd.attrs);
            uid_postfix = Cmd::variantToStr(data);
        }

        Presentation::Dockbars::obj().createLinkedDocBar(
            Presentation::BarCreationNames(
                name() % LSTR(" [") % dialog.getName() % '|' % uid_postfix % ']',
                uidStr(dialog_params % uid_postfix)
            ),
            Models::Params(
                (DataSubType)cmnd.source_type,
                (Models::ParamFlags)(Models::mpf_auto_play_next | configs),
                dialog_params, rec_set, data
            ),
            0, true, true
        );
    }
}
