#include "tabdialog.h"
#include "ui_tabdialog.h"

using namespace Dialogs;

TabDialog::TabDialog(QWidget * parent) :
  QDialog(parent), ui(new Ui::TabDialog) {
  ui -> setupUi(this);

  setWindowTitle(QStringLiteral("View settings"));
  ui -> interactive -> setEnabled(false);
  ui -> isVkRadio -> setEnabled(false);
  ui -> isSoundcloudRadio -> setEnabled(false);
}

TabDialog::~TabDialog() {
  delete ui;
}

QString TabDialog::getName() {
  return ui -> tabName -> text();
}

void TabDialog::setName(QString name) {
  ui -> tabName -> setText(name);
}

View::Params TabDialog::getSettings() {
    View::Params settings;

    settings.deleteFile = ui -> deleteFile -> isChecked();
    settings.interactive = ui -> interactive -> isChecked(); //checkState() == Qt::Checked
    settings.playlist = ui -> playlist -> isChecked();

    if (ui -> isListRadio -> isChecked())
        settings.type = Data::list;
    else if (ui -> isOneLevelTreeRadio -> isChecked())
        settings.type = Data::level_tree;
    else if (ui -> isTreeRadio -> isChecked())
        settings.type = Data::tree;
    else if (ui -> isVkRadio -> isChecked())
        settings.type = Data::vk;
    else if (ui -> isSoundcloudRadio -> isChecked())
        settings.type = Data::soundcloud;

    return settings;
}

void TabDialog::setSettings(View::Params settings) {
  ui -> deleteFile -> setChecked(settings.deleteFile);
  ui -> interactive -> setChecked(settings.interactive);
  ui -> playlist -> setChecked(settings.playlist);

  ui -> isListRadio -> setChecked(settings.type == Data::list);
  ui -> isOneLevelTreeRadio -> setChecked(settings.type == Data::level_tree);
  ui -> isTreeRadio -> setChecked(settings.type == Data::tree);
  ui -> isVkRadio -> setChecked(settings.type == Data::vk);
  ui -> isSoundcloudRadio -> setChecked(settings.type == Data::soundcloud);

  ui -> isListRadio -> setEnabled(false);
  ui -> isTreeRadio -> setEnabled(false);
  ui -> isOneLevelTreeRadio -> setEnabled(false);
  ui -> isVkRadio -> setEnabled(false);
  ui -> isSoundcloudRadio -> setEnabled(false);
}
