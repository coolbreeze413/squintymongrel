// SPDX-License-Identifier: BSD-3-Clause

#ifndef WINDOW_H
#define WINDOW_H

#include <QSystemTrayIcon>
#include <QMainWindow>


QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
class QTreeView;
QT_END_NAMESPACE

class MonacoTextEditor;

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window();

    void setVisible(bool visible) override;
    void setPositionAndSize();

protected:
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override;

private slots:
#ifndef QT_NO_SYSTEMTRAYICON
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMessage();
    void messageClicked();
#endif

private:
    void createTextEditor();
    void createFileExplorerView();
    void createActions();
#ifndef QT_NO_SYSTEMTRAYICON
    void createTrayIcon();
#endif

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QTreeView* fileExplorerView;
    MonacoTextEditor* textEditor;

#ifndef QT_NO_SYSTEMTRAYICON
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
#endif // #ifndef QT_NO_SYSTEMTRAYICON

};

#endif // #ifndef WINDOW_H
