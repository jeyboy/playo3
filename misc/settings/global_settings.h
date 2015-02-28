#ifndef GLOBAL_SETTINGS
#define GLOBAL_SETTINGS

#include <qapplication.h>
#include <qjsonobject.h>

class GlobalSettings {
public:
    void fromJson(QJsonObject & settings);
    void toJson(QJsonObject & settings);

    inline QString defaultDownloadPath() const { return _defaultDownloadPath; }
    inline void setDefaultDownloadPath(QString newPath) { _defaultDownloadPath = newPath; }

    inline bool isMetricShow() const { return _showMetric; }
    inline void setMetricShow(bool show) { _showMetric = show; }

    inline int tabPosition() { return _tabPosition; }
    inline void setTabPosition(int newPositionsType) { _tabPosition = newPositionsType; }

protected:
    QString _defaultDownloadPath;
    bool _showMetric;
    int _tabPosition;
};

#endif // GLOBAL_SETTINGS
