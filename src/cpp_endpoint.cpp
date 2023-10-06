#include "cpp_endpoint.h"

#include <QApplication>
#include <QDebug>
#include <QVariant>


// https://stackoverflow.com/questions/240353/convert-a-preprocessor-token-to-a-string
// https://gcc.gnu.org/onlinedocs/gcc-13.2.0/cpp/Stringizing.html
// if we have '#define foo abcd', then:
// step 1: TOSTRING(foo)  fully macro-expanded -> TOSTRING(abcd) -> STRINGIFY(abcd)
#define TOSTRING(x) STRINGIFY(x)
// step 2: STRINGIFY(abcd) -> replaced by "abcd" and not macro expanded because it is stringized with '#'
#define STRINGIFY(x) #x


Q_INVOKABLE void CPPEndPoint::log(QVariant s)
{
  QString str = s.toString();
  qDebug() << "log from JS: " << str;
}


Q_INVOKABLE QVariant CPPEndPoint::getAppVersion()
{
  qDebug() << "getAppVersion()";
  
  QString appVersion = TOSTRING(BUILD_VERSION);

  return appVersion;
}


Q_INVOKABLE int CPPEndPoint::getIntValue()
{
  qDebug() << "getIntValue()";
  return m_intValue;
}
