#include <QDebug>
#include <QCoreApplication>
#include <QVariantMap>
#include <QDir>
#include <DCPlugin.h>
#include <DCExternalPlugin.h>
#include "Plugin.h"

PluginClass::PluginClass(DCPluginManager &pluginManager): DCPlugin(pluginManager) {
    initialize("python_support");
    subscribe("python_support:load-plugin", [&pluginManager](const QString &sender, const QString &tag, const QVariant &data) {
        QString scriptFileName = data.toString();
        QString scriptWorkDir = QFileInfo(scriptFileName).absoluteDir().absolutePath();
#ifdef __WIN32
        QString python = QCoreApplication::applicationDirPath() + QDir::separator() + "plugins" +
                QDir::separator() + "python_support" + QDir::separator() + "win32" + QDir::separator() +
                "bin" + QDir::separator() + "python";
#else
        QString python = "python3";
#endif
        QString command = QString("\"%0\" \"%1\"").arg(python).arg(QFileInfo(scriptFileName).fileName());
        new DCExternalPlugin(command, scriptWorkDir, pluginManager);
    });
    sendMessage("core:register-plugin-loader", QVariantMap({
                                                                   {"pattern", "%0.py"},
                                                                   {"tag", "python_support:load-plugin"}
                                                           }));
}

void PluginClass::prepareToUnload() {
}

DC_LOADABLE_PLUGIN_CLASS(PluginClass)
