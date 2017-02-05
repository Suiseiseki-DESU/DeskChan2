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
	m_tabWidget->addTab(new QWidget(m_tabWidget), "Skin");
	m_tabWidget->addTab(new QWidget(m_tabWidget), "Plugins");
	m_tabWidget->addTab(new QWidget(m_tabWidget), "Alternatives");
}
