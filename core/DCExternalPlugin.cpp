#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "DCExternalPlugin.h"

DCExternalPlugin::DCExternalPlugin(const QString &command, const QString &workDir, DCPluginManager &pluginManager):
		DCPlugin(pluginManager), m_command(command) {
	m_process = new QProcess(this);
	connect(m_process, &QProcess::readyReadStandardOutput, this, &DCExternalPlugin::readyReadStandardOutput);
	connect(m_process, &QProcess::readyReadStandardError, this, &DCExternalPlugin::readyReadStandardError);
	connect(m_process, SIGNAL(errorOccurred(QProcess::ProcessError)),
			this, SLOT(errorOccurred(QProcess::ProcessError)));
	connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)),
			this, SLOT(finished(int, QProcess::ExitStatus)));
	m_process->setWorkingDirectory(workDir);
	m_process->start(command);
}

void DCExternalPlugin::prepareToUnload() {
	if (m_process) {
		m_process->terminate();
		if (!m_process->waitForFinished(2000)) {
			m_process->kill();
			if (!m_process->waitForFinished(2000)) {
				qWarning() << "Failed to kill process" << m_command;
			}
		}
		delete m_process;
	}
}

void DCExternalPlugin::handleMessage(const QString &sender, const QString &tag, const QVariant &data) {
	if (m_process) {
		QJsonObject root;
		root["sender"] = sender;
		root["tag"] = tag;
		root["data"] = data.toJsonValue();
		QJsonDocument doc(root);
		m_process->write(doc.toJson(QJsonDocument::Compact) + "\n");
	}
}

void DCExternalPlugin::readyReadStandardOutput() {
	m_dataBuffer += m_process->readAll();
	int lineBegin = 0, lineEnd;
	while ((lineEnd = m_dataBuffer.indexOf('\n', lineBegin)) >= 0) {
		QByteArray line = m_dataBuffer.mid(lineBegin, lineEnd - lineBegin);
		QJsonParseError error;
		QJsonDocument doc = QJsonDocument::fromJson(line, &error);
		if (error.error == QJsonParseError::NoError) {
			QJsonObject root = doc.object();
			if (isInitialized()) {
				QString tag = root["tag"].toString();
				QVariant data = root["data"].toVariant();
				if (tag.size() > 0) {
					sendMessage(tag, data);
				} else {
					QString subscribeTagName = root["subscribe"].toString();
					QString unsubscribeTagName = root["unsubscribe"].toString();
					if (subscribeTagName.size() > 0) {
						subscribe(subscribeTagName);
					}
					if (unsubscribeTagName.size() > 0) {
						unsubscribe(unsubscribeTagName);
					}
				}
			} else {
				QString id = root["deskchan-plugin-id"].toString();
				if (id.size() > 0) {
					initialize(id);
				} else {
					qWarning() << "Process" << m_command << "attempted to register plugin with empty id";
				}
			}
		} else {
			qWarning() << "Failed to parse JSON:" << error.errorString();
		}
		lineBegin = lineEnd + 1;
	}
	m_dataBuffer.remove(0, lineBegin);
}

void DCExternalPlugin::readyReadStandardError() {
	m_logBuffer += m_process->readAll();
	int lineBegin = 0, lineEnd;
	while ((lineEnd = m_logBuffer.indexOf('\n', lineBegin)) >= 0) {
		QByteArray line = m_logBuffer.mid(lineBegin, lineEnd - lineBegin);
		qDebug() << line;
		lineBegin = lineEnd + 1;
	}
	m_logBuffer.remove(0, lineBegin);
}

void DCExternalPlugin::errorOccurred(QProcess::ProcessError error) {
	if (error == QProcess::FailedToStart) {
		qDebug() << "Failed to start process" << m_command << ":" << m_process->errorString();
		m_process->deleteLater();
		m_process = nullptr;
		deleteLater();
	}
}

void DCExternalPlugin::finished(int exitCode, QProcess::ExitStatus exitStatus) {
	qDebug() << "Process" << m_command << ((exitStatus == QProcess::NormalExit) ? "exited" : "crashed") <<
			"with exit code" << exitCode;
	m_process->deleteLater();
	m_process = nullptr;
	deleteLater();
}
