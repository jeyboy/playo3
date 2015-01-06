#ifndef WINDOW_TITLE
#define WINDOW_TITLE

#include <QGridLayout>
#include <QWidget>
#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <qevent.h>

class WindowTitle : public QWidget {
    Q_OBJECT
public:
    inline WindowTitle(QWidget * window, int height, QMargins margins, int sidePadding) : QWidget(window) {
        setObjectName("WindowTitle");
        setContentsMargins(margins);
        setStyleSheet("#WindowTitle { border-bottom: 2px solid white; margin: 0 " + QString::number(sidePadding) + "px 0 " + QString::number(sidePadding) + "px; }");
        setFixedHeight(height);
        QGridLayout * l = new QGridLayout(this);
        l -> setContentsMargins(0, 0, 0, 0);

        titleLabel = new QLabel("", this);
        QFont font = titleLabel -> font();
        font.setPointSize(12);
        font.setBold(true);
        titleLabel -> setFont(font);

        l -> addWidget(titleLabel, 0, 0, Qt::AlignLeft);
    }

    inline void setText(const QString & text) { titleLabel -> setText(text); }

signals:
    void doubleClicked();
protected:
    inline void mouseDoubleClickEvent(QMouseEvent *) { emit doubleClicked(); }
    inline void paintEvent(QPaintEvent *) {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style() -> drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

private:
    QLabel * titleLabel;
};

#endif // WINDOW_TITLE

