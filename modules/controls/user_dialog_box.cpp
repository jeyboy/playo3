#include "user_dialog_box.h"

using namespace Playo3;

UserDialogBox * UserDialogBox::self = 0;

UserDialogBox * UserDialogBox::instance(QWidget * parent) {
    if(!self)
        self = new UserDialogBox(parent);
    return self;
}

void UserDialogBox::alert(const QString & title, const QString & text, QMessageBox::StandardButtons buttons) {
    last_answer = QMessageBox::warning(parentWidget(), title, text, buttons);
}

void UserDialogBox::files() { //TODO: test needed - not used
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
//    dialog.setOption(QFileDialog::ShowDirsOnly);
    if (dialog.exec())
        selectedFiles = dialog.selectedUrls();
    else
        selectedFiles.clear();
}
