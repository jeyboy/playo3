#include "isource.h"

#include "dockbars.h"
#include "settings.h"
#include "dialogs/relations_dialog.h"
#include "dialogs/packages_dialog.h"

using namespace Core;

QToolButton * ISource::initButton(QWidget * parent) {
    bool is_connectable = isConnectable();
    bool is_sociable = respondableToSocial();
    bool is_packable = respondableToCompilations();
    bool offline_respondable = is_sociable || is_packable;

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
    button -> setIcon(QIcon(QStringLiteral(":/sources/add_%1").arg(name().toLower())));

    if (is_connected || offline_respondable) {
        if(!menu)
            menu = new QMenu(button);
        else
            menu -> clear();

        if (is_connected) {
            button -> setIcon(QIcon(QStringLiteral(":/sources/add_%1_on").arg(name().toLower())));

            menu -> addAction(QLatin1String("Disconnect"), this, SLOT(disconnectUser()));
            menu -> addSeparator();
            menu -> addAction(QLatin1String("Open your tab"), this, SLOT(openTab()));

            if (hasSimillarAudioByUser())
                menu -> addAction(QLatin1String("Open audio recommendations"), this, SLOT(openAudioRecs()));

            if (hasSimillarVideoByUser())
                menu -> addAction(QLatin1String("Open video recommendations"), this, SLOT(openVideoRecs()));
        }
        else if (is_connectable) {
            menu -> addAction(QLatin1String("Connect"), this, SLOT(openTab()));
            menu -> addSeparator();
        }

        if (is_sociable)
            menu -> addAction(QLatin1String("Open friend/group tab"), this, SLOT(openRelationTab()));

        if (is_packable)
            menu -> addAction(QLatin1String("Open package tab"), this, SLOT(openPackageTab()));

        button -> setToolTip(name());
        button -> setPopupMode(QToolButton::InstantPopup);
        button -> setMenu(menu);
    } else if (is_connectable) {
        button -> setToolTip(QStringLiteral("Connect to %1").arg(name()));
        connect(button, SIGNAL(clicked()), this, SLOT(openTab()));
    }

    return button;
}

QWidget * ISource::settingsBlock(int block_type, const QVariantMap & configs) {
    QWidget * block;
    QString block_title;

    switch(block_type) {
        case Models::mpf_stream_configurable: {
            block = streamSettingsBlock(configs[QString::number(block_type)]);
            block_title = QLatin1String("Stream Config");
        break;}

        case Models::mpf_feeds_configurable: {
            block = feedsSettingsBlock(configs[QString::number(block_type)]);
            block_title = QLatin1String("Feeds Config");
        break;}

        case Models::mpf_source_configurable: {
            block = sourceSettingsBlock(configs[QString::number(block_type)]);
            block_title = QLatin1String("Source Config");
        break;}

        default: block = 0;
    }

    if (block) {
        QGroupBox * group = new QGroupBox(block_title);
        QHBoxLayout * layout = new QHBoxLayout(group);
        layout -> addWidget(block);
        block = group;
    }

    return block;
}

void ISource::applySettings(QVariantMap & configs) {
    if (settings_forms.contains(sst_source))
        applySourceSettings(settings_forms.take(sst_source), configs[QString::number(Models::mpf_source_configurable)]);

    if (settings_forms.contains(sst_feeds))
        applyFeedsSettings(settings_forms.take(sst_feeds), configs[QString::number(Models::mpf_feeds_configurable)]);

    if (settings_forms.contains(sst_stream))
        applyStreamSettings(settings_forms.take(sst_stream), configs[QString::number(Models::mpf_stream_configurable)]);
}


// TODO: update open tab for using icmd as params str
void ISource::openTab() {
    if (connectUser()) {
        QString user_id = userID();

        Presentation::Dockbars::obj().createLinkedDocBar(
            Presentation::BarCreationNames(QString(name() % " [YOU]"), uidStr(user_id)),
            Models::Params(sourceType(), (Models::ParamFlags)(Models::mpf_auto_play_next | (Settings::obj().isUseIgnoreList() ? Models::mpf_use_ignore_list : Models::mpf_none) | Models::mpf_source_configurable), user_id, rec_obj_info), 0, true, true, 0, true
        );
    }
}

