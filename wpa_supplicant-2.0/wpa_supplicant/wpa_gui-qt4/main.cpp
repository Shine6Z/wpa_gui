/*
 * wpa_gui - Application startup
 * Copyright (c) 2005-2006, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

//hack
#define QT_NO_SESSIONMANAGER

#ifdef CONFIG_NATIVE_WINDOWS
#include <winsock.h>
#endif /* CONFIG_NATIVE_WINDOWS */
#include <QtWidgets/QApplication>
#include <QtCore/QLibraryInfo>
#include <QtCore/QTranslator>
#include "wpagui.h"


class WpaGuiApp : public QApplication
{
public:
	WpaGuiApp(int argc, char **argv);

#ifndef QT_NO_SESSIONMANAGER
	void saveState();
#endif

	WpaGui *w;
};

WpaGuiApp::WpaGuiApp(int argc, char **argv) : QApplication(argc, argv)
{
}

#ifndef QT_NO_SESSIONMANAGER
void WpaGuiApp::saveState()
{
	QApplication::saveState();
	w->saveState();
}
#endif

int main(int argc, char *argv[])
{
	WpaGuiApp app(argc, argv);
	QTranslator translator;
	QString locale;
	QString resourceDir;
	int ret;

	locale = QLocale::system().name();
	resourceDir = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
	if (!translator.load("wpa_gui_" + locale, resourceDir))
		translator.load("wpa_gui_" + locale, "lang");
	app.installTranslator(&translator);

	WpaGui w(&app, 0, 0, 0, argc, argv);

#ifdef CONFIG_NATIVE_WINDOWS
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData)) {
		/* printf("Could not find a usable WinSock.dll\n"); */
		return -1;
	}
#endif /* CONFIG_NATIVE_WINDOWS */

	app.w = &w;

	ret = app.exec();

#ifdef CONFIG_NATIVE_WINDOWS
	WSACleanup();
#endif /* CONFIG_NATIVE_WINDOWS */

	return ret;
}
