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
        void setName(const QString & name);

        Models::Params getSettings();
        void setSettings(const Models::Params & hash);
    protected slots:
        void accepted();

    private:
        Models::Params settings;
        Ui::TabDialog * ui;
    };
}

#endif // TABDIALOG_H
