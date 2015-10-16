#ifndef FILE_ERRORS
#define FILE_ERRORS

#include <qfile.h>

namespace Core {
    class FileErrors {
    protected:
        QString ioError(QFile * file) {
            switch(file -> error()) {
                case QFile::NoError:            { return "All ok"; }
                case QFile::ReadError:          { return "Read error"; }
                case QFile::WriteError:         { return "Write error"; }
                case QFile::FatalError:         { return "Fatal error"; }
                case QFile::ResourceError:      { return "Out of resources (free space, etc)"; }
                case QFile::OpenError:          { return "Open error"; }
                case QFile::AbortError:         { return "Operation was aborted"; }
                case QFile::TimeOutError:       { return "Timeout occurred"; }
                case QFile::UnspecifiedError:   { return "Unspecified error occurred"; }
                case QFile::RemoveError:        { return "Could not be removed"; }
                case QFile::RenameError:        { return "Could not be renamed"; }
                case QFile::PositionError:      { return "Position in the file could not be changed"; }
                case QFile::ResizeError:        { return "Free space of memory is not enough"; }
                case QFile::PermissionsError:   { return "Is not accessable"; }
                case QFile::CopyError:          { return "Could not be copied"; }
                default: return "Unknow error";
            }
        }
    };
}

#endif // FILE_ERRORS
