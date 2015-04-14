#ifndef TITLE_LAYOUT
#define TITLE_LAYOUT

#include <QGridLayout>
#include <qdebug.h>

class TitleLayout : public QGridLayout {
public :
    inline explicit TitleLayout(QWidget * parent) : QGridLayout(parent), isHorizontal(true) {
        setSizeConstraint(QLayout::SetNoConstraint);
        setContentsMargins(0, 0, 0, 0);
    }

    inline QSize minimumSize() const {
        return QSize(54, QGridLayout::minimumSize().height()); // this is fix for dock widget correct dragging
//        return QGridLayout::minimumSize();
    }

    inline void setVertical(bool vertical) {
        if (vertical && isHorizontal) {
            for (int loop1 = 0; loop1 < columnCount(); loop1++) {
                QLayoutItem * it = itemAtPosition(0, loop1);
                if (it) {
                    int row = columnCount() - loop1 - 1;
                    QWidget * w = it -> widget();
                    removeItem(it);
                    addWidget(w, row, 0, Qt::AlignRight | (loop1 == columnCount() - 1 ? Qt::AlignTop : Qt::AlignBottom));
//                    setRowStretch(row, columnStretch(loop1));
                }
            }
        } else if (!vertical && !isHorizontal) {
            for (int loop1 = 0; loop1 < rowCount(); loop1++) {
                QLayoutItem * it = itemAtPosition(loop1, 0);
                if (it) {
                    int column = rowCount() - loop1 - 1;
                    QWidget * w = it -> widget();
                    removeItem(it);
                    addWidget(w, 0, column, Qt::AlignBottom | Qt::AlignLeft);
//                    setColumnStretch(rowCount() - loop1, rowStretch(loop1));
                }
            }
        }

        isHorizontal = !vertical;
    }

private:
    bool isHorizontal;
};

#endif // TITLE_LAYOUT

