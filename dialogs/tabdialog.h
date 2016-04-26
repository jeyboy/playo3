#ifndef TABDIALOG_H
#define TABDIALOG_H

#include "base_dialog.h"
#include "modules/models/model_settings.h"

namespace Ui { class TabDialog; }

namespace Dialogs {
    class TabDialog : public BaseDialog {
      Q_OBJECT
    public:
      explicit TabDialog(QWidget * parent = 0);
      ~TabDialog();

      QString getName();
      void setName(QString name);

      Models::Params getSettings();
      void setSettings(const Models::Params & hash);

    private:
      Ui::TabDialog * ui;
    };
}

#endif // TABDIALOG_H
