#include "search_bar.h"

using namespace Playo3;

SearchBar::SearchBar(const QObject * receiver, const char * search_start_slot, const char * search_end_signal,
                     const QObject * closeReceiver, const char * close_slot, QWidget * parent) : QWidget(parent)
{
    setContentsMargins(2, 0, 8, 0);

    QHBoxLayout * l = new QHBoxLayout(this);
    setLayout(l);

    text = new QLineEdit(this);
    l -> addWidget(text, 4);

    connect(receiver, search_end_signal, this, SLOT(searchEnded()));

    ClickableLabel * searchButton = new ClickableLabel("Search", "Search", this, 0, this, SLOT(initiateSearch()));
    connect(this, SIGNAL(searchCalled(QString)), receiver, search_start_slot);
    l -> addWidget(searchButton, 1);

    ClickableLabel * closeButton = new ClickableLabel("Close", "Close", this, 0, this, SLOT(hide()));
    connect(closeButton, SIGNAL(clicked()), closeReceiver, close_slot);
    l -> addWidget(closeButton, 1);
}

void SearchBar::initiateSearch() {
    emit searchCalled(text -> text());
    text -> setEnabled(false);
}

void SearchBar::searchEnded() {
    text -> setEnabled(true);
}
