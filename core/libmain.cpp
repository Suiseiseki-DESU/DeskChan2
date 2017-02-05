#include <QCoreApplication>
#include "DCPluginManager.h"

extern "C" {

Q_DECL_EXPORT int DeskChan_main(int argc, char **argv) {
	DCPluginManager pluginManager;
	pluginManager.initialize();
	return QCoreApplication::exec();
}

}
