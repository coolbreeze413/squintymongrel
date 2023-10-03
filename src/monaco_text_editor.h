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


private:
    QVBoxLayout* monacoTextEditorVBoxLayout;

    QWebEngineView* webEngineView;
    QWebChannel* webEngineChannel;
    CPPEndPoint* CPPEndPointObject;
};

#endif // #ifndef MONACOTEXTEDITOR_H