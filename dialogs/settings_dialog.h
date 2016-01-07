#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <qfiledialog.h>
#include <qcolordialog.h>

#include "extension_dialog.h"

#include "settings.h"
#include "modules/models/service/hotkey_manager.h"
#include "modules/core/misc/autorun.h"

#include "modules/models/service/hotkey_model.h"
#include "modules/models/service/hotkey_model_item.h"
#include "modules/models/service/hotkey_delegate.h"

namespace Ui { class SettingsDialog; }

namespace Dialogs {
    class SettingsDialog : public BaseDialog {
        Q_OBJECT

        bool execColorDialog(QColor & color);
        void instantiation();
        void instantiateLayout();

        Ui::SettingsDialog * ui;

        QWidget * lastTab;
        ExtensionDialog * extDialog;

        bool iconSizeChanged;
        QColor defaultColor, listenedColor, likedColor, playedColor, folderColor, itemColor, spectrumColor, spectrumColor2, spectrumColor3;
        QColor itemTextColor, selectedItemTextColor, itemInfoTextColor, selectedItemInfoTextColor;
    protected:
        void setBackgroundColor(QWidget * widget, const QColor & color) {
            widget -> setStyleSheet(QStringLiteral("background-color: ") % color.name() % QStringLiteral(";"));
        }

        void initGlobalSettings();
        void initItemsSettings();
        void initViewSettings();
        void initHotkeysSettings();
        void initSpectrumSettings();
        void initLibrarySettings();
        void initExtensions();

        void saveGlobalSettings();
        void saveItemsSettings();
        void saveViewSettings();
        void saveHotkeysSettings();
        void saveSpectrumSettings();
        void saveLibrarySettings();
    public:
        explicit SettingsDialog(QWidget * parent);
        ~SettingsDialog();

        static void registerHotkeys(QWidget * receiver);

        bool isIconSizeChanged() const;

    private slots:
        void on_cancelButton_clicked();
        void on_resetButton_clicked();
        void on_acceptButton_clicked();
        void on_browseButton_clicked();
        void on_defaultColorButton_clicked();
        void on_listenedColorButton_clicked();
        void on_likedColorButton_clicked();
        void on_playedColorButton_clicked();
        void on_folderColorButton_clicked();
        void on_defaultItemTextColorButton_clicked();
        void on_selectedItemTextColorButton_clicked();
        void on_defaultItemInfoTextColorButton_clicked();
        void on_selectedItemInfoTextColorButton_clicked();
        void on_spectrumColor_clicked();
        void on_spectrumColor2_clicked();
        void on_spectrumColor3_clicked();
        void on_openDropPointInTab_toggled(bool checked);
        void on_drawMetrics_clicked(bool checked);
        void on_itemColorButton_clicked();
        void on_autorunned_toggled(bool checked);
        void on_colorScheme_activated(int index);

        void tabClicked(int);
//        void on_commonBtn_clicked(bool checked);
//        void on_viewsBtn_clicked(bool checked);
//        void on_itemsBtn_clicked(bool checked);
//        void on_hotkeysBtn_clicked(bool checked);
//        void on_spectrumBtn_clicked(bool checked);
//        void on_extensionsBtn_clicked(bool checked);
    };
}

#endif // SETTINGS_DIALOG_H
