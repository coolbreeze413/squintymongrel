#include "monaco_text_editor.h"
#include "cpp_endpoint.h"

#include <QCoreApplication>
#include <QDebug>
#include <QWidget>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <QWebEngineScript>


MonacoTextEditor::MonacoTextEditor(QWidget* parent) :
    QWidget(parent) {

  qDebug() << QCoreApplication::applicationDirPath();

  monacoTextEditorVBoxLayout = new QVBoxLayout();

  // create the web engine view to hold the HTML content
  webEngineView = new QWebEngineView(this);

  // create a separate 'end-point' object to handle comms between C++ and JS
  CPPEndPointObject = new CPPEndPoint(this);

  // ref: https://doc.qt.io/qt-5/qwebenginesettings.html#WebAttribute-enum
  // this doesn't seem to be required ?
  webEngineView->page()->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
  webEngineView->page()->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);

  // also ref: future.
  // https://stackoverflow.com/questions/11228435/qt-including-resource-directory-structure-inside-executable
  // https://github.com/fccoelho/qrcgen

  // create a webchannel for comms between C++ and JS and register the 'end-point' object to be used
  // on the C++ side to handle the commns
  webEngineChannel = new QWebChannel(this);
  webEngineChannel->registerObject(QStringLiteral("CPPEndPoint"), CPPEndPointObject);

  // set the webchannel on the web engine view's page
  webEngineView->page()->setWebChannel(webEngineChannel);
  // load the HTML page
  webEngineView->page()->load(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() +
                                                  "/../share/squintymongrel/monaco-editor.html"));

  // did the HTML page load ok?
  QObject::connect(
    webEngineView->page(),
    &QWebEnginePage::loadFinished,
    [this](bool ok) {
      qDebug() << "URL: " << this->webEngineView->url().toString();
      qDebug() << "Load Finished: " << ok;

      if (!ok)
      {
        // exit the application here?
      }

      // we can choose to load js files from this code, instead of modifying
      // the HTML page directly (this code is left here for future usage in this way and testing)

      // NOTE: verify that the 'qwebchannel.js' file being loaded by us is the right one !!!
      //       if it's not, we see unexpected issues such as:
      //         'Cannot invoke unknown method of index -1 on object ...'
      //         'Error: qt is not defined'
      //       if in doubt, enable the 'built-in' file using ":/qtwebchannel/qwebchannel.js" and make a copy of this file
      //          locally using the 'copycode' below and compare.

      // 1. load the qwebchannel.js into the HTML pae
      // // QFile file(":/qtwebchannel/qwebchannel.js");
      // QFile file(QCoreApplication::applicationDirPath() +
      //                     "/../share/squintymongrel/qwebchannel.js");
      // if (!file.open(QIODevice::ReadOnly))
      // {
      //     qDebug() << "could not read!";
      // }
      // QString js;
      // QTextStream stream(&file);
      // js.append(stream.readAll());
      // this->webEngineView->page()->runJavaScript(js);

      // 2. copycode to copy the 'built-in' file if needed
      // QFile file2(QCoreApplication::applicationDirPath() +
      //                     "/../share/squintymongrel/qwebchannel_save.js");
      // if (!file2.open(QIODevice::WriteOnly))
      // {
      //     qDebug() << "could not write!";
      // }
      // QTextStream outstream(&file2);
      // outstream << js;
      // file2.close();

      // 3. insert JS code to create webchannel and access C++ API from JS
      // QString code2 = QStringLiteral(
      // R"DELIM(

      // window.webChannel = new QWebChannel(qt.webChannelTransport, function( channel)
      // {
      //     window.CPPEndPoint = channel.objects.CPPEndPoint;
      //     window.CPPEndPoint.log("QWebChannel created!");
      // });

      // )DELIM");
      // this->webEngineView->page()->runJavaScript(code2);

      // 4. insert JS code to 'test' if the channel we created previously is still alive and usable.
      // QString code3 = QStringLiteral(
      // R"DELIM(

      //     window.CPPEndPoint.log("Test log() call!");

      // )DELIM");
      // this->webEngineView->page()->runJavaScript(code3);


      // emit CPPEndPointObject->updateFilePath(QCoreApplication::applicationDirPath() +
      //                                  "/../share/squintymongrel/monaco-editor.html");
    }
  );

  monacoTextEditorVBoxLayout->addWidget(webEngineView);
  this->setLayout(monacoTextEditorVBoxLayout);
}


MonacoTextEditor::~MonacoTextEditor() {

}

void MonacoTextEditor::openFileInCurrentTab(QString filepath) {

  emit CPPEndPointObject->updateFilePath(filepath);

}