#include "tabdialog.h"
#include "ui_tabdialog.h"

using namespace Playo3;

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

ViewSettings TabDialog::getSettings() {
    ViewSettings settings;

    settings.deleteFile = ui -> deleteFile -> isChecked();
    settings.interactive = ui -> interactive -> isChecked(); //checkState() == Qt::Checked
    settings.playlist = ui -> playlist -> isChecked();

    if (ui -> isListRadio -> isChecked())
        settings.type = Playo3::list;
    else if (ui -> isOneLevelTreeRadio -> isChecked())
        settings.type = Playo3::level_tree;
    else if (ui -> isTreeRadio -> isChecked())
        settings.type = Playo3::tree;
    else if (ui -> isVkRadio -> isChecked())
        settings.type = Playo3::vk;
    else if (ui -> isSoundcloudRadio -> isChecked())
        settings.type = Playo3::soundcloud;

    return settings;
}

void TabDialog::setSettings(ViewSettings settings) {
  ui -> deleteFile -> setChecked(settings.deleteFile);
  ui -> interactive -> setChecked(settings.interactive);
  ui -> playlist -> setChecked(settings.playlist);

  ui -> isListRadio -> setChecked(settings.type == Playo3::list);
  ui -> isOneLevelTreeRadio -> setChecked(settings.type == Playo3::level_tree);
  ui -> isTreeRadio -> setChecked(settings.type == Playo3::tree);
  ui -> isVkRadio -> setChecked(settings.type == Playo3::vk);
  ui -> isSoundcloudRadio -> setChecked(settings.type == Playo3::soundcloud);

  ui -> isListRadio -> setEnabled(false);
  ui -> isTreeRadio -> setEnabled(false);
  ui -> isOneLevelTreeRadio -> setEnabled(false);
  ui -> isVkRadio -> setEnabled(false);
  ui -> isSoundcloudRadio -> setEnabled(false);
}
