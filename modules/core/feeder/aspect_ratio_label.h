#ifndef ASPECT_RATIO_LABEL
#define ASPECT_RATIO_LABEL

#include <QLabel>

class AspectRatioLabel : public QLabel {
    Q_OBJECT
public:
    explicit AspectRatioLabel(QWidget * parent = 0);

    QSize sizeHint() const;
public slots:
    void setPixmap(const QPixmap & p);
protected:
    void paintEvent(QPaintEvent *);
};


#endif // ASPECT_RATIO_LABEL
