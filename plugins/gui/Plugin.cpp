#include <QApplication>
#include <QWidget>
#include <DCPlugin.h>
#include "CharacterWidget.h"
#include "SettingsDialog.h"
#include "Plugin.h"

PluginClass::PluginClass(DCPluginManager &pluginManager): DCPlugin(pluginManager) {
	initialize("gui");
	subscribe("core-events:plugin-unload", [this](const QString&, const QString&, const QVariant &data) {
		const QString id = data.toString();
		m_pluginsList.remove(data.toString());
		emit pluginUnloaded(id);
	});
	subscribe("core-events:plugin-load", [this](const QString&, const QString&, const QVariant &data) {
		const QString id = data.toString();
		m_pluginsList.insert(id);
		emit pluginLoaded(id);
	});
	m_characterWidget = new CharacterWidget(this);
	m_settingsDialog = new SettingsDialog(m_characterWidget);
}

void PluginClass::prepareToUnload() {
	if (m_settingsDialog) {
		delete m_settingsDialog;
	}
	if (m_characterWidget) {
		delete m_characterWidget;
	}
}

DC_LOADABLE_PLUGIN_CLASS(PluginClass)
