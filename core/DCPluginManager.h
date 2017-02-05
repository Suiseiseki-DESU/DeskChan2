#pragma once

#include <QObject>
#include <QMap>
#include <QSet>
#include "DCCommon.h"

class DCPlugin;
class DCCorePlugin;

class DC_CORE_DECL DCPluginManager final: public QObject {
	Q_OBJECT
public:
	explicit DCPluginManager();
	~DCPluginManager() override;
	void initialize();
	const QList<DCPlugin*> plugins() const;
	DCPlugin *plugin(const QString &id) const;

private:
	friend class DCPlugin;

	QMap<QString, DCPlugin*> m_plugins;
	QMap<QString, QSet<DCPlugin*>> m_subscriptions;
	DCCorePlugin *m_corePlugin;

	QString registerPlugin(DCPlugin *plugin, const QString &id);
	void unregisterPlugin(DCPlugin *plugin);
	void addSubscription(DCPlugin *plugin, const QString &tag);
	void removeSubscription(DCPlugin *plugin, const QString &tag);
	void sendMessage(DCPlugin *sender, const QString &tag, const QVariant &data);

};
