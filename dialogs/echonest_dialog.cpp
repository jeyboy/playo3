#include "echonest_dialog.h"
#include "ui_echonest_dialog.h"

EchonestDialog::EchonestDialog(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::EchonestDialog) {
    ui -> setupUi(this);
}

EchonestDialog::~EchonestDialog() {
    delete ui;
}

void EchonestDialog::on_findArtistSongs_clicked() {
    if (!ui -> findArtistName -> text().isEmpty()) {
        QJsonObject info = EchonestApi::instance() -> artistProfile(ui -> findArtistName -> text());

        if (!songs.isEmpty()) {

        }
    }
}
