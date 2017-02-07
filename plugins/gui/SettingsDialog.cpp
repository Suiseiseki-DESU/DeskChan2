#include <QGridLayout>
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
	m_tabWidget->addTab(tab, "Alternatives");
}
