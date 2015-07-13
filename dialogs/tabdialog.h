#ifndef TABDIALOG_H
#define TABDIALOG_H

#include <qdialog.h>
#include "modules/data_struct/view_settings.h"

namespace Ui { class TabDialog; }

class TabDialog : public QDialog {
  Q_OBJECT
public:
  explicit TabDialog(QWidget * parent = 0);
  ~TabDialog();

  QString getName();
  void setName(QString name);

  Playo3::ViewSettings getSettings();
  void setSettings(Playo3::ViewSettings hash);

private:
  Ui::TabDialog * ui;
};

#endif // TABDIALOG_H
