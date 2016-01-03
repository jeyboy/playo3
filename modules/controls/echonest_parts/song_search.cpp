#include "song_search.h"

using namespace Controls::Echonest;

SongSearch::SongSearch(const QStringList & stylesList, const QStringList & moodsList, QWidget * parent) : QWidget(parent), stylesList(stylesList), moodsList(moodsList) {
    setObjectName(QStringLiteral("SongSearch"));
    generateLayout();
}

void SongSearch::spanChanged(int lower, int upper) {
    QxtSpanSlider * slider = (QxtSpanSlider *)sender();
    QLabel * display = (QLabel *)slider -> property("display").toInt();
    float coeff = slider -> property("coeff").toFloat();
    QString newText = display -> property("original_text").toString();
    display -> setText(newText % QStringLiteral(" (%1 to %2 %3)").arg(QString::number(lower / coeff), QString::number(upper / coeff), slider -> property("postfix").toString()));
}

void SongSearch::onDescriptionAdd() {
    QWidget * descBlock = new QWidget(descsLayout -> widget());
    descsLayout -> addWidget(descBlock);

    QGridLayout * descBlockLayout = new QGridLayout(descBlock);
    descBlock -> setLayout(descBlockLayout);

    descBlockLayout -> addWidget(new QLabel(QStringLiteral("Description")), 0, 0);

    QLineEdit * edit = new QLineEdit(descBlock);
    descBlockLayout -> addWidget(edit, 1, 0);
    edit -> setProperty("parent", (int)descBlock);

    descBlockLayout -> addWidget(new QLabel(QStringLiteral("Power")), 0, 1);

    QDoubleSpinBox * power = new QDoubleSpinBox(descBlock);
    power -> setSingleStep(.1);
    power -> setRange(-0.1, 2);
    power -> setValue(1);
    descBlockLayout -> addWidget(power, 1, 1);

    descriptions.insert(edit, power);

    QPushButton * remove = new QPushButton(QStringLiteral("Remove"), descBlock);
    remove -> setProperty("parent", (int)edit);
    descBlockLayout -> addWidget(remove, 1, 2);
    connect(remove, SIGNAL(clicked(bool)), this, SLOT(onDescriptionRemove()));
}
void SongSearch::onDescriptionRemove() {
    QPushButton * remove = (QPushButton *)sender();
    QLineEdit * edit = (QLineEdit *)remove -> property("parent").toInt();
    QWidget * descBlock = (QWidget *) edit -> property("parent").toInt();
    descriptions.remove(edit);
    descBlock -> deleteLater();
}

void SongSearch::onStyleAdd() {
    QWidget * styleBlock = new QWidget(stylesLayout -> widget());
    stylesLayout -> addWidget(styleBlock);

    QGridLayout * styleBlockLayout = new QGridLayout(styleBlock);
    styleBlock -> setLayout(styleBlockLayout);

    styleBlockLayout -> addWidget(new QLabel(QStringLiteral("Style")), 0, 0);

    QComboBox * edit = new QComboBox(styleBlock);
    edit -> addItems(stylesList);
    styleBlockLayout -> addWidget(edit, 1, 0);
    edit -> setProperty("parent", (int)styleBlock);

    styleBlockLayout -> addWidget(new QLabel(QStringLiteral("Power")), 0, 1);

    QDoubleSpinBox * power = new QDoubleSpinBox(styleBlock);
    power -> setSingleStep(.1);
    power -> setRange(-0.1, 2);
    power -> setValue(1);
    styleBlockLayout -> addWidget(power, 1, 1);

    styles.insert(edit, power);

    QPushButton * remove = new QPushButton(QStringLiteral("Remove"), styleBlock);
    remove -> setProperty("parent", (int)edit);
    styleBlockLayout -> addWidget(remove, 1, 2);
    connect(remove, SIGNAL(clicked(bool)), this, SLOT(onDescriptionRemove()));
}
void SongSearch::onStyleRemove() {
    QPushButton * remove = (QPushButton *)sender();
    QComboBox * edit = (QComboBox *)remove -> property("parent").toInt();
    QWidget * styleBlock = (QWidget *) edit -> property("parent").toInt();
    styles.remove(edit);
    styleBlock -> deleteLater();
}

