#pragma once

#include <cmath>
#include <Python.h>
#include "DCPlugin.h"

class PythonPlugin;

class PluginClass: public DCPlugin {
	Q_OBJECT
public:
	explicit PluginClass(DCPluginManager &pluginManager);
	void loadPlugin(const QString &fileName);

protected:
	void prepareToUnload() override;

private:
	bool m_initialized = false;
	QMap<QString, PythonPlugin*> m_plugins;

};
