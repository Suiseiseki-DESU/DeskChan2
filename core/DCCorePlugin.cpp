#include <QDebug>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QVariant>
#include "DCPluginManager.h"
#include "DCCorePlugin.h"

DCCorePlugin::DCCorePlugin(DCPluginManager &pluginManager): DCPlugin(pluginManager) {
	initialize("core");
	initCallbacks();
	if (QCoreApplication::instance()->property("guiSupportAvailable").toBool()) {
		loadPlugin("gui");
	}
}

DCCorePlugin::~DCCorePlugin() {
}

bool DCCorePlugin::loadPlugin(const QString &name) const {
	QString pluginDirName = pluginDir(name);
	if (pluginDirName.isEmpty()) {
		return false;
	}
	static const QStringList librarySuffixes = {
#if defined(_WIN32) || defined(WIN32)
			".dll", ".DLL",
#elif defined(__APPLE__) && defined(TARGET_OS_MAC)
			".dylib", ".so",
#elif defined(__linux__)
			".so",
#else
#error "Unsupported OS"
#endif
			"\\/" // Invalid file name suffix
	};
	QString pluginLibraryName;
	for (auto& suffix : librarySuffixes) {
		pluginLibraryName = pluginDirName + QDir::separator() + name + suffix;
		if (QFileInfo(pluginLibraryName).isReadable()) break;
		pluginLibraryName = pluginDirName + QDir::separator() + "lib" + name + suffix;
		if (QFileInfo(pluginLibraryName).isReadable()) break;
	}
	if (QLibrary::isLibrary(pluginLibraryName)) {
		QFileInfo libraryFileInfo(pluginLibraryName);
		QLibrary *library = new QLibrary(libraryFileInfo.dir().absoluteFilePath(libraryFileInfo.completeBaseName()));
		library->setLoadHints(QLibrary::ResolveAllSymbolsHint);
		if (library->load()) {
			auto createPluginObjectFunc = (DCPlugin *(*)(DCPluginManager &)) library->resolve(
					DC_LOADABLE_PLUGIN_FUNC_NAME);
			if (createPluginObjectFunc) {
				DCPlugin *plugin = createPluginObjectFunc(pluginManager());
				Q_ASSERT(plugin != nullptr);
				plugin->m_sharedLibrary = library;
				return true;
			}
			library->unload();
		}
		qWarning() << library->errorString();
		delete library;
	}
	return false;
}

QString DCCorePlugin::pluginDir(const DCPlugin *plugin) const {
	return pluginDir(plugin->id());
}

QString DCCorePlugin::pluginDir(const QString &id) const {
	QString baseDir = QCoreApplication::applicationDirPath();
	QString dir = baseDir + QDir::separator() + "plugins" + QDir::separator() + id;
	if (QFileInfo(dir).isDir()) {
		return dir;
	}
	return QString();
}

QString DCCorePlugin::pluginDataDir(const DCPlugin *plugin) const {
	return pluginDataDir(plugin->id());
}

QString DCCorePlugin::pluginDataDir(const QString &id) const {
	QString baseDirName = QCoreApplication::applicationDirPath();
	QString dataDirName = baseDirName + QDir::separator() + "data";
	if (!QFileInfo(dataDirName).isDir()) {
		baseDirName = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
		dataDirName = baseDirName + QDir::separator() + "data";
	}
	QString pluginDataDirName = dataDirName + QDir::separator() + id;
	QDir dir(pluginDataDirName);
	if (!dir.exists()) {
		dir.mkpath(".");
		qDebug() << "Directory" << pluginDataDirName << "created";
	}
	return pluginDataDirName;
}

void DCCorePlugin::initCallbacks() {
	subscribe("core:quit", [](const QString &sender, const QString &tag, const QVariant &data) {
		qDebug() << "Plugin" << sender << "requested application termination";
		QCoreApplication::quit();
	});
	subscribe("core:load-plugin", [this](const QString &sender, const QString &tag, const QVariant &data) {
		loadPlugin(data.toString());
	});
	subscribe("core:query-plugin-data-dir", [this](const QString &sender, const QString &tag, const QVariant &data) {
		int seq = data.toMap()["seq"].toInt();
		QString dirName = pluginDataDir(sender);
		sendMessage(sender, QMap<QString, QVariant>({{"seq", seq}, {"path", dirName}}));
	});
	subscribe("core:query-plugin-dir", [this](const QString &sender, const QString &tag, const QVariant &data) {
		int seq = data.toMap()["seq"].toInt();
		QString dirName = pluginDir(sender);
		sendMessage(sender, QMap<QString, QVariant>({{"seq", seq}, {"path", dirName}}));
	});
	subscribe("core:register-alternative", [this](const QString &sender, const QString &tag, const QVariant &data) {
		auto m = data.toMap();
		QString srcTag = m["srcTag"].toString();
		QString dstTag = m["dstTag"].toString();
		int priority = m["priority"].toInt();
	});
	subscribe("core:unregister-alternative", [this](const QString &sender, const QString &tag, const QVariant &data) {
		auto m = data.toMap();
		QString srcTag = m["srcTag"].toString();
		QString dstTag = m["dstTag"].toString();
		QString pluginId = data.toString();
		auto it = m_alternatives.find(srcTag);
		if (it != m_alternatives.end()) {
			//
		}
	});
	subscribe("core:change-alternative-priority", [this](const QString &sender, const QString &tag, const QVariant &data) {
		auto m = data.toMap();
		QString srcTag = m["srcTag"].toString();
		QString dstTag = m["dstTag"].toString();
		int priority = m["priority"].toInt();
	});
	subscribe("core:query-alternatives", [this](const QString &sender, const QString &tag, const QVariant &data) {
		//
	});
	subscribe("core-events:plugin-unload", [this](const QString &sender, const QString &tag, const QVariant &data) {
		QString pluginId = data.toString();
		for (auto&& altList : m_alternatives) {
			for (auto it = altList.begin(); it != altList.end(); ++it) {
				if (it->pluginId == pluginId) {
					altList.erase(it);
					break;
				}
			}
		}
	});
}
