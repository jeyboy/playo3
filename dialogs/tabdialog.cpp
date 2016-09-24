#include "tabdialog.h"
#include "ui_tabdialog.h"
#include "modules/core/web/web_apis.h"

using namespace Dialogs;

TabDialog::TabDialog(QWidget * parent) :
    BaseDialog(parent), ui(new Ui::TabDialog) {
    ui -> setupUi(this);

    setWindowTitle(QStringLiteral("View settings"));
    ui -> interactive -> setEnabled(false);

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
    ui -> tabName -> setText(name);
}

Models::Params TabDialog::getSettings() {
    Core::DataSubType sel_data_type = (Core::DataSubType)ui -> tabType -> currentData().toInt();

    int new_flags =
        (ui -> deleteFile -> isChecked() ? Models::mpf_del_file : Models::mpf_none) |
        (ui -> interactive -> isChecked() ? Models::mpf_interactive : Models::mpf_none) |
        (ui -> autoPlayNext -> isChecked() ? Models::mpf_auto_play_next : Models::mpf_none) |
        (!settings.isCommon() && sel_data_type < Core::dt_search ? Models::mpf_configurable : Models::mpf_none);

    settings.data_type = sel_data_type;
    settings.setFlags(new_flags);

    return settings;
}

void TabDialog::setSettings(const Models::Params & params) {
    settings = params;

    ui -> deleteFile -> setChecked(settings.isDeleteFile());
    ui -> interactive -> setChecked(settings.isInteractive());
    ui -> autoPlayNext -> setChecked(settings.isAutoPlayNext());

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

}
