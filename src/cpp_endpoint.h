#ifndef CPP_ENDPOINT_H
#define CPP_ENDPOINT_H

#include <QObject>
#include <QVariant>


class QVariant;


class CPPEndPoint: public QObject
{
    Q_OBJECT

public:
    CPPEndPoint(QObject* parent=nullptr);


public:
    Q_INVOKABLE void log(QVariant s);
    Q_INVOKABLE QVariant getAppVersion();
    Q_INVOKABLE void saveFileContent(QVariant fileContent);
    
    // expose 'intValue' as a property, which will invoke getIntValue() to get the value
    Q_PROPERTY(int intValue READ getIntValue NOTIFY signalToJS_IntValueChanged);
    Q_INVOKABLE int getIntValue();

    // expose 'qtVersion' as a property, which will invoke getQtVersion() to get the value
    Q_PROPERTY(QVariant qtVersion READ getQtVersion CONSTANT);
    Q_INVOKABLE QVariant getQtVersion();


signals:

    // to Monaco Text Editor JS
    void signalToJS_IntValueChanged(int);
    void signalToJS_UpdateFilePath(const QString filepath);
    void signalToJS_SaveFile();

    // to Monaco Text Editor C++
    void signalToCPP_SaveFileContentFromJS(QVariant fileContent);


private:
    int m_intValue = 413;
    QList<int> m_qtVersion;
};

#endif // #ifndef CPP_ENDPOINT_H
