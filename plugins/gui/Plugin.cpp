#include <QApplication>
#include <QWidget>
#include <DCPlugin.h>
#include "CharacterWidget.h"
#include "Plugin.h"

PluginClass::PluginClass(DCPluginManager &pluginManager): DCPlugin(pluginManager) {
	initialize("gui");
	m_characterWidget = new CharacterWidget(this);
}

void PluginClass::prepareToUnload() {
	delete m_characterWidget;
}

DC_LOADABLE_PLUGIN_CLASS(PluginClass)
