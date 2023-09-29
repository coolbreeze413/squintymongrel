// SPDX-License-Identifier: BSD-3-Clause

#include <QApplication>
#include <QMessageBox>
#include <QSplashScreen>
#include <QTimer>
#include <QColor>
#include "squintymongrel_config.h"
#include "window.h"

int main(int argc, char *argv[])
{
    // initialize the Qt resource system ('squintymongrel.qrc')
    Q_INIT_RESOURCE(squintymongrel);

    // https://doc.qt.io/qt-5/highdpi.html
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    // Qt5 only: Make QIcon::pixmap() generate high-dpi pixmaps that can be larger than the requested size.
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    char ARG_DISABLE_WEB_SECURITY[] = "--disable-web-security";
    int newArgc = argc+1+1;
    char** newArgv = new char*[newArgc];
    for(int i=0; i<argc; i++) {
        newArgv[i] = argv[i];
    }
    newArgv[argc] = ARG_DISABLE_WEB_SECURITY;
    newArgv[argc+1] = nullptr;

    QApplication app(argc, argv);

#ifndef QT_NO_SYSTEMTRAYICON
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::warning(nullptr, 
                             QObject::tr("squintymongrel"),
                             QObject::tr("system tray not available on this system!"));
        return 1;
    }
#endif // #ifndef QT_NO_SYSTEMTRAYICON


#if SQUINTYMONGREL_USE_SPLASH_SCREEN
    QPixmap pixmap = QIcon(":/images/logo.svg").pixmap(QSize(88*7,95*7)); // use original image w,h or multiple.
    QSplashScreen splash(pixmap, Qt::SplashScreen | Qt::WindowStaysOnTopHint);
    splash.show();
    splash.showMessage(QObject::tr("Thinking..."), Qt::AlignBottom | Qt::AlignRight, Qt::black);
    QTimer::singleShot(3000, &splash, &QWidget::close); // keep displayed for 5 seconds
#endif // #if SQUINTYMONGREL_USE_SPLASH_SCREEN

    Window window;
    window.show();
#if SQUINTYMONGREL_USE_SPLASH_SCREEN
    QTimer::singleShot(3000, &window, &QWidget::raise); // raise it to front after splash screen is done.
#endif // #if SQUINTYMONGREL_USE_SPLASH_SCREEN
    return app.exec();
}
