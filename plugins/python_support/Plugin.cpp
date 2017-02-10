#include <cmath>
#include <Python.h>
#include <QDebug>
#include <QVariant>
#include <QDir>
#include "Plugin.h"

PluginClass::PluginClass(DCPluginManager &pluginManager): DCPlugin(pluginManager) {
	initialize("python_support");
	sendMessage("core:query-plugin-dir", QVariant(), [this](const QVariant &data) {
		QString fileName = data.toString() + QDir::separator() + "python";
		wchar_t programName[fileName.size() + 1];
		fileName.toWCharArray(programName);
		programName[fileName.size()] = 0;
		Py_SetProgramName(programName);
		Py_Initialize();
		m_initialized = true;
		sendMessage("core:register-plugin-loader",
					QVariantMap({{"pattern", "%0.py"}, {"tag", "python_support:load-plugin"}}));
		subscribe("python_support:load-plugin",
				  [this](const QString &sender, const QString &tag, const QVariant& data_) {
			loadPlugin(data_.toString());
		});
	});
}

void PluginClass::prepareToUnload() {
	if (m_initialized) {
		Py_Finalize();
	}
}

void PluginClass::loadPlugin(const QString &fileName) {
	QByteArray utf8 = fileName.toUtf8();
	PyObject *pName = PyUnicode_DecodeUTF8(utf8.data(), utf8.size(), nullptr);
	PyObject *pModule = PyImport_Import(pName);
	Py_DECREF(pName);
	if (pModule != nullptr) {
		qDebug() << "!!!";
	} else {
		PyErr_Print();
	}
}

DC_LOADABLE_PLUGIN_CLASS(PluginClass)
