#ifndef OD_COLLECTION
#define OD_COLLECTION

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Collection : public Base {
            protected:
                inline QString collectionsAudioUrl() { return audioUrlStr(path_audio_collections); } // params: (collectionId: not used for index of collections) and pagination attrs

                QJsonValue tracksByArtist(const QString & artist_id) { //TODO: not finished
                    return pRequest(
                        audioUrlStr(
                            path_audio_by_artist_id,
                            { {QStringLiteral("artistId"), artist_id} }
                        ),
                        call_type_json, rules(), 0,
                        proc_json_extract, QStringList() << tkn_artists
                    );
                }
            };
        }
    }
}

#endif // OD_COLLECTION
