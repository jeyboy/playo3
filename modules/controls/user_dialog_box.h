#ifndef USER_DIALOG_BOX
#define USER_DIALOG_BOX

#include <qmessagebox.h>

#include "modules/core/misc/thread_utils.h"
#include "modules/core/interfaces/singleton.h"

namespace Controls {
    class UserDialogBox : public QObject, public Core::Singleton<UserDialogBox> {
        Q_OBJECT
    public:
        inline int lastAnswer() const { return last_answer; }

        void alert(QWidget * parent, const QString & title, const QString & text, QMessageBox::StandardButtons buttons) {
            if (Core::ThreadUtils::livesInCurrThread(this)) {
                QMetaObject::invokeMethod(
                    this,
                    "_alert",
                    Qt::BlockingQueuedConnection,
                    Q_ARG(QWidget *, parent),
                    Q_ARG(QString, title),
                    Q_ARG(QString, text),
                    Q_ARG(QMessageBox::StandardButtons, buttons)
                );
            } else _alert(parent, title, text, buttons);
        }
    private slots:
        inline void _alert(QWidget * parent, const QString & title, const QString & text, QMessageBox::StandardButtons buttons) {
            last_answer = QMessageBox::warning(parent, title, text, buttons);
        }
    private:
        friend class Core::Singleton<UserDialogBox>;
        UserDialogBox() {}

        int last_answer;
    };
}

#endif // USER_DIALOG_BOX

