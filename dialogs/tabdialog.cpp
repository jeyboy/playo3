#include "tabdialog.h"
#include "ui_tabdialog.h"
#include "qdialogbuttonbox.h"

#include "modules/core/web/web_apis.h"

using namespace Dialogs;

TabDialog::TabDialog(QWidget * parent) :
    BaseDialog(parent), ui(new Ui::TabDialog) {
    ui -> setupUi(this);

    connect(this, SIGNAL(accepted()), this, SLOT(accepted()));

    setWindowTitle(LSTR("View settings"));
    ui -> interactive -> setEnabled(false);
    ui -> deleteFile -> setProperty("danger", true);

    QMap<Core::DataSubType, QString> data_list = {
        {Core::dt_tree, LSTR("Grouped list with subgroups")},
        {Core::dt_level_tree, LSTR("Grouped list")},
        {Core::dt_level, LSTR("List")}
    };
    for(QMap<Core::DataSubType, QString>::Iterator source = data_list.begin(); source != data_list.end(); source++) {
        ui -> tabType -> addItem(source.value(), source.key());
    }
}

TabDialog::~TabDialog() {
    delete ui;
}

QString TabDialog::getName() {
    return ui -> tabName -> text();
}

void TabDialog::setName(const QString & name) {    
    ui -> tabName -> setText(name/*.section(TITLE_PARAMS_SPLITTER, 0, 0)*/);
    ui -> tabName -> setToolTip(name);
}

Models::Params TabDialog::getSettings() { return settings; }
void TabDialog::setSettings(const Models::Params & params) {
    settings = params;

    ui -> deleteFile -> setChecked(settings.isDeleteFile());
    ui -> interactive -> setChecked(settings.isInteractive());
    ui -> autoPlayNext -> setChecked(settings.isAutoPlayNext());
    ui -> ignoreList -> setChecked(settings.isUseIgnoreList());

    ui -> ignoreList -> setEnabled(settings.isSourceConfigurable());

    int index = 0;
    Core::DataSubType data_type = DST_EXTRACT_FLAGS(settings.data_type);

    for(int i = 0; i < ui -> tabType -> count(); i++)
        if (ui -> tabType -> itemData(i).toInt() == data_type) {
            index = i;
            break;
        }

    QMap<Core::DataSubType, Core::ISource *> list = Core::Web::Apis::sourcesList();
    for(QMap<Core::DataSubType, Core::ISource *>::Iterator source = list.begin(); source != list.end(); source++) {
        ui -> tabType -> addItem(source.value() -> name(), source.key());
        if (source.key() == data_type)
            index = ui -> tabType -> count() - 1;
    }

    ui -> tabType -> setEnabled(false);
    ui -> tabType -> setCurrentIndex(index);

    if (!settings.isTabConfigurable()) {
        ui -> groupBox -> setEnabled(false);
        ui -> groupBox_2 -> setEnabled(false);
        ui -> groupBox_3 -> setEnabled(false);
    }

    Core::ISource * source = Core::Web::Apis::source(data_type);

    if (source) {
        if (settings.isSourceConfigurable()) {
            QWidget * settings_block = source -> settingsBlock(Models::mpf_source_configurable, params.configs);
            if (settings_block)
                ui -> verticalLayout -> addWidget(settings_block);
        }

        if (settings.isFeedsConfigurable()) {
            QWidget * settings_block = source -> settingsBlock(Models::mpf_feeds_configurable, params.configs);
            if (settings_block)
                ui -> verticalLayout -> addWidget(settings_block);
        }

        if (settings.isStreamConfigurable()) {
            QWidget * settings_block = source -> settingsBlock(Models::mpf_stream_configurable, params.configs);
            if (settings_block)
                ui -> verticalLayout -> addWidget(settings_block);
        }
    }
}

void TabDialog::accepted() {
    Core::DataSubType sel_data_type = (Core::DataSubType)ui -> tabType -> currentData().toInt();

    int new_flags =
        (ui -> deleteFile -> isChecked() ? Models::mpf_del_file : Models::mpf_none) |
        (ui -> interactive -> isChecked() ? Models::mpf_interactive : Models::mpf_none) |
        (ui -> autoPlayNext -> isChecked() ? Models::mpf_auto_play_next : Models::mpf_none) |
        (!settings.isCommon() && sel_data_type < Core::dt_search ? Models::mpf_tab_configurable : Models::mpf_none);

    settings.data_type = sel_data_type;
    settings.setFlags(new_flags);

    Core::ISource * source = Core::Web::Apis::source(settings.data_type);
    if (source)
        source -> applySettings(settings.configs);
}

int TabDialog::exec() {
    QDialogButtonBox * button_box = new QDialogButtonBox(
        QDialogButtonBox::Cancel | QDialogButtonBox::Ok,
        Qt::Horizontal,
        this
    );
    button_box -> setCenterButtons(true);
    ui -> verticalLayout -> addWidget(button_box);
    connect(button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    return BaseDialog::exec();
}
