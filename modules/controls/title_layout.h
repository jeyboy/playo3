#ifndef TITLE_LAYOUT
#define TITLE_LAYOUT

#include <QGridLayout>

class TitleLayout : public QGridLayout {
public :
    inline explicit TitleLayout(QWidget * parent) : QGridLayout(parent) {
        setSizeConstraint(QLayout::SetNoConstraint);
        setContentsMargins(0, 0, 0, 0);
    }

    inline QSize minimumSize() const {
        return QSize(1, QGridLayout::minimumSize().height()); // this is fix for dock widget correct dragging
//        return QGridLayout::minimumSize();
    }
};

#endif // TITLE_LAYOUT

