#include "PythonPlugin.h"

PythonPlugin::PythonPlugin(const QString &fileName, DCPluginManager &pluginManager): DCPlugin(pluginManager) {
}

void PythonPlugin::prepareToUnload() {
}
