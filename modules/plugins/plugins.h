#ifndef PLUGINS
#define PLUGINS

#include <qdir.h>
#include <qapplication.h>
#include <qpluginloader.h>

#include "modules/core/misc/logger.h"
#include "web_dialog_interface.h"

class Plugins {
public:
    static bool loadWebDialog(WebDialogInterface *& wdi);
};

#endif // PLUGINS
