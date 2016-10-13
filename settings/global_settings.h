#ifndef GLOBAL_SETTINGS
#define GLOBAL_SETTINGS

#include <qstringbuilder.h>
#include <qapplication.h>
#include <qjsonobject.h>

#include "modules/core/data_sub_types.h"

#define SETTINGS_SHOW_METRICS_KEY QStringLiteral("show_metric")
#define SETTINGS_SHOW_METRICS_NUMERO_KEY QStringLiteral("show_metric_numero")
#define SETTINGS_DOWNLOAD_PATH_KEY QStringLiteral("download_path")
#define SETTINGS_AUTORUNNED_KEY QStringLiteral("autorunned")
#define SETTINGS_TAB_POSITION_KEY QStringLiteral("tab_position")
#define SETTINGS_SAVE_COMMON_TAB_KEY QStringLiteral("save_common_tab")
#define SETTINGS_OPEN_DROP_IN_TAB_KEY QStringLiteral("open_drop_point_in_tab")
#define SETTINGS_OPEN_DROP_IN_TAB_TYPE_KEY QStringLiteral("open_drop_point_in_tab_type")
#define SETTINGS_COLOR_SCHEME_KEY QStringLiteral("color_scheme")
#define SETTINGS_TOOL_ICON_SIZE_KEY QStringLiteral("tool_icon_size")
#define SETTINGS_OUTPUT_DEVICE_KEY QStringLiteral("output_device")
#define SETTINGS_SHOW_DOWNLOAD_KEY QStringLiteral("show_download")
#define SETTINGS_TRAY_SHOW_PLAYED_KEY QStringLiteral("tray_show_played")
#define SETTINGS_TRAY_SHOW_PERIOD_KEY QStringLiteral("tray_show_period")

class GlobalSettings {
public:
    enum TrayShowInfo : uint {
        tsi_none = 0,
        tsi_inform = 1, // only if main window inactive
        tsi_show = 2
    };

    void fromJson(const QJsonObject & settings);
    void toJson(QJsonObject & settings);

    inline QString defaultDownloadPath() const { return _defaultDownloadPath; }
    inline void setDefaultDownloadPath(QString newPath) { _defaultDownloadPath = newPath; }

    inline bool isShowDownloadingOnStart() const { return _showDownload; }
    inline void setShowDownloadingOnStart(bool show) { _showDownload = show; }

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

    inline Core::DataSubType openDropPointInTabType() const { return _openDropPointInTabType; }
    inline void setOpenDropPointInTabType(const Core::DataSubType & type) { _openDropPointInTabType = type; }

    inline int tabPosition() { return _tabPosition; }
    inline void setTabPosition(int newPositionsType) { _tabPosition = newPositionsType; }

    inline int toolIconSize() { return _toolIconSize; }
    inline void setToolIconSize(int newIconSize) { _toolIconSize = newIconSize; }

    inline int colorScheme() { return _colorScheme; }
    inline void setColorScheme(int newColorScheme) { _colorScheme = newColorScheme; }

    inline QString outputDevice() { return _output_device; }
    inline void setOutputDevice(const QString & newDevice) { _output_device = newDevice; }

    inline int showPlayedInTrayMessage() { return _tray_show_played; }
    inline void setShowPlayedInTrayMessage(int new_state) { _tray_show_played = new_state; }

    inline int showTrayMessageTime() { return _tray_show_period; }
    inline void setShowTrayMessageTime(int new_period) { _tray_show_period = new_period; }
protected:
    QString _output_device;
    QString _defaultDownloadPath;
    bool _showDownload;
    bool _showMetric;
    bool _showMetricNumero;
    bool _isAutorunned;

    bool _saveCommonTab;
    bool _openDropPointInTab;
    Core::DataSubType _openDropPointInTabType;
    int _tabPosition;
    int _colorScheme;
    int _toolIconSize;

    int _tray_show_played;
    int _tray_show_period;
};

#endif // GLOBAL_SETTINGS
