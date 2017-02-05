#pragma once

#include <DCPlugin.h>

class CharacterWidget;

class PluginClass final: public DCPlugin {
	Q_OBJECT
public:
	explicit PluginClass(DCPluginManager &pluginManager);
	using DCPlugin::sendMessage;

protected:
	void prepareToUnload() override;

private:
	friend class CharacterWidget;

	CharacterWidget *m_characterWidget;

};
