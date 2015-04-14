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
                    int row = columnCount() - loop1;
                    addWidget(it -> widget(), row, 0, it -> alignment());
                    setRowStretch(row, columnStretch(loop1));
                    removeItem(it);
                }
            }
        } else if (!vertical && !isHorizontal) {
            for (int loop1 = 0; loop1 < rowCount(); loop1++) {
                QLayoutItem * it = itemAtPosition(loop1, 0);
                if (it) {
                    addWidget(it -> widget(), 0, rowCount() - loop1, it -> alignment());
                    setColumnStretch(rowCount() - loop1, rowStretch(loop1));
                    removeItem(it);
                }
            }
        }
    }

private:
    bool isHorizontal;
};

#endif // TITLE_LAYOUT

