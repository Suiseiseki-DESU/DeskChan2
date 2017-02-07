#include <QDebug>
#include <QGridLayout>
#include <QPushButton>
#include <QJsonDocument>
#include <QMessageBox>
#include "Plugin.h"
#include "CharacterWidget.h"
#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(CharacterWidget *parent):
		QDialog(parent), m_characterWidget(parent), m_plugin(parent->plugin()) {
	initUI();
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
	m_tabWidget->addTab(tab, "Debug");
}
