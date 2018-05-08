#ifndef GLOBAL_SETTINGS
#define GLOBAL_SETTINGS

#include <qstringbuilder.h>
#include <qapplication.h>
#include <qjsonobject.h>

#include "modules/core/data_sub_types.h"

#define SETTINGS_PLAYER_DRIVER_KEY QLatin1String("player_driver")
#define SETTINGS_SHOW_METRICS_KEY QLatin1String("show_metric")
#define SETTINGS_SHOW_METRICS_NUMERO_KEY QLatin1String("show_metric_numero")
#define SETTINGS_DOWNLOAD_PATH_KEY QLatin1String("download_path")
#define SETTINGS_AUTORUNNED_KEY QLatin1String("autorunned")
#define SETTINGS_TAB_POSITION_KEY QLatin1String("tab_position")
#define SETTINGS_SAVE_COMMON_TAB_KEY QLatin1String("save_common_tab")
#define SETTINGS_CLEAR_COMMON_TAB_KEY QLatin1String("clear_common_tab")
#define SETTINGS_OPEN_DROP_IN_TAB_KEY QLatin1String("open_drop_point_in_tab")
#define SETTINGS_OPEN_DROP_IN_TAB_TYPE_KEY QLatin1String("open_drop_point_in_tab_type")
#define SETTINGS_COLOR_SCHEME_KEY QLatin1String("color_scheme")
#define SETTINGS_TOOL_ICON_SIZE_KEY QLatin1String("tool_icon_size")
#define SETTINGS_OUTPUT_DEVICE_KEY QLatin1String("output_device")
#define SETTINGS_SHOW_DOWNLOAD_KEY QLatin1String("show_download")
#define SETTINGS_TRAY_SHOW_PLAYED_KEY QLatin1String("tray_show_played")
#define SETTINGS_TRAY_SHOW_PERIOD_KEY QLatin1String("tray_show_period")

#define SETTINGS_DEFAULT_DOWNLOAD_PATH QCoreApplication::applicationDirPath() % QLatin1String("/downloads/")

class GlobalSettings {
public:
    enum TrayShowInfo : uint {
        tsi_none = 0,
        tsi_inform = 1, // only if main window inactive
        tsi_show = 2
    };

    void fromJson(const QJsonObject & settings);
    void toJson(QJsonObject & settings);

    inline QString defaultDownloadPath() const { return _default_download_path; }
    inline void setDefaultDownloadPath(const QString & new_path) { _default_download_path = new_path; }

    inline bool isShowDownloadingOnStart() const { return _show_download; }
    inline void setShowDownloadingOnStart(const bool & show) { _show_download = show; }

    inline bool isAutorunned() const { return _is_autorunned; }
    inline void setAutorun(const bool & is_autorunned) { _is_autorunned = is_autorunned; }

    inline bool isMetricShow() const { return _show_metric_marks; }
    inline void setMetricShow(const bool & show) { _show_metric_marks = show; }

    inline bool isMetricNumero() const { return _show_metric_numero; }
    inline void setMetricNumeroShow(const bool & show) { _show_metric_numero = show; }

    inline bool isSaveCommonTab() const { return _save_common_tab; }
    inline void setSaveCommonTab(const bool & save) { _save_common_tab = save; }

    inline bool isClearCommonTab() const { return _clear_common_tab_on_exit; }
    inline void setClearCommonTab(const bool & clear) { _clear_common_tab_on_exit = clear; }

    inline bool isOpenDropPointInTab() const { return _open_drop_point_in_tab; }
    inline void setOpenDropPointInTab(const bool & open) { _open_drop_point_in_tab = open; }

    inline Core::DataSubType openDropPointInTabType() const { return _open_drop_point_in_tab_type; }
    inline void setOpenDropPointInTabType(const Core::DataSubType & type) { _open_drop_point_in_tab_type = type; }

    inline int tabPosition() { return _tab_position; }
    inline void setTabPosition(const int & new_position_type) { _tab_position = new_position_type; }

    inline int toolIconSize() { return _tool_icon_size; }
    inline void setToolIconSize(const int & new_icon_size) { _tool_icon_size = new_icon_size; }

    inline int colorScheme() { return _color_scheme; }
    inline void setColorScheme(const int & new_color_scheme) { _color_scheme = new_color_scheme; }

    inline QString outputDevice() { return _output_device; }
    inline void setOutputDevice(const QString & new_device) { _output_device = new_device; }

    inline int showPlayedInTrayMessage() { return _tray_show_played; }
    inline void setShowPlayedInTrayMessage(const int & new_state) { _tray_show_played = new_state; }

    inline int showTrayMessageTime() { return _tray_show_period; }
    inline void setShowTrayMessageTime(const int & new_period) { _tray_show_period = new_period; }

    inline quint8 playerDriver() { return _player_driver; }
    inline void setPlayerDriver(const quint8 & new_player_driver) { _player_driver = new_player_driver; }
protected:
    QString _output_device;
    QString _default_download_path;
    bool _show_download;
    bool _show_metric_marks;
    bool _show_metric_numero;
    bool _is_autorunned;

    bool _save_common_tab;
    bool _clear_common_tab_on_exit;
    bool _open_drop_point_in_tab;
    Core::DataSubType _open_drop_point_in_tab_type;
    int _tab_position;
    int _color_scheme;
    int _tool_icon_size;

    int _tray_show_played;
    int _tray_show_period;

    quint8 _player_driver;
};

#endif // GLOBAL_SETTINGS
