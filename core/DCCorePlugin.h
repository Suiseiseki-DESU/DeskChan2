#pragma once

#include <QMap>
#include <QList>
#include "DCCommon.h"
#include "DCPlugin.h"

class DC_CORE_DECL DCCorePlugin final: public DCPlugin {
	Q_OBJECT
public:
	explicit DCCorePlugin(DCPluginManager &pluginManager);
	~DCCorePlugin() override;
	bool loadPlugin(const QString &name);
	QString pluginDir(const DCPlugin *plugin) const;
	QString pluginDir(const QString& id) const;
	QString pluginDataDir(const DCPlugin *plugin) const;
	QString pluginDataDir(const QString &id) const;

private:
	struct AlternativeInfo {
		int priority;
		QString tag;
		QString pluginId;

		AlternativeInfo(int priority, const QString &tag, const QString &pluginId):
				priority(priority), tag(tag), pluginId(pluginId) {
		}
	};
	struct PluginLoaderInfo {
		QString pattern;
		QString tag;
		QString pluginId;

		PluginLoaderInfo(const QString &pattern, const QString &tag, const QString &pluginId):
				pattern(pattern), tag(tag), pluginId(pluginId) {
		}
	};
	QMap<QString, QList<AlternativeInfo>> m_alternatives;
	QList<PluginLoaderInfo> m_pluginLoaders;

	void initCallbacks();

};
