#ifndef ISHAREABLE
#define ISHAREABLE

#include <qjsonarray.h>

namespace Core {
  class IShareable {
  public:
      virtual QJsonArray audioInfo(const QStringList & audio_uids) = 0;
  };
}

#endif // ISHAREABLE
