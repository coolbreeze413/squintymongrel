#include "cpp_endpoint.h"

#include <QApplication>
#include <QDebug>
#include <QVariant>



Q_INVOKABLE void CPPEndPoint::log(QVariant s)
{
  QString str = s.toString();
  qDebug() << "log from JS: " << str;
}


Q_INVOKABLE QVariant CPPEndPoint::getAppVersion()
{
  qDebug() << "getAppVersion()";
  
  QString appVersion = "v0.0.0";
  // emit updateFilePath(QCoreApplication::applicationDirPath() +
  //                                                 "/../share/squintymongrel/monaco-editor.html");

  return appVersion;
}


Q_INVOKABLE int CPPEndPoint::getIntValue()
{
  qDebug() << "getIntValue()";
  return m_intValue;
}
