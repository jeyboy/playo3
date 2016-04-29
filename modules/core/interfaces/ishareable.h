#ifndef ISHAREABLE
#define ISHAREABLE

#include <qjsonarray.h>

namespace Core {
  class IShareable {
  public:
      inline bool isShareable() { return true; }
      virtual QJsonArray audioInfo(QStringList & audio_uids) = 0;
  };
}

#endif // ISHAREABLE
