#ifndef TIME_LABEL
#define TIME_LABEL

#include "clickable_label.h"
#include "modules/core/misc/format.h"

namespace Controls {
    class TimeLabel : public ClickableLabel {
        Q_OBJECT

        int total;
        bool forward_turn, extended;
    public:
        TimeLabel(const QString & user_text, const QString & text, QWidget * parent = 0, Qt::WindowFlags f = 0, const QObject * receiver = 0, const char * slot = 0)
            : ClickableLabel(user_text, text, parent, f, receiver, slot), total(0), forward_turn(true), extended(false) {
            connect(this, SIGNAL(clicked()), this, SLOT(invertTurn()));
            setTotal(0);
        }
    public slots:
        void invertTurn() { forward_turn = !forward_turn; }
        void setPos(int pos) { output(pos); }
        void setTotal(int newTotal) {
            extended = Duration::hasHours((total = newTotal));
            int newWidth = qMax(0, fontMetrics().width(QString::number(total)) - 10);
            setMinimumWidth(newWidth);
            parentWidget() -> setMinimumWidth(qMax(parentWidget() -> minimumWidth(), newWidth));
            output(0);
        }
    protected:
        void output(int pos) {
            QString val, totalStr;
            totalStr = Duration::fromMillis(total, extended);

            if (forward_turn) {
                val = Duration::fromMillis(pos, extended);
                setText(val % QLatin1String("\n") % totalStr);
            } else {
                val = Duration::fromMillis(total - pos, extended);
                setText(totalStr % QLatin1String("\n") % val);
            }
        }
    };
}

#endif // TIME_LABEL
