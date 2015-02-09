#include "list_view.h"

using namespace Playo3;

ListView::ListView(QWidget * parent, ViewSettings settings, QJsonObject * hash)
    : ViewInterface(dynamic_cast<ModelInterface *>(new ListModel(hash)), parent, settings) {
    setIndentation(-2);
}

ListView::~ListView() {

}
