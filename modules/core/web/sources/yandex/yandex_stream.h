#ifndef YANDEX_STREAM_H
#define YANDEX_STREAM_H

//curl 'https://music.yandex.ua/api/v2.1/handlers/radio/genre/relax/settings/n/ru/e1tHp88dt--aL4xrIiq9qQ~~-1asb9kceg16492250&external-domain=music.yandex.ua&overembed=no&__t=1473552998747' -H 'Accept: application/json; q=1.0, text/*; q=0.8, */*; q=0.1' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Referer: https://music.yandex.ua/genre/easy' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https%3A%2F%2Fmusic.yandex.ua%2Fgenre%2Feasy'

// curl 'https://music.yandex.ua/api/v2.1/handlers/radio/genre/rnb/feedback/trackStarted/5570:7112/sy/net/match_r1201.html?&__t=1474438662883' -H 'Host: music.yandex.ua' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'Accept: application/json; q=1.0, text/*; q=0.8, */*; q=0.1' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Content-Type: application/x-www-form-urlencoded' -H 'Referer: https://music.yandex.ua/genre/r-n-b' -H 'X-Retpath-Y: https%3A%2F%2Fmusic.yandex.ua%2Fgenre%2Fr-n-b' -H 'X-Requested-With: XMLHttpRequest' -H 'Cookie: yandexuid=2190563221452621883; L=WnEJRFAPV0xrQVpRcXN0SEdPV0t6bmtBXl0rEDg8e3p/cg==.1474342480.12659.397393.538aa840261797a55723bf7670f887c4; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Ffeed%22%7D; yabs-vdrf=SpDnNIW57Yqy0pDnNEmOrC0y1pDnNWG5dJ5C1pDnNDm3LUrK1WDnN5GIw4Am0VjnNDm1Aa5G1hTbN201Z8L819ifNtWF8wa009ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; spravka=dD0xNDczNjY2OTQwO2k9MTc4LjEzNy4xMTIuMjM7dT0xNDczNjY2OTQwOTQ1MjcyMDI1O2g9OTYzMWExMzk1OWQ1NDQ1MWRiNWMwZWFiYTZhNTNkNzU=; Session_id=noauth:1474430260; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"; _ym_isad=1' -H 'Connection: keep-alive' --data 'timestamp=1474438664332&from=web-genre-radio-radio-main&batchId=99fcf62b504c765dccaf5db9bc297b29.0&trackId=5570&albumId=7112&totalPlayed=0.1&sign=d6bac9e908af2b2fd5ad19b379630cace0dbf1ca%3A1474437751320&external-domain=music.yandex.ua&overembed=no'
// curl 'https://music.yandex.ua/api/v2.1/handlers/track/5570:7112/web-genre-radio-radio-main/feedback/end/n/ru/LKBUO9aoC_DQnRUgqplMmA~~-1at5m8uum1227.js?&__t=1474438835405' -X POST -H 'Host: music.yandex.ua' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'Accept: application/json; q=1.0, text/*; q=0.8, */*; q=0.1' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'X-Retpath-Y: https%3A%2F%2Fmusic.yandex.ua%2Fgenre%2Fr-n-b' -H 'Content-Type: application/json' -H 'X-Requested-With: XMLHttpRequest' -H 'Referer: https://music.yandex.ua/genre/r-n-b' -H 'Content-Length: 435' -H 'Cookie: yandexuid=2190563221452621883; L=WnEJRFAPV0xrQVpRcXN0SEdPV0t6bmtBXl0rEDg8e3p/cg==.1474342480.12659.397393.538aa840261797a55723bf7670f887c4; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Ffeed%22%7D; yabs-vdrf=SpDnNIW57Yqy0pDnNEmOrC0y1pDnNWG5dJ5C1pDnNDm3LUrK1WDnN5GIw4Am0VjnNDm1Aa5G1hTbN201Z8L819ifNtWF8wa009ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; spravka=dD0xNDczNjY2OTQwO2k9MTc4LjEzNy4xMTIuMjM7dT0xNDczNjY2OTQwOTQ1MjcyMDI1O2g9OTYzMWExMzk1OWQ1NDQ1MWRiNWMwZWFiYTZhNTNkNzU=; Session_id=noauth:1474430260; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"; _ym_isad=1' -H 'Connection: keep-alive'
// curl 'https://music.yandex.ua/api/v2.1/handlers/radio/genre/rnb/feedback/skip/31861:2653/n/ru/UuP7ceQIPXX6uYSosKm9Yg~~-1at5mg96b146929?0=v&1=i&2=d&3=e&4=o&5=-&6=c&7=a&8=t&9=e&10=g&11=o&12=r&13=y&14=-&15=i&16=d&17=%3D&18=0&19=%26&20=d&21=u&22=r&23=a&24=t&25=i&26=o&27=n&28=%3D&29=u&30=n&31=d&32=e&33=f&34=i&35=n&36=e&37=d&38=%26&39=c&40=l&41=i&42=e&43=n&44=t&45=_&46=t&47=y&48=p&49=e&50=%3D&51=h&52=t&53=m&54=l&55=%26&56=v&57=i&58=d&59=e&60=o&61=-&62=a&63=p&64=i&65=-&66=v&67=e&68=r&69=s&70=i&71=o&72=n&73=%3D&74=j&75=s&76=%25&77=3&78=A&79=1&80=.&81=0&82=-&83=3&84=4&85=8&__t=1474438913899' -H 'Host: music.yandex.ua' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'Accept: application/json; q=1.0, text/*; q=0.8, */*; q=0.1' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate, br' -H 'DNT: 1' -H 'Content-Type: application/x-www-form-urlencoded' -H 'Referer: https://music.yandex.ua/genre/r-n-b' -H 'X-Retpath-Y: https%3A%2F%2Fmusic.yandex.ua%2Fgenre%2Fr-n-b' -H 'X-Requested-With: XMLHttpRequest' -H 'Cookie: yandexuid=2190563221452621883; L=WnEJRFAPV0xrQVpRcXN0SEdPV0t6bmtBXl0rEDg8e3p/cg==.1474342480.12659.397393.538aa840261797a55723bf7670f887c4; yp=1464207350.ww.1#1472199353.szm.1%3A1920x1080%3A1920x969#1788794542.multib.1; _ym_uid=1473426341377070158; lastVisitedPage=%7B%22363617853%22%3A%22%2Ffeed%22%7D; yabs-vdrf=SpDnNIW57Yqy0pDnNEmOrC0y1pDnNWG5dJ5C1pDnNDm3LUrK1WDnN5GIw4Am0VjnNDm1Aa5G1hTbN201Z8L819ifNtWF8wa009ifNt01j_nG1USbNFm2Hxje0BCXN202rAXW1ZiTNIW6T4nu0lyPNDm0H3bS16iPNRW3cBLS1TiLNDm1yArq1eiHNDm3V1oi1eiHNt02WOLW1nxvN402CAm400; spravka=dD0xNDczNjY2OTQwO2k9MTc4LjEzNy4xMTIuMjM7dT0xNDczNjY2OTQwOTQ1MjcyMDI1O2g9OTYzMWExMzk1OWQ1NDQ1MWRiNWMwZWFiYTZhNTNkNzU=; Session_id=noauth:1474430260; device_id="b79263ca059d9f7d5505a415b6cf5632af8419b20"; _ym_isad=1' -H 'Connection: keep-alive' --data 'timestamp=1474438915347&from=web-genre-radio-radio-main&batchId=7d5b81fb7fb1f29a6a3107a8a7b0cd0e.0&trackId=31861&albumId=2653&totalPlayed=16.459092&sign=d6bac9e908af2b2fd5ad19b379630cace0dbf1ca%3A1474437751320&external-domain=music.yandex.ua&overembed=no'


