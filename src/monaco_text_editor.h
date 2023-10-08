#ifndef MONACOTEXTEDITOR_H
#define MONACOTEXTEDITOR_H

class QObject;
class QWidget;
class QVBoxLayout;
class QWebEngineView;
class QWebChannel;
class CPPEndPoint;

#include <QWidget>

class MonacoTextEditor : public QWidget
{
    Q_OBJECT


public:
    MonacoTextEditor(QWidget* parent=nullptr);
    ~MonacoTextEditor();

    void openFileInCurrentTab(QString filepath);
    void runJavaScript(QString javascriptCode);


private:
    QVBoxLayout* monacoTextEditorVBoxLayout;

    QWebEngineView* webEngineView;
    QWebChannel* webEngineChannel;
    CPPEndPoint* CPPEndPointObject;

    // actions
    QAction* saveCurrentFileAction;

    // data
    QString currentFilePath;

private slots:
    void handleAction_SaveFile();

public slots:
    // handle signals from JS side
    void handleSignalFromJS_SaveFileContent(QVariant fileContent);
};

#endif // #ifndef MONACOTEXTEDITOR_H