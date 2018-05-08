#include "search_bar.h"

using namespace Controls;

SearchBar::SearchBar(const QObject * receiver, const char * search_start_slot, const char * search_end_slot, const char * search_end_signal,
                     const QObject * closeReceiver, const char * close_slot, QWidget * parent) : QWidget(parent)
{
    setContentsMargins(2, 0, 12, 0);

    QHBoxLayout * l = new QHBoxLayout(this);
    setLayout(l);

    text = new QLineEdit(this);
    connect(text, SIGNAL(textChanged(QString)), receiver, search_start_slot);
    l -> addWidget(text, 7);

    connect(receiver, search_end_signal, this, SLOT(searchEnded()));

//    ClickableLabel * searchButton = new ClickableLabel("Search", QPixmap(":search").scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation), this, 0, this, SLOT(initiateSearch()));
//    connect(this, SIGNAL(searchCalled(QString)), receiver, search_start_slot);
//    l -> addWidget(searchButton, 1);

    ClickableLabel * closeButton = new ClickableLabel(QLatin1String("Close search"), QPixmap(QLatin1String(":remove")).scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation), this, 0, this, SLOT(onHide()));
    connect(closeButton, SIGNAL(clicked()), closeReceiver, close_slot);
    connect(closeButton, SIGNAL(clicked()), receiver, search_end_slot);
    l -> addWidget(closeButton, 1);
}

void SearchBar::searchEnded() {
    text -> setDisabled(false);
}

void SearchBar::onHide() {
    text -> setText("");
    hide();
}
