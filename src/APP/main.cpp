#include "MainWindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QTranslator qtTranslator;
    QString qtTransPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
    if (qtTranslator.load("qt_zh_CN", qtTransPath)) {
        qApp->installTranslator(&qtTranslator);
        qDebug() << "Qt language set to Chinese.";
    } else {
        qDebug() << "Failed to load qt_zh_CN translation from:" << qtTransPath;
    }
    
    MainWindow w;
    w.show();
    return a.exec();
}
