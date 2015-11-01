#ifndef TIME_LABEL
#define TIME_LABEL

#include "clickable_label.h"

#include "modules/core/misc/format.h"

namespace Controls {
    class TimeLabel : public ClickableLabel {
        Q_OBJECT

        uint total;
        bool forward_turn, extended;
    public:
        TimeLabel(const QString & user_text, const QString & text, QWidget * parent = 0, Qt::WindowFlags f = 0, const QObject * receiver = 0, const char * slot = 0)
            : ClickableLabel(user_text, text, parent, f, receiver, slot), total(0), forward_turn(true), extended(false) {
            connect(this, SIGNAL(clicked()), this, SLOT(invertTurn()));
            setTotal(0);
        }
    public slots:
        void invertTurn() { forward_turn = !forward_turn; }
        void setPos(uint pos) {
            output(pos);
        }
        void setTotal(uint newTotal) {
            extended = Duration::hasHours((total = newTotal));
            output(0);
        }
    protected:
        void output(uint pos) {
            QString val, totalStr;
            totalStr = Duration::fromMillis(total, extended);

            if (forward_turn) {
                val = Duration::fromMillis(pos, extended);
                setText(val % QStringLiteral("\n") % totalStr);
            } else {
                val = Duration::fromMillis(total - pos, extended);
                setText(totalStr % QStringLiteral("\n") % val);
            }
        }
    };
}

#endif // TIME_LABEL
