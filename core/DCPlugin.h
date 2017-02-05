#pragma once

#include <functional>
#include <QMap>
#include <QObject>
#include <QLibrary>
#include "DCCommon.h"

class DCPluginManager;

class DC_CORE_DECL DCPlugin: public QObject {
	Q_OBJECT
public:
	typedef std::function<void(const QString&, const QString&, const QVariant&)> MessageCallback;

	explicit DCPlugin(DCPluginManager &pluginManager);
	~DCPlugin() override;
	const QString &id() const { return m_id; }
	bool isInitialized() const;
	QLibrary *sharedLibrary() const { return m_sharedLibrary; }

protected:
	void initialize(const QString &id);
	DCPluginManager &pluginManager() const { return m_pluginManager; }
	void subscribe(const QString &tag);
	void subscribe(const QString &tag, const MessageCallback& callback);
	void unsubscribe(const QString &tag);
	virtual void prepareToUnload() {}
	virtual void handleMessage(const QString &sender, const QString &tag, const QVariant &data);
	void sendMessage(const QString &tag, const QVariant &data);
	void sendMessage(const QString &tag, const QVariant &data, const MessageCallback& responseCallback);
	void sendMessage(const QString &tag, const QVariant &data,
					 const std::function<void(const QVariant&)> &responseCallback);
	int makeSeqValue();

private:
	friend class DCPluginManager;
	friend class DCCorePlugin;

	DCPluginManager &m_pluginManager;
	QString m_id;
	QLibrary *m_sharedLibrary;
	QMap<QString, MessageCallback> m_messageCallbacks;
	QMap<int, MessageCallback> m_responseCallbacks;
	int m_seq = 0;

};

#define DC_LOADABLE_PLUGIN_CLASS(className)\
	extern "C" { Q_DECL_EXPORT DCPlugin *DeskChan_CreateLoadablePluginInstance(DCPluginManager &pluginManager) {\
		return new className(pluginManager);\
	} }
#define DC_LOADABLE_PLUGIN_FUNC_NAME "DeskChan_CreateLoadablePluginInstance"
