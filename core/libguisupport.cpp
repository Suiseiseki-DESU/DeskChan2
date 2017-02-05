#include <QApplication>

extern "C" {

Q_DECL_EXPORT QCoreApplication *DeskChan_createApplication(int &argc, char **argv) {
	QApplication *app = new QApplication(argc, argv);
	app->setProperty("guiSupportAvailable", true);
	return app;
}

}
