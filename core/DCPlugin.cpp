#include <QVariant>
#include "DCPluginManager.h"
#include "DCPlugin.h"

DCPlugin::DCPlugin(DCPluginManager &pluginManager):
		QObject(nullptr), m_pluginManager(pluginManager), m_sharedLibrary(nullptr) {
}

DCPlugin::~DCPlugin() {
	if (isInitialized()) {
		m_pluginManager.unregisterPlugin(this);
	}
}

bool DCPlugin::isInitialized() const {
	return m_id.size() > 0;
}

void DCPlugin::initialize(const QString &id) {
	Q_ASSERT(!isInitialized());
	m_id = m_pluginManager.registerPlugin(this, id);
}

void DCPlugin::subscribe(const QString &tag) {
	Q_ASSERT(isInitialized());
	m_pluginManager.addSubscription(this, tag);
}

void DCPlugin::subscribe(const QString &tag, const MessageCallback &callback) {
	m_messageCallbacks[tag].append(callback);
	subscribe(tag);
}

void DCPlugin::unsubscribe(const QString &tag) {
	Q_ASSERT(isInitialized());
	m_pluginManager.removeSubscription(this, tag);
	m_messageCallbacks.remove(tag);
}

void DCPlugin::handleMessage(const QString &sender, const QString &tag, const QVariant &data) {
	auto it = m_messageCallbacks.find(tag);
	if (it != m_messageCallbacks.end()) {
		for (auto&& callback : *it) {
			callback(sender, tag, data);
		}
	}
	int seq = data.toMap()["seq"].toInt();
	if (seq != 0) {
		auto it2 = m_responseCallbacks.find(seq);
		if (it2 != m_responseCallbacks.end()) {
			(*it2)(sender, tag, data);
		}
	}
}

void DCPlugin::sendMessage(const QString &tag, const QVariant &data) {
	m_pluginManager.sendMessage(this, tag, data);
}

void DCPlugin::sendMessage(const QString &tag, const QVariant &data, const MessageCallback &responseCallback) {
	int seq = makeSeqValue();
	QMap<QString, QVariant> m = data.toMap();
	m["seq"] = seq;
	m_responseCallbacks[seq] = responseCallback;
	sendMessage(tag, m);
}

void DCPlugin::sendMessage(const QString &tag, const QVariant &data,
						   const std::function<void(const QVariant&)> &responseCallback) {
	sendMessage(tag, data, [responseCallback](const QString&, const QString&, const QVariant &d) {
		responseCallback(d);
	});
}

int DCPlugin::makeSeqValue() {
	return ++m_seq;
}
