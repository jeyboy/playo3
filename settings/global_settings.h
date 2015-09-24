#ifndef GLOBAL_SETTINGS
#define GLOBAL_SETTINGS

#include <qstringbuilder.h>
#include <qapplication.h>
#include <qjsonobject.h>

#include "modules/views/view_types.h"

#define SETTINGS_SHOW_METRICS_KEY QStringLiteral("show_metric")
#define SETTINGS_SHOW_METRICS_NUMERO_KEY QStringLiteral("show_metric_numero")
#define SETTINGS_DOWNLOAD_PATH_KEY QStringLiteral("download_path")
#define SETTINGS_AUTORUNNED_KEY QStringLiteral("autorunned")
#define SETTINGS_TAB_POSITION_KEY QStringLiteral("tab_position")
#define SETTINGS_SAVE_COMMON_TAB_KEY QStringLiteral("save_common_tab")
#define SETTINGS_OPEN_DROP_IN_TAB_KEY QStringLiteral("open_drop_point_in_tab")
#define SETTINGS_OPEN_DROP_IN_TAB_TYPE_KEY QStringLiteral("open_drop_point_in_tab_type")

#define SETTINGS_OPEN_TIMEOUT QStringLiteral("open_time_out_key")

class GlobalSettings {
public:
    void fromJson(QJsonObject & settings);
    void toJson(QJsonObject & settings);

    inline QString defaultDownloadPath() const { return _defaultDownloadPath; }
    inline void setDefaultDownloadPath(QString newPath) { _defaultDownloadPath = newPath; }

    inline bool isMetricShow() const { return _showMetric; }
    inline void setMetricShow(bool show) { _showMetric = show; }

    inline bool isAutorunned() const { return _isAutorunned; }
    inline void setAutorun(bool is_autorunned) { _isAutorunned = is_autorunned; }

    inline bool isMetricNumero() const { return _showMetricNumero; }
    inline void setMetricNumeroShow(bool show) { _showMetricNumero = show; }

    inline bool isSaveCommonTab() const { return _saveCommonTab; }
    inline void setSaveCommonTab(bool save) { _saveCommonTab = save; }

    inline bool isOpenDropPointInTab() const { return _openDropPointInTab; }
    inline void setOpenDropPointInTab(bool open) { _openDropPointInTab = open; }

    inline Model::Type openDropPointInTabType() const { return _openDropPointInTabType; }
    inline void setOpenDropPointInTabType(const Model::Type & type) { _openDropPointInTabType = type; }

    inline int tabPosition() { return _tabPosition; }
    inline void setTabPosition(int newPositionsType) { _tabPosition = newPositionsType; }

    inline float openTimeOut() { return _openTimeOut; }
    inline void setOpenTimeOut(int newTimeOut) { _openTimeOut = newTimeOut; }

protected:
    QString _defaultDownloadPath;
    bool _showMetric;
    bool _showMetricNumero;
    bool _isAutorunned;

    bool _saveCommonTab;
    bool _openDropPointInTab;
    Playo3::ContainerType _openDropPointInTabType;
    int _tabPosition;

    float _openTimeOut;
};

#endif // GLOBAL_SETTINGS
