#pragma once

#include <QDialog>
#include <QTabWidget>
#include <QListWidget>
#include <QTreeWidget>
#include <QLineEdit>
#include <QTextEdit>
#include "Plugin.h"

class CharacterWidget;

class SettingsDialog: public QDialog {
	Q_OBJECT
public:
	explicit SettingsDialog(CharacterWidget *parent);

private:
	CharacterWidget *m_characterWidget;
	PluginClass *m_plugin;
	QTabWidget *m_tabWidget;
	QListWidget *m_pluginsListWidget;
	QTreeWidget *m_alternativesTreeWidget;
	QLineEdit *m_sendMessageTagEdit;
	QTextEdit *m_sendMessageDataEdit;
	void initUI();

};