#include "yandex_defines.h"
#include "modules/controls/clickable_slider.h"
#include "qgroupbox.h"
#include "qradiobutton.h"

namespace Core {
    namespace Web {
        namespace Yandex {
            class Stream : public virtual Base {
            public:
                QWidget * streamSettingsBlock(const QVariant & data) {
                    QVariantMap curr_settings = data.toMap();

                    QHash<QString, QWidget *> elems;

                    QWidget * block = new QWidget();
                    QGridLayout * layout = new QGridLayout(block);
                    int col_span = 6;

                    layout -> addWidget(new QLabel(LSTR("Mood"), block), 0, 0, 1, col_span, Qt::AlignCenter);
                    layout -> addWidget(new QLabel(LSTR("Calmly"), block), 1, 0);
                    Controls::ClickableSlider * mood_slider = new Controls::ClickableSlider(block);
                    mood_slider -> setRange(1, 4);
                    mood_slider -> setOrientation(Qt::Horizontal);
                    mood_slider -> setValue(curr_settings[YANEX_STREAM_MOOD].toInt());
                    layout -> addWidget(mood_slider, 1, 1, 1, 4);
                    layout -> addWidget(new QLabel(LSTR("Cheerful"), block), 1, col_span - 1);
                    elems.insert(YANEX_STREAM_MOOD, mood_slider);

                    layout -> addWidget(new QLabel(LSTR("Energy"), block), 2, 0, 1, col_span, Qt::AlignCenter);
                    layout -> addWidget(new QLabel(LSTR("Less"), block), 3, 0);
                    Controls::ClickableSlider * energy_slider = new Controls::ClickableSlider(block);
                    energy_slider -> setRange(1, 4);
                    energy_slider -> setOrientation(Qt::Horizontal);
                    energy_slider -> setValue(curr_settings[YANEX_STREAM_ENERGY].toInt());
                    layout -> addWidget(energy_slider, 3, 1, 1, 4);
                    layout -> addWidget(new QLabel(LSTR("More"), block), 3, col_span - 1);
                    elems.insert(YANEX_STREAM_ENERGY, energy_slider);


                    layout -> addWidget(new QLabel(LSTR("Language"), block), 4, 0, 1, 2, Qt::AlignCenter);
                    QString curr_lang = curr_settings[YANEX_STREAM_LANG].toString();
                    QComboBox * lang_select = new QComboBox(block);
                    int lang_index = 0;

                    QHash<QString, QString> langs = {{LSTR("Any"), LSTR("any")}, {LSTR("Russian"), LSTR("russian")},
                                                     {LSTR("Non Russian"), LSTR("not-russian")}};
                    for(QHash<QString, QString>::Iterator lang = langs.begin(); lang != langs.end(); lang++) {
                        lang_select -> addItem(lang.key(), lang.value());
                        if (curr_lang != lang.value())
                            lang_index++;
                    }
                    lang_select -> setCurrentIndex(lang_index);
                    elems.insert(YANEX_STREAM_LANG, lang_select);
                    layout -> addWidget(lang_select, 4, 3, 1, 4);



                    layout -> addWidget(new QLabel(LSTR("Diversity"), block), 5, 0, 1, 2, Qt::AlignCenter);
                    QString curr_diversity = curr_settings[YANEX_STREAM_DIVERSITY].toString();
                    QComboBox * diversity_select = new QComboBox(block);
                    int diversity_index = 0;

                    QHash<QString, QString> diversities = {{LSTR("Any"), LSTR("default")}, {LSTR("Popular"), LSTR("popular")},
                                                           {LSTR("Favorite"), LSTR("favorite")}, {LSTR("Diverse"), LSTR("diverse")}};
                    for(QHash<QString, QString>::Iterator diversity = diversities.begin(); diversity != diversities.end(); diversity++) {
                        diversity_select -> addItem(diversity.key(), diversity.value());
                        if (curr_diversity != diversity.value())
                            diversity_index++;
                    }
                    diversity_select -> setCurrentIndex(lang_index);
                    elems.insert(YANEX_STREAM_DIVERSITY, diversity_select);
                    layout -> addWidget(diversity_select, 5, 3, 1, 4);


                    settings_forms.insert(sst_stream, elems);
                    return block;
                }

