#pragma once

#include <DCPlugin.h>

class CharacterWidget;
class SettingsDialog;

class PluginClass final: public DCPlugin {
	Q_OBJECT
public:
	explicit PluginClass(DCPluginManager &pluginManager);
	using DCPlugin::sendMessage;
	using DCPlugin::subscribe;

protected:
	void prepareToUnload() override;

private:
	friend class CharacterWidget;
	friend class SettingsDialog;

	CharacterWidget *m_characterWidget;
	SettingsDialog *m_settingsDialog;

};
