#pragma once

#include <DCPlugin.h>

class PluginClass final: public DCPlugin {
	Q_OBJECT
public:
	explicit PluginClass(DCPluginManager &pluginManager);

protected:
	void prepareToUnload() override;

private:

};
