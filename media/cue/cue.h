#ifndef CUE
#define CUE

#include <qfile.h>
#include <qtextstream.h>

#include "cue_structs.h"

class Cue {
    public:
        Cue(QIODevice & obj);
    protected:
        void initRules();
    private:
        QList<CueFile> files;
};

#endif // CUE
