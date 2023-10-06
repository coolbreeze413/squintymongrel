// SPDX-License-Identifier: BSD-3-Clause

#include "window.h"
#include "monaco_text_editor.h"

#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QMainWindow>
#include <QWindow>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QMovie>
#include <QProgressDialog>
#include <QDebug>
#include <QSpacerItem>
#include <QWebEngineView>
#include <QTreeView>
#include <QFileSystemModel>
#include <QSplitter>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <QWebEngineScript>

#include <iostream>
#include <regex>
#include <fstream>


Window::Window()
{
    createTextEditor();
    createFileExplorerView();
    createActions();

    QIcon icon(":images/logo.svg");

#ifndef QT_NO_SYSTEMTRAYICON
    createTrayIcon();
    // QSystemTrayIcon::MessageIcon msgIcon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
    trayIcon->setIcon(icon);
    trayIcon->setToolTip("Tray Icon Tooltip");
    connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &Window::messageClicked);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &Window::iconActivated);
    trayIcon->show();
#endif // #ifndef QT_NO_SYSTEMTRAYICON

    QVBoxLayout *centraWidgetLayout = new QVBoxLayout;
    QSplitter* splitter = new QSplitter();
    splitter->addWidget(fileExplorerView);
    splitter->setStretchFactor(0, 1);
    splitter->addWidget(textEditor);
    splitter->setStretchFactor(1, 10);
    centraWidgetLayout->addWidget(splitter);

    QWidget *widget = new QWidget();
    widget->setLayout(centraWidgetLayout);

    setCentralWidget(widget);

    setWindowTitle(tr("squintymongrel"));
    setWindowIcon(icon);

    // finally adjust ourself to position centered and an appropriate size
    setPositionAndSize();
}


void Window::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QMainWindow::setVisible(visible);
}


void Window::setPositionAndSize()
{
    // aesthetic: setup the MainWindow to be two-third the area of the screen where it is displayed, centered.

    // support multiple screens correctly.
    // NOTE: show() required before windowHandle() is valid!
    show();

    // set the screen to be displayed according to the mouse cursor position right now
    windowHandle()->setScreen(QGuiApplication::screenAt(QCursor::pos()));

    // set the geometry so that we are approximately 2/3 of the screen size
    QRect desk_rect = QGuiApplication::screenAt(QCursor::pos())->geometry();
    setGeometry(desk_rect.width() / 6, desk_rect.height() / 6, desk_rect.width() * 2 / 3,
                desk_rect.height() * 2 / 3);
    
    // move ourself so that we are centered
    move(desk_rect.width() / 2 - width() / 2 + desk_rect.left(), desk_rect.height() / 2 - height() / 2 + desk_rect.top());
}


void Window::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange)
    {
        if (windowState() == Qt::WindowMinimized)
        {
#ifndef QT_NO_SYSTEMTRAYICON
            // if (simpleCheckBox->isChecked())
            // {
            //     // make ourselves invisible (tray icon only.)
            //     hide();
            // }
#endif // #ifndef QT_NO_SYSTEMTRAYICON
        }
        else if (windowState() == Qt::WindowNoState)
        {
        }
    }
}


void Window::closeEvent(QCloseEvent *event)
{
    // close event originated outside application (system event?)
    // basically, did the user click the close button?
    if (event->spontaneous())
    {
        // qDebug() << "closeEvent() -> spontaneous()";
    }
    else
    {
        // qDebug() << "closeEvent() -> *NOT* spontaneous()";
    }
    QMainWindow::closeEvent(event);
}


#ifndef QT_NO_SYSTEMTRAYICON
void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        break;
    }
}


void Window::showMessage()
{
    QSystemTrayIcon::MessageIcon msgIcon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
    QIcon icon(":/images/squintymongrel.svg");
    trayIcon->showMessage("This is TITLE",
                          "This is BODY",
                          icon,
                          1 * 1000);
}


void Window::messageClicked()
{
    QMessageBox::information(nullptr,
                             tr("TITLE"),
                             tr("TEXT"));
}


void Window::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}
#endif // #ifndef QT_NO_SYSTEMTRAYICON


void Window::createTextEditor() {

    textEditor = new MonacoTextEditor(this);
}


void Window::createFileExplorerView() {

    model = new QFileSystemModel();
    
    fileExplorerView = new QTreeView();
    fileExplorerView->setModel(model);
    fileExplorerView->setAnimated(false);
    fileExplorerView->setIndentation(20);
    fileExplorerView->setSortingEnabled(true);
    fileExplorerView->hideColumn(1);
    fileExplorerView->hideColumn(2);
    fileExplorerView->hideColumn(3);
    fileExplorerView->setWindowTitle(QObject::tr("Dir View"));
    
    QModelIndex index = model->setRootPath(QCoreApplication::applicationDirPath());
    fileExplorerView->setCurrentIndex(index);

    QObject::connect(fileExplorerView, &QTreeView::doubleClicked, this, &Window::onFileExplorerDoubleClicked);
}

void Window::onFileExplorerDoubleClicked(const QModelIndex& modelIndex) {
    
    QString filepath = model->filePath(modelIndex);
    
    qDebug() << "double-clicked on: " << filepath;

    if(QFileInfo(filepath).isFile()) {
        textEditor->openFileInCurrentTab(filepath);
    }
}


void Window::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}
