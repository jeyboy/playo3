#ifndef TABDIALOG_H
#define TABDIALOG_H

#include <QDialog>
#include "misc/tab_settings.h"

namespace Ui {
  class TabDialog;
}

class TabDialog : public QDialog {
  Q_OBJECT
public:
//  static Playo3::TabSettings VKSettings(int uid = 0);
//  static Playo3::TabSettings soundcloudSettings(int uid = 0);

  explicit TabDialog(QWidget *parent = 0);
  ~TabDialog();

  QString getName();
  void setName(QString name);

  Playo3::TabSettings getSettings();
  void setSettings(Playo3::TabSettings hash);

private:
  Ui::TabDialog *ui;
};

#endif // TABDIALOG_H
