#include "tabdialog.h"
#include "ui_tabdialog.h"

using namespace Dialogs;

TabDialog::TabDialog(QWidget * parent) :
  BaseDialog(parent), ui(new Ui::TabDialog) {
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

Models::Params TabDialog::getSettings() {
    Models::Params settings;

    settings.deleteFile = ui -> deleteFile -> isChecked();
    settings.interactive = ui -> interactive -> isChecked(); //checkState() == Qt::Checked
    settings.playlist = ui -> playlist -> isChecked();

    if (ui -> isListRadio -> isChecked())
        settings.type = Core::dt_level;
    else if (ui -> isOneLevelTreeRadio -> isChecked())
        settings.type = Core::dt_level_tree;
    else if (ui -> isTreeRadio -> isChecked())
        settings.type = Core::dt_tree;
    else if (ui -> isVkRadio -> isChecked())
        settings.type = Core::dt_web_vk;
    else if (ui -> isSoundcloudRadio -> isChecked())
        settings.type = Core::dt_web_sc;

    return settings;
}

void TabDialog::setSettings(const Models::Params & settings) {
  ui -> deleteFile -> setChecked(settings.deleteFile);
  ui -> interactive -> setChecked(settings.interactive);
  ui -> playlist -> setChecked(settings.playlist);

  ui -> isListRadio -> setChecked(settings.type == Core::dt_level);
  ui -> isOneLevelTreeRadio -> setChecked(settings.type == Core::dt_level_tree);
  ui -> isTreeRadio -> setChecked(settings.type == Core::dt_tree);
  ui -> isVkRadio -> setChecked(settings.type == Core::dt_web_vk);
  ui -> isSoundcloudRadio -> setChecked(settings.type == Core::dt_web_sc);

  ui -> isListRadio -> setEnabled(false);
  ui -> isTreeRadio -> setEnabled(false);
  ui -> isOneLevelTreeRadio -> setEnabled(false);
  ui -> isVkRadio -> setEnabled(false);
  ui -> isSoundcloudRadio -> setEnabled(false);
}
