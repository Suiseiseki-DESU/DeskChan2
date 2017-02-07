#pragma once

#include <QSet>
#include <DCPlugin.h>

class CharacterWidget;
class SettingsDialog;

class PluginClass final: public DCPlugin {
	Q_OBJECT
public:
	explicit PluginClass(DCPluginManager &pluginManager);
	const QSet<QString> &pluginsList() const { return m_pluginsList; }
	using DCPlugin::sendMessage;
	using DCPlugin::subscribe;

signals:
	void pluginLoaded(const QString &id);
	void pluginUnloaded(const QString &id);

protected:
	void prepareToUnload() override;

private:
	friend class CharacterWidget;
	friend class SettingsDialog;

	CharacterWidget *m_characterWidget;
	SettingsDialog *m_settingsDialog;
	QSet<QString> m_pluginsList;

};
