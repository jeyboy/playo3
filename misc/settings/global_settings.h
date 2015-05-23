#ifndef GLOBAL_SETTINGS
#define GLOBAL_SETTINGS

#include <qapplication.h>
#include <qjsonobject.h>

#include "modules/data_struct/container_types.h"

class GlobalSettings {
public:
    void fromJson(QJsonObject & settings);
    void toJson(QJsonObject & settings);

    inline QString defaultDownloadPath() const { return _defaultDownloadPath; }
    inline void setDefaultDownloadPath(QString newPath) { _defaultDownloadPath = newPath; }

    inline bool isMetricShow() const { return _showMetric; }
    inline void setMetricShow(bool show) { _showMetric = show; }

    inline bool isMetricNumero() const { return _showMetricNumero; }
    inline void setMetricNumeroShow(bool show) { _showMetricNumero = show; }

    inline bool isSaveCommonTab() const { return _saveCommonTab; }
    inline void setSaveCommonTab(bool save) { _saveCommonTab = save; }

    inline bool isOpenDropPointInTab() const { return _openDropPointInTab; }
    inline void setOpenDropPointInTab(bool open) { _openDropPointInTab = open; }

    inline Playo3::ContainerType openDropPointInTabType() const { return _openDropPointInTabType; }
    inline void setOpenDropPointInTabType(Playo3::ContainerType type) { _openDropPointInTabType = type; }

    inline int tabPosition() { return _tabPosition; }
    inline void setTabPosition(int newPositionsType) { _tabPosition = newPositionsType; }

protected:
    QString _defaultDownloadPath;
    bool _showMetric;
    bool _showMetricNumero;

    bool _saveCommonTab;
    bool _openDropPointInTab;
    Playo3::ContainerType _openDropPointInTabType;
    int _tabPosition;
};

#endif // GLOBAL_SETTINGS
