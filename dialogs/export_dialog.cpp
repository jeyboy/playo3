#include "export_dialog.h"
#include "ui_export_dialog.h"

ExportDialog::ExportDialog(IModel * parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog), mdl(parent)
{
    ui -> setupUi(this);
}

ExportDialog::~ExportDialog() {
    delete ui;
}

void ExportDialog::on_buttonBox_accepted() {
    QHash<QString, QStringList> uidsMap;
    QStringList ids = ui -> uids -> toPlainText().split(",");

    for(QStringList::Iterator it = ids.begin(); it != ids.end(); it++) {
        uidsMap[(*it).mid(0, 2)].append(*it);
    }

    FolderItem * parent = mdl -> rootForRemote();

    for(QHash<QString, QStringList>::Iterator map_it = uidsMap.begin(); map_it != uidsMap.end(); map_it++) {
        if (map_it.key() == "vk") {

        }
    }
}
