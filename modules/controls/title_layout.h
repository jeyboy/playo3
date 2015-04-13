#ifndef TITLE_LAYOUT
#define TITLE_LAYOUT

#include <QGridLayout>

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

private:
    bool isHorizontal;
};

#endif // TITLE_LAYOUT

