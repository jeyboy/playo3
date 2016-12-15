#ifndef FOURSHARED_SET
#define FOURSHARED_SET

#include "fourshared_audio.h"
#include "fourshared_video.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Set : public Audio, public Video {
            protected:
                enum SetType { set_popular_audio, set_popular_video };

                QString setTypeToStr(const SetType & stype) {
                    switch(stype) {
                        case set_popular_audio: return LSTR("Popular audio");
                        case set_popular_video: return LSTR("Popular video");
                        default: return LSTR("Unknown");
                    }
                }

                QJsonValue setByType(const SetType & set_type, const SearchLimit & limits, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    QueriableResponse response;
                    DataMediaType dmt_val = dmt_unknow;

                    switch(set_type) {
                        case set_popular_audio: {
                            dmt_val = dmt_audio;
                            response = audioPopular(limits);
                        break;}

                        case set_popular_video: {
                            dmt_val = dmt_video;
                            response = videoPopular(limits);
                        break;}
                    }

                    return prepareBlock(dmt_val, cmd_mtd_set_by_type, response, limits, block_params, {{CMD_SET_TYPE, set_type}});
                }
            public:
                inline QMap<QString, QString> setsList() {
                    QMap<QString, QString> res;
                    Cmd cmd_tmpl(sourceType(), cmd_mtd_open_set, {});

                    APPEND_SET_OPTION(res, cmd_tmpl, set_popular_audio);
                    APPEND_SET_OPTION(res, cmd_tmpl, set_popular_video);

                    return res;
                }
            };
        }
    }
}

#endif // FOURSHARED_SET