void SongSearch::onMoodAdd() {
    QWidget * moodBlock = new QWidget(moodsLayout -> widget());
    moodsLayout -> addWidget(moodBlock);

    QGridLayout * moodBlockLayout = new QGridLayout(moodBlock);
    moodBlock -> setLayout(moodBlockLayout);

    moodBlockLayout -> addWidget(new QLabel(QStringLiteral("Mood")), 0, 0);

    QComboBox * edit = new QComboBox(moodBlock);
    edit -> addItems(moodsList);
    moodBlockLayout -> addWidget(edit, 1, 0);
    edit -> setProperty("parent", (int)moodBlock);

    moodBlockLayout -> addWidget(new QLabel(QStringLiteral("Power")), 0, 1);

    QDoubleSpinBox * power = new QDoubleSpinBox(moodBlock);
    power -> setSingleStep(.1);
    power -> setRange(-0.1, 2);
    power -> setValue(1);
    moodBlockLayout -> addWidget(power, 1, 1);

    moods.insert(edit, power);

    QPushButton * remove = new QPushButton(QStringLiteral("Remove"), moodBlock);
    remove -> setProperty("parent", (int)edit);
    moodBlockLayout -> addWidget(remove, 1, 2);
    connect(remove, SIGNAL(clicked(bool)), this, SLOT(onDescriptionRemove()));
}
void SongSearch::onMoodRemove() {
    QPushButton * remove = (QPushButton *)sender();
    QComboBox * edit = (QComboBox *)remove -> property("parent").toInt();
    QWidget * moodBlock = (QWidget *) edit -> property("parent").toInt();
    moods.remove(edit);
    moodBlock -> deleteLater();
}

void SongSearch::onSearchClicked() {
    emit moveInProcess();

    Core::Web::Echonest::SongTypeParamsList songTypeParams;
    for(QHash<QLabel *, QComboBox *>::Iterator songType = songTypes.begin(); songType != songTypes.end(); songType++)
        songTypeParams << Core::Web::Echonest::SongTypeParam(songType.key() -> text(), songType.value() -> currentText());


    QList<Core::Web::Echonest::ParamWithPower> descriptionParams;
    for(QHash<QLineEdit *, QDoubleSpinBox *>::Iterator description = descriptions.begin(); description != descriptions.end(); description++)
        descriptionParams << Core::Web::Echonest::ParamWithPower(description.key() -> text(), description.value() -> value());


    QList<Core::Web::Echonest::ParamWithPower> styleParams;
    for(QHash<QComboBox *, QDoubleSpinBox *>::Iterator style = styles.begin(); style != styles.end(); style++)
        styleParams << Core::Web::Echonest::ParamWithPower(style.key() -> currentText(), style.value() -> value());

    QList<Core::Web::Echonest::ParamWithPower> moodParams;
    for(QHash<QComboBox *, QDoubleSpinBox *>::Iterator mood = moods.begin(); mood != moods.end(); mood++)
        moodParams << Core::Web::Echonest::ParamWithPower(mood.key() -> currentText(), mood.value() -> value());

    Core::Web::Echonest::DGSMParams * gsmParams = new Core::Web::Echonest::DGSMParams(
        descriptionParams,
        styleParams,
        moodParams
    );

    Core::Web::Echonest::SongSearchParams params(
        artist -> text(),
        title -> text(),
        combined -> isChecked(),
        artistFromYear -> currentText(),
        artistToYear -> currentText(),
        mode -> currentText().toInt(),
        songTypeParams,
        gsmParams,
        new Core::Web::Echonest::IntervalParam(artist_familiarity -> lowerValue() / 1000.0, artist_familiarity -> upperValue() / 1000.0),
        new Core::Web::Echonest::IntervalParam(song_hotttnesss -> lowerValue() / 1000.0, song_hotttnesss -> upperValue() / 1000.0),
        new Core::Web::Echonest::IntervalParam(song_tempo -> lowerValue(), song_tempo -> upperValue()),
        new Core::Web::Echonest::IntervalParam(song_danceability -> lowerValue() / 1000.0, song_danceability -> upperValue() / 1000.0),
        new Core::Web::Echonest::IntervalParam(song_energy -> lowerValue() / 1000.0, song_energy -> upperValue() / 1000.0),
        new Core::Web::Echonest::IntervalParam(song_liveness -> lowerValue() / 1000.0, song_liveness -> upperValue() / 1000.0),
        new Core::Web::Echonest::IntervalParam(song_speechiness -> lowerValue() / 1000.0, song_speechiness -> upperValue() / 1000.0),
        new Core::Web::Echonest::IntervalParam(song_acousticness -> lowerValue() / 1000.0, song_acousticness -> upperValue() / 1000.0)
    );

    QJsonArray results = Core::Web::Echonest::Api::obj().songSearch(params);

//    {
//      "artist_id": "ARH6W4X1187B99274F",
//      "id": "SOCZZBT12A6310F251",
//      "artist_name": "Radiohead",
//      "title": "Karma Police"
//    }



    QStringList predicates;

    for(QJsonArray::Iterator song = results.begin(); song != results.end(); song++) {
        QJsonObject obj = (*song).toObject();
        predicates << (obj.value(QStringLiteral("artist_name")).toString() % QStringLiteral(" - ") % obj.value(QStringLiteral("title")).toString());
    }

    emit playlistGenerationNeed(QStringLiteral("Echonest predicable playlist"), predicates);
}

