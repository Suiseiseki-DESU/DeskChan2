#include <QDebug>
#include "DCPlugin.h"
#include "DCCorePlugin.h"
#include "DCPluginManager.h"

DCPluginManager::DCPluginManager(): QObject(nullptr), m_corePlugin(nullptr) {
}

DCPluginManager::~DCPluginManager() {
	if (m_corePlugin) {
		for (auto &&plugin: m_plugins) {
			plugin->prepareToUnload();
		}
		while (m_plugins.size() > 0) {
			auto it = m_plugins.begin();
			if ((*it == m_corePlugin) && (m_plugins.size() > 1)) {
				++it;
			} else {
				m_subscriptions.clear();
				m_corePlugin = nullptr;
			}
			QLibrary *sharedLibrary = m_plugins.first()->sharedLibrary();
			delete *it;
			if (sharedLibrary) {
				sharedLibrary->unload();
				delete sharedLibrary;
			}
		}
	}
}

void DCPluginManager::initialize() {
	m_corePlugin = new DCCorePlugin(*this);
}

QString DCPluginManager::registerPlugin(DCPlugin *plugin, const QString &id) {
	QString newId = id;
	int i = 0;
	while (m_plugins.contains(newId)) {
		newId = QString("%0_%1").arg(id).arg(i++);
	}
	m_plugins.insert(newId, plugin);
	qDebug() << "Registered plugin" << newId;
	if (m_corePlugin) {
		sendMessage(m_corePlugin, "core-events:plugin-load", newId);
	}
	return newId;
}

void DCPluginManager::unregisterPlugin(DCPlugin *plugin) {
	qDebug() << "Unregistered plugin" << plugin->id();
	for (auto&& it : m_subscriptions) {
		it.remove(plugin);
	}
	if (plugin->isInitialized()) {
		m_plugins.remove(plugin->id());
	}
	if (m_corePlugin) {
		sendMessage(m_corePlugin, "core-events:plugin-unload", plugin->id());
	}
}

void DCPluginManager::addSubscription(DCPlugin *plugin, const QString &tag) {
	auto it = m_subscriptions.find(tag);
	if (it == m_subscriptions.end()) {
		it = m_subscriptions.insert(tag, QSet<DCPlugin*>());
	}
	it->insert(plugin);
	if (tag == "core-events:plugin-load") {
		for (auto&& anotherPlugin: m_plugins) {
			plugin->handleMessage("core", "core-events:plugin-load", anotherPlugin->id());
		}
	}
}

void DCPluginManager::removeSubscription(DCPlugin *plugin, const QString &tag) {
	auto it = m_subscriptions.find(tag);
	if (it != m_subscriptions.end()) {
		it->remove(plugin);
		if (it->size() == 0) {
			m_subscriptions.erase(it);
		}
	}
}

const QList<DCPlugin*> DCPluginManager::plugins() const {
	return m_plugins.values();
}

DCPlugin* DCPluginManager::plugin(const QString &id) const {
	auto it = m_plugins.find(id);
	return (it != m_plugins.end()) ? *it : nullptr;
}

void DCPluginManager::sendMessage(DCPlugin *sender, const QString &tag, const QVariant &data) {
	auto it1 = m_plugins.find(tag);
	if (it1 != m_plugins.end()) {
		(*it1)->handleMessage(sender->id(), tag, data);
	}
	auto it2 = m_subscriptions.find(tag);
	if (it2 != m_subscriptions.end()) {
		for (auto&& plugin : *it2) {
			plugin->handleMessage(sender->id(), tag, data);
		}
	}
}
