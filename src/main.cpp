// SPDX-License-Identifier: BSD-3-Clause

#include <QtGlobal>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QSplashScreen>
#include <QTimer>
#include <QColor>
#include <QWebEngineUrlScheme>
#include <QWebEngineUrlSchemeHandler>
#include <QWebEngineProfile>
#include "squintymongrel_config.h"
#include "window.h"


// https://stackoverflow.com/questions/240353/convert-a-preprocessor-token-to-a-string
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)


// example usage for checking version for includes
// remember to #include <QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif


// https://doc.qt.io/qt-5/qwebengineurlschemehandler.html
class MySchemeHandler : public QWebEngineUrlSchemeHandler
{
public:
    MySchemeHandler(QObject *parent = nullptr) {
    }
    void requestStarted(QWebEngineUrlRequestJob *request) {
    }
};


int main(int argc, char *argv[])
{
    // initialize the Qt resource system ('squintymongrel.qrc')
    Q_INIT_RESOURCE(squintymongrel);


    // https://stackoverflow.com/questions/52256264/qt-version-incorrect
    qDebug() << "";
    qDebug() << "Application Version           :" << "SquintyMongrel" << TOSTRING(SQUINTYMONGREL_VERSION);
    qDebug() << "Git SHA1                      :" << TOSTRING(SQUINTYMONGREL_GIT_HASH);
    qDebug() << "built with Qt Version (string):" << QT_VERSION_STR;
    qDebug() << "built with Qt Version (hex)   :"
             << (((QT_VERSION) >> 16) & 0xff)
             << (((QT_VERSION) >> 8) & 0xff)
             << ((QT_VERSION) & 0xff);
    qDebug() << "runtime Qt Version (string)   :" << qVersion();
    qDebug() << "";


    // https://doc.qt.io/qt-5/highdpi.html
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    // Qt5 only: Make QIcon::pixmap() generate high-dpi pixmaps that can be larger than the requested size.
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);


    // https://stackoverflow.com/questions/64892161/qtwebengine-fetch-api-fails-with-custom-scheme ( Qt 6.6 though :-( )
    // https://doc.qt.io/qt-5/qtwebengine-debugging.html
    // sudo apt-get install qtwebengine5-dev
    // https://doc.qt.io/qt-6/qtwebenginewidgets-module.html
    QWebEngineUrlScheme scheme("myscheme");
    scheme.setFlags(
        QWebEngineUrlScheme::SecureScheme |
        QWebEngineUrlScheme::LocalScheme |
        QWebEngineUrlScheme::LocalAccessAllowed |
        QWebEngineUrlScheme::ServiceWorkersAllowed |
        QWebEngineUrlScheme::ContentSecurityPolicyIgnored
        // QWebEngineUrlScheme::CorsEnabled - not available in Qt 5.12
    );
    QWebEngineUrlScheme::registerScheme(scheme);


    QApplication app(argc, argv);


    MySchemeHandler *handler = new MySchemeHandler();
    QWebEngineProfile::defaultProfile()->installUrlSchemeHandler("myscheme", handler);


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
