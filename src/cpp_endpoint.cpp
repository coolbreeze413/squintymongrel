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

  return appVersion;
}


Q_INVOKABLE int CPPEndPoint::getIntValue()
{
  qDebug() << "getIntValue()";
  return m_intValue;
}
