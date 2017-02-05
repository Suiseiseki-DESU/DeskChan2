#include <QCoreApplication>
#include <QDebug>
#include <QLibrary>
#include <QFileInfo>
#include <QDir>

#ifndef DC_CORE_LIBRARY
#error "DC_CORE_LIBRARY undefined"
#endif

int main(int argc, char **argv) {
	Q_ASSERT(argc > 0);
	QString executableFileName = argv[0];
	QFileInfo executableFileInfo(executableFileName);
	Q_ASSERT(executableFileInfo.isReadable());
	QString executableFileDir = executableFileInfo.absoluteDir().path();
	QString libraryFileName = executableFileDir + QDir::separator() + "libDeskChan_System";
	QLibrary library(libraryFileName);
	library.setLoadHints(QLibrary::ResolveAllSymbolsHint | QLibrary::ExportExternalSymbolsHint);
	if (!library.load()) {
		auto msg = library.errorString().toStdString();
		qFatal(msg.c_str());
	}
	auto mainFunc = (int (*)(int, char**))library.resolve("DeskChan_main");
	Q_ASSERT(mainFunc != nullptr);
	QString guiSupportLibraryName = executableFileDir + QDir::separator() + "libDeskChan_GuiSupport";
	QLibrary guiSupportLibrary(guiSupportLibraryName);
	QCoreApplication *app = nullptr;
	if (guiSupportLibrary.load()) {
		auto createApplicationFunc = (QCoreApplication *(*)(int&, char**))
				guiSupportLibrary.resolve("DeskChan_createApplication");
		Q_ASSERT(createApplicationFunc != nullptr);
		app = createApplicationFunc(argc, argv);
	}
	if (app == nullptr) {
		app = new QCoreApplication(argc, argv);
	}
	int retVal = mainFunc(argc, argv);
	delete app;
	return retVal;
}