                QVariant streamConfiguration(const QUrlQuery & args) {
                    QJsonObject json = JSON_OBJ(streamConfiguration(args.queryItemValue(CMD_GENRE)).toObject(), LSTR("settings"));
                    return QVariantMap{
                        {YANEX_STREAM_LANG, JSON_STR(json, YANEX_STREAM_LANG)}, {YANEX_STREAM_MOOD, JSON_INT(json, YANEX_STREAM_MOOD)},
                        {YANEX_STREAM_ENERGY, JSON_INT(json, YANEX_STREAM_ENERGY)}, {YANEX_STREAM_DIVERSITY, JSON_STR(json, YANEX_STREAM_DIVERSITY)}
                    };
                }
                QJsonValue streamConfiguration(const QString & genre) {
                    QJsonObject response = sRequest(
                        baseUrlStr(
                            qst_site_alt2, LSTR("radio/genre/%1/settings").arg(genre), {}
                        ),
                        call_type_json, 0, proc_none, QStringList(), call_method_get, headers()
                    );

                    return response;
                }

                void applyStreamSettings(const QHash<QString, QWidget *> & data) {
                    //TODO: write me
                }
                QJsonValue streamConfigure(const QUrlQuery & args) {
                    return streamConfigure(
                        args.queryItemValue(CMD_GENRE),
                        args.queryItemValue(YANEX_STREAM_LANG),
                        args.queryItemValue(YANEX_STREAM_MOOD),
                        args.queryItemValue(YANEX_STREAM_ENERGY),
                        args.queryItemValue(YANEX_STREAM_DIVERSITY)
                    );
                }
                //POST /api/v2.1/handlers/radio/genre/trance/settings?__t=1474595765369 HTTP/1.1
                //language=any&mood=3&energy=3&diversity=default&sign=172d207c56f1213098e61e8b967222e7ec5a9c18%3A1474595513474&external-domain=music.yandex.ua&overembed=no
                // mood and energy has diapazon from 1 to 4
                // lang // any / russian / not-russian
                // diversity // popular / favorite / diverse - редкие / default - все
                QJsonValue streamConfigure(const QString & genre, const QString & lang = LSTR("any"), const QString & mood = LSTR("2"),
                        const QString & energy = LSTR("2"), const QString & diversity = LSTR("default")) {
                    QJsonObject response = sRequest(
                        baseUrlStr(
                            qst_site_alt2, LSTR("radio/genre/%1/settings").arg(genre),
                            {
                                {LSTR("language"), lang},
                                {LSTR("mood"), mood},
                                {LSTR("energy"), energy},
                                {LSTR("diversity"), diversity},
                                {LSTR("sign"), siteAdditionalToken()}
                            }
                        ),
                        call_type_json, 0, proc_none, QStringList(), call_method_post, headers()
                    );

                    return response;
                }

