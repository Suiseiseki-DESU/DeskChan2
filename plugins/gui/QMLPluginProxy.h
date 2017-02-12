#pragma once

#include <QObject>
#include <QVariant>

class PluginClass;

class QMLPluginProxy: public QObject {
	Q_OBJECT
public:
	explicit QMLPluginProxy(PluginClass *plugin, QObject *parent = nullptr);
	Q_INVOKABLE void sendMessage(const QString &tag, const QVariant &data);

private:
	PluginClass *m_plugin;

};
