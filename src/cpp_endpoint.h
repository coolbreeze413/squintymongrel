#ifndef CPP_ENDPOINT_H
#define CPP_ENDPOINT_H

#include <QObject>
#include <QVariant>


class QVariant;


class CPPEndPoint: public QObject
{
    Q_OBJECT

public:
    CPPEndPoint(QObject* parent=nullptr) {}


public:
    Q_INVOKABLE void log(QVariant s);
    Q_INVOKABLE QVariant getAppVersion();
    
    // expose 'intValue' as a property, which will invoke getIntValue() to get the value
    Q_PROPERTY(int intValue READ getIntValue NOTIFY intValueChanged);
    Q_INVOKABLE int getIntValue();


signals:
    void intValueChanged(int);
    void updateFilePath(const QString filepath);


private:
    int m_intValue = 413;
};

#endif // #ifndef CPP_ENDPOINT_H