                // queue (96212:67376,350006:35232) track:album
                QJsonValue streamsByGenre(const QUrlQuery & args) {
                    return streamsByGenre(
                        args.queryItemValue(CMD_GENRE),
                        args.queryItemValue(CMD_OFFSET)
                    );
                }
                QJsonValue streamsByGenre(const QString & genre, const QString & queue = QString("")) { // INFO: X-Retpath-Y header is mandatory there
                    QJsonArray content = saRequest(
                        baseUrlStr(
                            qst_site_alt2, LSTR("radio/genre/%1/tracks").arg(genre),
                            {{LSTR("queue"), queue}}
                        ),
                        call_type_json, 0, proc_json_extract, QStringList() << tkn_tracks << tkn_track, call_method_get, headers()
                    );
                    QString offset = queue;

                    if (!content.isEmpty()) {
                        if (content[0].isNull()) // sometimes first item is empty / this item broke logic
                            content.removeAt(0);

                        for(QJsonArray::Iterator item = content.begin(); item != content.end(); item++) {
                            QJsonObject it = (*item).toObject();

                            QJsonObject js = Manager::prepare() -> jsonPost(
                                baseUrlStr(
                                    qst_site_alt2, LSTR("radio/genre/%1/feedback/trackFinished/%2").arg(genre, YANDEX_ITEM_UID(it)),
                                    {
                                        {LSTR("timestamp"), QString::number(QDateTime::currentMSecsSinceEpoch())},
                                        {LSTR("from"), LSTR("web-genre-radio-radio-main")},
                                        {LSTR("batchId"), JSON_STR(it, LSTR("batchId"))},
                                        {LSTR("trackId"), JSON_CSTR(it, tkn_id)},
                                        {LSTR("albumId"), YANDEX_ITEM_ALBUM(it)},
                                        {LSTR("totalPlayed"), QString::number(JSON_INT(it, LSTR("durationMs")) / 1000 - 3)},
                                        {LSTR("sign"), siteAdditionalToken()}
                                    }
                                ), headers()
                            );

                            if (JSON_STR(js, LSTR("result")) != LSTR("ok"))
                                qCritical() << name() << "radio item finishing failed";
                        }
                        offset = YANDEX_ITEM_UID(content.first().toObject()) % ',' % YANDEX_ITEM_UID(content.last().toObject());
                    }

                    return prepareBlock(
                        dmt_audio, prepareTracks(content),
                        {
                            {
                                tkn_more_cmd, Cmd::build(
                                    sourceType(), cmd_mtd_streams_by_genre,
                                    {{CMD_GENRE, genre}, {CMD_OFFSET, offset}}
                                ).toString()
                            }
                        }
                    );
                }