void ISource::openAudioRecs() {
    QString user_id = userID(sf_audio_recs_by_me);

    Presentation::Dockbars::obj().createDocBar(
        QStringLiteral("Audio Rec for YOU"),
        Models::Params(sourceType(), (Models::ParamFlags)(Models::mpf_auto_play_next | (Settings::obj().isUseIgnoreList() ? Models::mpf_use_ignore_list : Models::mpf_none) | Models::mpf_source_configurable), user_id, rec_user_audio), 0, true, true
    );
}

void ISource::openVideoRecs() {
    QString user_id = userID(sf_video_recs_by_me);

    Presentation::Dockbars::obj().createDocBar(
        QStringLiteral("Video Rec for YOU"),
        Models::Params(sourceType(), (Models::ParamFlags)(Models::mpf_auto_play_next | (Settings::obj().isUseIgnoreList() ? Models::mpf_use_ignore_list : Models::mpf_none) | Models::mpf_source_configurable), user_id, rec_user_video), 0, true, true
    );
}

void ISource::openRelationTab() {
    RelationsDialog dialog(this, Settings::obj().anchorWidget());
    connect(&dialog, SIGNAL(relationTabCreationRequired(QString,QString)), this, SLOT(createRelationTab(QString,QString)));
    dialog.exec();
}

void ISource::openPackageTab() {
    PackagesDialog dialog(this, Settings::obj().anchorWidget());
    if (dialog.exec() == QDialog::Accepted) {
        QString dialog_params = dialog.getParams();
        QString uid_postfix;
        QVariantMap params_data;
        Cmd cmnd = Cmd(dialog_params); // we should use source type from cmd, because it contains additional flags clarification

        bool source_configurable = true; //cmnd.attrs.hasQueryItem(CMD_SOURCE_CONFIGURABLE); // INFO: reserved
        bool stream_configurable = cmnd.attrs.hasQueryItem(CMD_STREAM_CONFIGURABLE);
        bool feeds_configurable = cmnd.attrs.hasQueryItem(CMD_FEEDS_CONFIGURABLE);

        Models::ParamFlags configs = (Models::ParamFlags)(
            (source_configurable ? Models::mpf_source_configurable : Models::mpf_none) |
            (feeds_configurable ? Models::mpf_feeds_configurable : Models::mpf_none) |
            (stream_configurable ? Models::mpf_stream_configurable : Models::mpf_none) |
            (Settings::obj().isUseIgnoreList() ? Models::mpf_use_ignore_list : Models::mpf_none)
        );

        if (feeds_configurable) {
            QString conf_flag = QString::number(Models::mpf_feeds_configurable);
            params_data.insert(conf_flag, feedsConfiguration(cmnd.attrs));
            uid_postfix = TITLE_PARAMS_SPLITTER % Cmd::variantToStr(params_data[conf_flag]);
        }
        if (stream_configurable) {
            QString conf_flag = QString::number(Models::mpf_stream_configurable);
            params_data.insert(conf_flag, streamConfiguration(cmnd.attrs));
            uid_postfix = TITLE_PARAMS_SPLITTER % Cmd::variantToStr(params_data[conf_flag]);
        }

        Presentation::Dockbars::obj().createLinkedDocBar(
            Presentation::BarCreationNames(
                name() % LSTR(" [") % dialog.getName() % ']'/* % uid_postfix*/,
                uidStr(dialog_params % uid_postfix)
            ),
            Models::Params(
                (DataSubType)cmnd.source_type,
                (Models::ParamFlags)(Models::mpf_auto_play_next | configs),
                dialog_params, rec_set, params_data
            ),
            0, true, true
        );
    }
}

void ISource::createRelationTab(const QString & rel_name, const QString & id) {
    Presentation::Dockbars::obj().createLinkedDocBar(
        Presentation::BarCreationNames(
            name() % LSTR(" [") % rel_name % ']',
            uidStr(id)
        ),
        Models::Params(sourceType(), (Models::ParamFlags)(Models::mpf_auto_play_next | (Settings::obj().isUseIgnoreList() ? Models::mpf_use_ignore_list : Models::mpf_none) | Models::mpf_source_configurable), id, rec_obj_info), 0, true, true
    );
}
