#pragma once

#include <QProcess>
#include "DCPlugin.h"

class DCExternalPlugin: public DCPlugin {
	Q_OBJECT
public:
	explicit DCExternalPlugin(const QString& command, const QString& workDir, DCPluginManager &pluginManager);

protected:
	void prepareToUnload() override;
	void handleMessage(const QString &sender, const QString &tag, const QVariant &data) override;

private slots:
	void readyReadStandardOutput();
	void readyReadStandardError();
	void errorOccurred(QProcess::ProcessError error);
	void finished(int exitCode, QProcess::ExitStatus exitStatus);

private:
	QString m_command;
	QProcess *m_process;
	QByteArray m_dataBuffer;
	QByteArray m_logBuffer;

};