                //curl 'https://music.yandex.ua/api/v2.1/handlers/radio/artist/79215/tracks/a/net/get-direct/F4GGm-tghyrZErZ2vj9USg/x80?queue=&external-domain=music.yandex.ua&overembed=no&__t=1473809509622' -H 'Accept: application/json; q=1.0, text/*; q=0.8, */*; q=0.1' -H 'Accept-Language: en-US,en;q=0.5' -H 'Connection: keep-alive' -H 'DNT: 1' -H 'Host: music.yandex.ua' -H 'Referer: https://music.yandex.ua/artist/79215' -H 'User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0' -H 'X-Requested-With: XMLHttpRequest' -H 'X-Retpath-Y: https%3A%2F%2Fmusic.yandex.ua%2Fartist%2F79215'
                QJsonValue streamByArtist(const QUrlQuery & args) {
                    return streamByArtist(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET)
                    );
                }
                QJsonValue streamByArtist(const QString & artist_id, const QString & queue = QString("")) { // TODO: test me
                    QJsonArray content = saRequest(
                        baseUrlStr(
                            qst_site_alt2, LSTR("radio/artist/%1/tracks").arg(artist_id),
                            {{LSTR("queue"), queue}}
                        ),
                        call_type_json, 0, proc_json_extract, QStringList() << tkn_tracks << tkn_track, call_method_get, headers()
                    );

                    QJsonObject first_obj = content[content.size() - 2].toObject();
                    QJsonObject last_obj = content[content.size() - 1].toObject();

                    QString offset = encodeStr(YANDEX_ITEM_UID(first_obj) % ',' % YANDEX_ITEM_UID(last_obj));
                    return prepareBlock(
                        dmt_audio, prepareTracks(content),
                        {
                            {
                                tkn_more_cmd, Cmd::build(
                                    sourceType(), cmd_mtd_stream_by_artist,
                                    {{CMD_ID, artist_id}, {CMD_OFFSET, offset}}
                                ).toString()
                            }
                        }
                    );
                }
            };
        }
    }
}

#endif // YANDEX_STREAM_H
