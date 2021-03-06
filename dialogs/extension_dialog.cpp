#include "extension_dialog.h"
#include "ui_extension_dialog.h"

using namespace Dialogs;
using namespace Core;

ExtensionDialog::ExtensionDialog(QWidget * parent) :
    BaseDialog(parent), ui(new Ui::ExtensionDialog) {
    ui -> setupUi(this);

    ui -> presetExtensions -> setModel(new QStringListModel(Extensions::obj().activeFilterList(), this));
    updatePresets();
    updatePresetsButtons(false);
}

ExtensionDialog::~ExtensionDialog() {
    delete ui;
}

void ExtensionDialog::updatePresets(bool clear) {
    ui -> presets -> blockSignals(true);
    if (clear)
        ui -> presets -> clear();
    ui -> presets -> addItems(Extensions::obj().presetsList());
    ui -> presets -> blockSignals(false);
    if (!Extensions::obj().activeFilterName().isNull())
        ui -> presets -> setCurrentIndex(ui -> presets -> findText(Extensions::obj().activeFilterName()));
}

void ExtensionDialog::updatePresetsButtons(bool show) {
    ui -> newPresetName -> setVisible(show);
    ui -> addPreset -> setVisible(show);
    ui -> cancelPreset -> setVisible(show);
}

void ExtensionDialog::on_presets_currentIndexChanged(const QString & name) {
    Extensions::obj().setActiveFilterName(name);
    ((QStringListModel *)ui -> presetExtensions -> model()) -> setStringList(Extensions::obj().activeFilterList());
}

void ExtensionDialog::on_addExtension_clicked() {
    QStringList list = ((QStringListModel *)ui -> presetExtensions -> model()) -> stringList();
    QString newPreset = ui -> extension -> text().replace(' ', "").toLower();
    if (newPreset.isEmpty()) return;

    if (newPreset.contains(',')) {
        QStringList newPresets = newPreset.split(',', QString::SkipEmptyParts);
        QStringList::Iterator it = newPresets.begin();

        for(; it != newPresets.end(); it++)
            proceedFilter((*it), list);
    }
    else proceedFilter(newPreset, list);

    ui -> extension -> setText("");
    Extensions::obj().filterListUpdate(ui -> presets -> currentText(), list);
    ((QStringListModel *)ui -> presetExtensions -> model()) -> setStringList(list);
}

void ExtensionDialog::proceedFilter(QString & filter, QStringList & preset) {
//    if (!filter.startsWith("*."))
//        filter = "*." + filter;

    if (!preset.contains(filter))
        preset.append(filter);
}

void ExtensionDialog::on_newPreset_clicked() {
    updatePresetsButtons(true);
}

void ExtensionDialog::on_removePreset_clicked() {
    if (ui -> presets -> count() > 0 && ui -> presets -> currentText() != QLatin1String("all")) {
        Extensions::obj().removePreset(ui -> presets -> currentText());
        updatePresets();
    }
}

void ExtensionDialog::on_addPreset_clicked() {
    Extensions::obj().addNewPreset(ui -> newPresetName -> text());
    ui -> presets -> clear();
    Extensions::obj().setActiveFilterName(ui -> newPresetName -> text());
    updatePresets();
    updatePresetsButtons(false);
    ui -> newPresetName -> clear();
}

void ExtensionDialog::on_cancelPreset_clicked() {
    updatePresetsButtons(false);
    ui -> newPresetName -> clear();
}