QStringList SongSearch::yearsSet() {
    if (yearslist.isEmpty()) {
        yearslist << QString();

        for(int i = 1910; i < QDateTime::currentDateTime().date().year(); i++)
            yearslist << QString::number(i);

        yearslist << QStringLiteral("present");
    }

    return yearslist;
}

void SongSearch::generateLayout() {
    QVBoxLayout * songSearchLayout = new QVBoxLayout(this);

    QGroupBox * songAttrGroup = new QGroupBox(QStringLiteral("Song attrs"), this);
    QVBoxLayout * songAttrsLayout = new QVBoxLayout(songAttrGroup);
    songSearchLayout -> addWidget(songAttrGroup);

    songAttrsLayout -> addWidget((combined = new QCheckBox(QStringLiteral("Search by artist and title"), songAttrGroup)));
    songAttrsLayout -> addWidget(new QLabel(QStringLiteral("Artist"), songAttrGroup));
    songAttrsLayout -> addWidget((artist = new QLineEdit(songAttrGroup)));
    songAttrsLayout -> addWidget(new QLabel(QStringLiteral("Song title"), songAttrGroup));
    songAttrsLayout -> addWidget((title = new QLineEdit(songAttrGroup)));


    QGroupBox * songCredentilsGroup = new QGroupBox(QStringLiteral("Song credentials"), this);
    QVBoxLayout * songCredentilsLayout = new QVBoxLayout(songCredentilsGroup);
    songSearchLayout -> addWidget(songCredentilsGroup);


    songCredentilsLayout -> addWidget(new QLabel(QStringLiteral("Tonality of songs"), songCredentilsGroup));
    mode = new QComboBox(songCredentilsGroup);
    mode -> addItem(QStringLiteral("Any"));
    mode -> addItem(QStringLiteral("Minor"), 0);
    mode -> addItem(QStringLiteral("Major"), 1);
    songCredentilsLayout -> addWidget(mode);


    QGroupBox * songTypesGroup = new QGroupBox(QStringLiteral("Song types"), songCredentilsGroup);
    QGridLayout * songTypesLayout = new QGridLayout(songTypesGroup);
    songCredentilsLayout -> addWidget(songTypesGroup);

    QList<QString> types; types << QStringLiteral("christmas") << QStringLiteral("live")
        << QStringLiteral("studio") << QStringLiteral("acoustic") << QStringLiteral("electric");
    QList<QString> chooses; chooses << QStringLiteral("any") << QStringLiteral("true") << QStringLiteral("false");

    for(QList<QString>::Iterator tipe = types.begin(); tipe != types.end(); tipe++) {
        QLabel * typeTitle = new QLabel(*tipe, songTypesGroup);
        QComboBox * typeChoose = new QComboBox(songTypesGroup);
        typeChoose -> addItems(chooses);

        songTypesLayout -> addWidget(typeTitle, songTypesLayout -> rowCount(), 0);
        songTypesLayout -> addWidget(typeChoose, songTypesLayout -> rowCount() - 1, 1);

        songTypes.insert(typeTitle, typeChoose);
    }


    songCredentilsLayout -> addWidget(new QLabel(QStringLiteral("Artist carrier starts from"), songCredentilsGroup));
    artistFromYear = new QComboBox(songCredentilsGroup);
    artistFromYear -> addItems(yearsSet());
    songCredentilsLayout -> addWidget(artistFromYear);

    songCredentilsLayout -> addWidget(new QLabel(QStringLiteral("Artist carrier ends in"), songCredentilsGroup));
    artistToYear = new QComboBox(songCredentilsGroup);
    artistToYear -> addItems(yearsSet());
    songCredentilsLayout -> addWidget(artistToYear);




    QLabel * label = new QLabel(songCredentilsGroup);
    label -> setProperty("original_text", QStringLiteral("Artist familiarity"));
    songCredentilsLayout -> addWidget(label);
    artist_familiarity = new QxtSpanSlider(Qt::Horizontal, songCredentilsGroup);
    artist_familiarity -> setShowRail(true);
    connect(artist_familiarity, SIGNAL(spanChanged(int,int)), this, SLOT(spanChanged(int,int)));
    artist_familiarity -> setProperty("display", (int)label);
    artist_familiarity -> setProperty("coeff", 10);
    artist_familiarity -> setProperty("postfix", QStringLiteral("%"));
    artist_familiarity -> setMinimum(0); artist_familiarity -> setMaximum(1000); artist_familiarity -> setUpperValue(artist_familiarity -> maximum());
    artist_familiarity -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songCredentilsLayout -> addWidget(artist_familiarity);

    label = new QLabel(songCredentilsGroup);
    label -> setProperty("original_text", QStringLiteral("Song hotttnesss"));
    songCredentilsLayout -> addWidget(label);
    song_hotttnesss = new QxtSpanSlider(Qt::Horizontal, songCredentilsGroup);
    connect(song_hotttnesss, SIGNAL(spanChanged(int,int)), this, SLOT(spanChanged(int,int)));
    song_hotttnesss -> setProperty("display", (int)label);
    song_hotttnesss -> setProperty("coeff", 10);
    song_hotttnesss -> setProperty("postfix", QStringLiteral("%"));
    song_hotttnesss -> setMinimum(0); song_hotttnesss -> setMaximum(1000); song_hotttnesss -> setUpperValue(song_hotttnesss -> maximum());
    song_hotttnesss -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songCredentilsLayout -> addWidget(song_hotttnesss);

    label = new QLabel(songCredentilsGroup);
    label -> setProperty("original_text", QStringLiteral("Song danceability"));
    songCredentilsLayout -> addWidget(label);
    song_danceability = new QxtSpanSlider(Qt::Horizontal, songCredentilsGroup);
    connect(song_danceability, SIGNAL(spanChanged(int,int)), this, SLOT(spanChanged(int,int)));
    song_danceability -> setProperty("display", (int)label);
    song_danceability -> setProperty("coeff", 10);
    song_danceability -> setProperty("postfix", QStringLiteral("%"));
    song_danceability -> setMinimum(0); song_danceability -> setMaximum(1000); song_danceability -> setUpperValue(song_danceability -> maximum());
    song_danceability -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songCredentilsLayout -> addWidget(song_danceability);

    label = new QLabel(songCredentilsGroup);
    label -> setProperty("original_text", QStringLiteral("Song energy"));
    songCredentilsLayout -> addWidget(label);
    song_energy = new QxtSpanSlider(Qt::Horizontal, songCredentilsGroup);
    connect(song_energy, SIGNAL(spanChanged(int,int)), this, SLOT(spanChanged(int,int)));
    song_energy -> setProperty("display", (int)label);
    song_energy -> setProperty("coeff", 10);
    song_energy -> setProperty("postfix", QStringLiteral("%"));
    song_energy -> setMinimum(0); song_energy -> setMaximum(1000); song_energy -> setUpperValue(song_energy -> maximum());
    song_energy -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songCredentilsLayout -> addWidget(song_energy);

    label = new QLabel(songCredentilsGroup);
    label -> setProperty("original_text", QStringLiteral("Song liveness"));
    songCredentilsLayout -> addWidget(label);
    song_liveness = new QxtSpanSlider(Qt::Horizontal, songCredentilsGroup);
    connect(song_liveness, SIGNAL(spanChanged(int,int)), this, SLOT(spanChanged(int,int)));
    song_liveness -> setProperty("display", (int)label);
    song_liveness -> setProperty("coeff", 10);
    song_liveness -> setProperty("postfix", QStringLiteral("%"));
    song_liveness -> setMinimum(0); song_liveness -> setMaximum(1000); song_liveness -> setUpperValue(song_liveness -> maximum());
    song_liveness -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songCredentilsLayout -> addWidget(song_liveness);

    label = new QLabel(songCredentilsGroup);
    label -> setProperty("original_text", QStringLiteral("Song speechiness"));
    songCredentilsLayout -> addWidget(label);
    song_speechiness = new QxtSpanSlider(Qt::Horizontal, songCredentilsGroup);
    connect(song_speechiness, SIGNAL(spanChanged(int,int)), this, SLOT(spanChanged(int,int)));
    song_speechiness -> setProperty("display", (int)label);
    song_speechiness -> setProperty("coeff", 10);
    song_speechiness -> setProperty("postfix", QStringLiteral("%"));
    song_speechiness -> setMinimum(0); song_speechiness -> setMaximum(1000); song_speechiness -> setUpperValue(song_speechiness -> maximum());
    song_speechiness -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songCredentilsLayout -> addWidget(song_speechiness);

    label = new QLabel(songCredentilsGroup);
    label -> setProperty("original_text", QStringLiteral("Song acousticness"));
    songCredentilsLayout -> addWidget(label);
    song_acousticness = new QxtSpanSlider(Qt::Horizontal, songCredentilsGroup);
    connect(song_acousticness, SIGNAL(spanChanged(int,int)), this, SLOT(spanChanged(int,int)));
    song_acousticness -> setProperty("display", (int)label);
    song_acousticness -> setProperty("coeff", 10);
    song_acousticness -> setProperty("postfix", QStringLiteral("%"));
    song_acousticness -> setMinimum(0); song_acousticness -> setMaximum(1000); song_acousticness -> setUpperValue(song_acousticness -> maximum());
    song_acousticness -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songCredentilsLayout -> addWidget(song_acousticness);

    label = new QLabel(songCredentilsGroup);
    label -> setProperty("original_text", QStringLiteral("Song tempo"));
    songCredentilsLayout -> addWidget(label);
    song_tempo = new QxtSpanSlider(Qt::Horizontal, songCredentilsGroup);
    connect(song_tempo, SIGNAL(spanChanged(int,int)), this, SLOT(spanChanged(int,int)));
    song_tempo -> setProperty("display", (int)label);
    song_tempo -> setProperty("coeff", 1);
    song_tempo -> setProperty("postfix", QStringLiteral("BPM"));
    song_tempo -> setMinimum(0); song_tempo -> setMaximum(1000); song_tempo -> setUpperValue(song_tempo -> maximum());
    song_tempo -> setHandleMovementMode(QxtSpanSlider::NoCrossing);
    songCredentilsLayout -> addWidget(song_tempo);



    QGroupBox * songChracterizationGroup = new QGroupBox(QStringLiteral("Song characrezition"), this);
    QVBoxLayout * songChracterizationLayout = new QVBoxLayout(songChracterizationGroup);
    songSearchLayout -> addWidget(songChracterizationGroup);


    // descriptions block
    QWidget * songDescriptions = new QWidget(songChracterizationGroup);
    songChracterizationLayout -> addWidget(songDescriptions);

    descsLayout = new QVBoxLayout(songDescriptions);
    songDescriptions -> setLayout(descsLayout);

    QPushButton * addDescription = new QPushButton(QStringLiteral("Add Description creteria"), songChracterizationGroup);
    songChracterizationLayout -> addWidget(addDescription);
    connect(addDescription, SIGNAL(clicked(bool)), this, SLOT(onDescriptionAdd()));

    // styles block
    QWidget * songStyles = new QWidget(songChracterizationGroup);
    songChracterizationLayout -> addWidget(songStyles);

    stylesLayout = new QVBoxLayout(songStyles);
    songStyles -> setLayout(stylesLayout);

    QPushButton * addStyle = new QPushButton(QStringLiteral("Add Style creteria"), songChracterizationGroup);
    songChracterizationLayout -> addWidget(addStyle);
    connect(addStyle, SIGNAL(clicked(bool)), this, SLOT(onStyleAdd()));

    // moods block
    QWidget * songMoods = new QWidget(songChracterizationGroup);
    songChracterizationLayout -> addWidget(songMoods);

    moodsLayout = new QVBoxLayout(songMoods);
    songMoods -> setLayout(moodsLayout);

    QPushButton * addMood = new QPushButton(QStringLiteral("Add Mood creteria"), songChracterizationGroup);
    songChracterizationLayout -> addWidget(addMood);
    connect(addMood, SIGNAL(clicked(bool)), this, SLOT(onMoodAdd()));


    QPushButton * startSearching = new QPushButton(QStringLiteral("Search"), this);
    connect(startSearching, SIGNAL(clicked(bool)), this, SLOT(onSearchClicked()));
    songSearchLayout -> addWidget(startSearching);
}
