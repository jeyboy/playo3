#ifndef USER_DIALOG_BOX
#define USER_DIALOG_BOX

#include <qmessagebox.h>
#include <qfiledialog.h>

namespace Playo3 {
    class UserDialogBox : public QWidget {
        Q_OBJECT
    public:
        inline int lastAnswer() const { return last_answer; }
        static UserDialogBox * instance(QWidget * parent = 0);
    public slots:
        void alert(const QString & title, const QString & text, QMessageBox::StandardButtons buttons);
        void files();
    private:
        inline UserDialogBox(QWidget * parent) : QWidget(parent) {}

        int last_answer;
        QList<QUrl> selectedFiles;
        static UserDialogBox * self;
    };
}

#endif // USER_DIALOG_BOX

