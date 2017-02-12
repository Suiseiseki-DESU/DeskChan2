#include "Plugin.h"
#include "QMLPluginProxy.h"

QMLPluginProxy::QMLPluginProxy(PluginClass *plugin, QObject *parent): QObject(parent), m_plugin(plugin) {
}

void QMLPluginProxy::sendMessage(const QString &tag, const QVariant &data) {
	m_plugin->sendMessage(tag, data);
}
