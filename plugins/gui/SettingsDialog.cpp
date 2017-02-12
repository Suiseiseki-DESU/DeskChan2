#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QJsonDocument>
#include <QMessageBox>
#include <QApplication>

#ifdef QT_QUICKWIDGETS_LIB
#include <QQuickWidget>
#include <QQmlEngine>
#include <QQmlContext>
#include "QMLPluginProxy.h"
#endif

#include "Plugin.h"
#include "CharacterWidget.h"
#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(CharacterWidget *parent):
		QDialog(parent), m_characterWidget(parent), m_plugin(parent->plugin()) {
	initUI();
#ifdef QT_QUICKWIDGETS_LIB
	m_plugin->subscribe("gui:register-config-ui", [this](const QString &sender, const QString &tag, const QVariant &data) {
		QVariantMap m = data.toMap();
		QString tabName = m["name"].toString();
		if (tabName.isEmpty()) {
			tabName = sender;
		}
		QString afterTabName = m["after"].toString();
		QString sceneFileName = m["sceneFile"].toString();
		QQuickWidget *widget = new QQuickWidget(m_tabWidget);
		widget->engine()->addImportPath(QApplication::applicationDirPath());
		widget->engine()->addPluginPath(QApplication::applicationDirPath());
		widget->rootContext()->setContextProperty("deskChan", new QMLPluginProxy(m_plugin, widget));
		widget->setSource(QUrl::fromLocalFile(sceneFileName));
		m_extraTabs[sender].append(widget);
		int index;
		for (index = 0; index < (m_tabWidget->count() - 1); ++index) {
			if (m_tabWidget->tabText(index) == afterTabName) {
				index++;
				break;
			}
		}
		m_tabWidget->insertTab(index, widget, tabName);
	});
	connect(m_plugin, &PluginClass::pluginLoaded, [this](const QString &pluginId) {
		auto it = m_extraTabs.find(pluginId);
		if (it != m_extraTabs.end()) {
			for (auto it2 = it->begin(); it2 != it->end(); ++it2) {
				m_tabWidget->removeTab(m_tabWidget->indexOf(*it2));
				delete *it2;
			}
			m_extraTabs.erase(it);
		}
	});
#endif
}

SettingsDialog::~SettingsDialog() {
#ifdef QT_QUICKWIDGETS_LIB
	m_plugin->unsubscribe("gui:register-config-ui");
#endif
}

void SettingsDialog::initUI() {
	resize(400, 300);
	QGridLayout *rootLayout = new QGridLayout(this);
	m_tabWidget = new QTabWidget(this);
	rootLayout->addWidget(m_tabWidget);
	QWidget *tab = new QWidget(m_tabWidget);
	m_tabWidget->addTab(tab, "Appearance");
	tab = new QWidget(m_tabWidget);
	QGridLayout *tabLayout = new QGridLayout(tab);
	m_pluginsListWidget = new QListWidget(tab);
	m_pluginsListWidget->addItems(m_plugin->pluginsList().toList());
	tabLayout->addWidget(m_pluginsListWidget, 0, 0);
	m_tabWidget->addTab(tab, "Plugins");
	tab = new QWidget(m_tabWidget);
	tabLayout = new QGridLayout(tab);
	m_alternativesTreeWidget = new QTreeWidget(tab);
	tabLayout->addWidget(m_alternativesTreeWidget, 0, 0);
	m_alternativesTreeWidget->setColumnCount(3);
	m_alternativesTreeWidget->setHeaderLabels({"Tag", "Priority", "Plugin"});
	m_plugin->sendMessage("core:query-alternatives", QVariant(), [this](const QVariant &data) {
		QVariantMap m = data.toMap()["alternatives"].toMap();
		for (auto it = m.begin(); it != m.end(); ++it) {
			QTreeWidgetItem *groupItem = new QTreeWidgetItem({it.key()});
			QVariantList l = it.value().toList();
			for (auto it2 = l.begin(); it2 != l.end(); ++it2) {
				QVariantMap m2 = it2->toMap();
				QString tag = m2["tag"].toString();
				int priority = m2["priority"].toInt();
				QString pluginId = m2["pluginId"].toString();
				QTreeWidgetItem *item = new QTreeWidgetItem({tag, QString::number(priority), pluginId});
				groupItem->addChild(item);
			}
			m_alternativesTreeWidget->addTopLevelItem(groupItem);
		}
	});
	m_tabWidget->addTab(tab, "Alternatives");
	tab = new QWidget(m_tabWidget);
	tabLayout = new QGridLayout(tab);
	m_sendMessageTagEdit = new QLineEdit("dc:say", tab);
	tabLayout->addWidget(m_sendMessageTagEdit, 0, 0);
	QPushButton *sendMessageButton = new QPushButton("Send", tab);
	tabLayout->addWidget(sendMessageButton, 0, 1);
	m_sendMessageDataEdit = new QTextEdit("{ \"text\": \"Test\" }", tab);
	tabLayout->addWidget(m_sendMessageDataEdit, 1, 0, 1, 2);
	connect(sendMessageButton, &QPushButton::clicked, [this]() {
		QJsonParseError parseError;
		QJsonDocument doc = QJsonDocument::fromJson(m_sendMessageDataEdit->toPlainText().toUtf8(), &parseError);
		if (parseError.error != QJsonParseError::NoError) {
			QMessageBox::warning(this, "DeskChan", parseError.errorString(), QMessageBox::Ok, QMessageBox::Ok);
			return;
		}
		m_plugin->sendMessage(m_sendMessageTagEdit->text(), doc.toVariant());
	});
	connect(m_plugin, &PluginClass::pluginLoaded, [this](const QString &pluginId) {
		m_pluginsListWidget->addItem(pluginId);
	});
	connect(m_plugin, &PluginClass::pluginUnloaded, [this](const QString &pluginId) {
		for (auto i = m_pluginsListWidget->count() - 1; i >= 0; --i) {
			if (m_pluginsListWidget->item(i)->text() == pluginId) {
				delete m_pluginsListWidget->item(i);
			}
		}
	});
	m_tabWidget->addTab(tab, "Debug");
}
