#ifndef TOOLBARBUTTONDIALOG_H
#define TOOLBARBUTTONDIALOG_H

#include <qfiledialog.h>

namespace Ui { class ToolbarButtonDialog; }

class ToolbarButtonDialog : public QDialog {
  Q_OBJECT
public:
  explicit ToolbarButtonDialog(QWidget * parent = 0);
  ~ToolbarButtonDialog();

  QString getName();
  void setName(QString name);

  QString getPath();
  void setPath(QString name);

private slots:
  void on_browseButton_clicked();

private:
  Ui::ToolbarButtonDialog * ui;
};

#endif // TOOLBARBUTTONDIALOG_H
