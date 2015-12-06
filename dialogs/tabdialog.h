#ifndef TABDIALOG_H
#define TABDIALOG_H

#include "base_dialog.h"
#include "modules/views/view_settings.h"

namespace Ui { class TabDialog; }

namespace Dialogs {
    class TabDialog : public BaseDialog {
      Q_OBJECT
    public:
      explicit TabDialog(QWidget * parent = 0);
      ~TabDialog();

      QString getName();
      void setName(QString name);

      Views::Params getSettings();
      void setSettings(Views::Params hash);

    private:
      Ui::TabDialog * ui;
    };
}

#endif // TABDIALOG_H
