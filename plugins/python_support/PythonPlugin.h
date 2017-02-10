#pragma once

#include "DCPlugin.h"

class PythonPlugin: public DCPlugin {
public:
	PythonPlugin(const QString &fileName, DCPluginManager &pluginManager);
	void prepareToUnload() override;

private:

};
