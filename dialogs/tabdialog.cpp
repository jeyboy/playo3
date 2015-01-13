#include "tabdialog.h"
#include "ui_tabdialog.h"

using namespace Playo3;

//Playo3::TabSettings TabDialog::VKSettings(int uid) {
//    CBHash res;
//    res.insert("d", 0);
//    res.insert("i", 0);
//    res.insert("p", 1);
//    res.insert("t", VIEW_VK);
//    res.insert("uid", uid);

//    return res;
//}

//Playo3::TabSettings TabDialog::soundcloudSettings(int uid) {
//    CBHash res;
//    res.insert("d", 0);
//    res.insert("i", 0);
//    res.insert("p", 1);
//    res.insert("t", VIEW_SOUNDCLOUD);
//    res.insert("uid", uid);

//    return res;
//}

TabDialog::TabDialog(QWidget * parent) :
  QDialog(parent), ui(new Ui::TabDialog) {
  ui -> setupUi(this);

  setWindowTitle("Tab settings");
  ui -> interactive -> setEnabled(false);
  ui -> isVkRadio -> setEnabled(false);
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

    settings.deleteFile = ui -> deleteFile -> checkState() == Qt::Checked;
    settings.interactive = ui -> interactive -> checkState() == Qt::Checked;
    settings.playlist = ui -> playlist -> checkState() == Qt::Checked;

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
}
