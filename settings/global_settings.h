#ifndef GLOBAL_SETTINGS
#define GLOBAL_SETTINGS

#include <qstringbuilder.h>
#include <qapplication.h>
#include <qjsonobject.h>

#include "modules/core/playlist_types.h"

#define SETTINGS_SHOW_METRICS_KEY QStringLiteral("show_metric")
#define SETTINGS_SHOW_METRICS_NUMERO_KEY QStringLiteral("show_metric_numero")
#define SETTINGS_DOWNLOAD_PATH_KEY QStringLiteral("download_path")
#define SETTINGS_AUTORUNNED_KEY QStringLiteral("autorunned")
#define SETTINGS_TAB_POSITION_KEY QStringLiteral("tab_position")
#define SETTINGS_SAVE_COMMON_TAB_KEY QStringLiteral("save_common_tab")
#define SETTINGS_OPEN_DROP_IN_TAB_KEY QStringLiteral("open_drop_point_in_tab")
#define SETTINGS_OPEN_DROP_IN_TAB_TYPE_KEY QStringLiteral("open_drop_point_in_tab_type")
#define SETTINGS_OPEN_TIMEOUT_KEY QStringLiteral("open_time_out_key")
#define SETTINGS_COLOR_SCHEME_KEY QStringLiteral("color_scheme")
#define SETTINGS_TOOL_ICON_SIZE_KEY QStringLiteral("tool_icon_size")

class GlobalSettings {
public:
    void fromJson(const QJsonObject & settings);
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

    inline Data::Type openDropPointInTabType() const { return _openDropPointInTabType; }
    inline void setOpenDropPointInTabType(const Data::Type & type) { _openDropPointInTabType = type; }

    inline int tabPosition() { return _tabPosition; }
    inline void setTabPosition(int newPositionsType) { _tabPosition = newPositionsType; }

    inline int toolIconSize() { return _toolIconSize; }
    inline void setToolIconSize(int newIconSize) { _toolIconSize = newIconSize; }

    inline float openTimeOut() { return _openTimeOut; }
    inline void setOpenTimeOut(int newTimeOut) { _openTimeOut = newTimeOut; }

    inline int colorScheme() { return _colorScheme; }
    inline void setColorScheme(int newColorScheme) { _colorScheme = newColorScheme; }

protected:
    QString _defaultDownloadPath;
    bool _showMetric;
    bool _showMetricNumero;
    bool _isAutorunned;

    bool _saveCommonTab;
    bool _openDropPointInTab;
    Data::Type _openDropPointInTabType;
    int _tabPosition;
    int _colorScheme;
    int _toolIconSize;

    float _openTimeOut;
};

#endif // GLOBAL_SETTINGS
