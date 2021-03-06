#ifndef EXTENSION_DIALOG_H
#define EXTENSION_DIALOG_H

#include <qstringlistmodel.h>

#include "base_dialog.h"
#include "modules/core/misc/file_utils/extensions.h"

namespace Ui { class ExtensionDialog; }

namespace Dialogs {
    class ExtensionDialog : public BaseDialog {
        Q_OBJECT

    public:
        explicit ExtensionDialog(QWidget * parent = 0);
        ~ExtensionDialog();

    protected:
        void updatePresets(bool clear = true);
        void updatePresetsButtons(bool show = true);

    private slots:
        void on_addPreset_clicked();

        void on_presets_currentIndexChanged(const QString & arg1);

        void on_addExtension_clicked();

        void on_newPreset_clicked();

        void on_cancelPreset_clicked();

        void on_removePreset_clicked();

    private:
        void proceedFilter(QString & filter, QStringList & preset);

        Ui::ExtensionDialog * ui;
    };
}

#endif // EXTENSION_DIALOG_H
