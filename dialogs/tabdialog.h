#ifndef TABDIALOG_H
#define TABDIALOG_H

#include <qdialog.h>
#include "modules/views/view_settings.h"

namespace Ui { class TabDialog; }

namespace Dialogs {
    class TabDialog : public QDialog {
      Q_OBJECT
    public:
      explicit TabDialog(QWidget * parent = 0);
      ~TabDialog();

      QString getName();
      void setName(QString name);

      View::Params getSettings();
      void setSettings(View::Params hash);

    private:
      Ui::TabDialog * ui;
    };
}

#endif // TABDIALOG_H
