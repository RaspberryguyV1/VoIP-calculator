#include "mainwindow.h"
#include <QApplication>
#include <QDirIterator>
#include <QFile>
#include <QFontDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Fonty dolaczone do aplikacji (resources.qrc -> fonts/). Brak fontow nie psuje programu,
    // wtedy Qt uzyje czcionki zastepczej.
    QDirIterator fonts(":/fonts", {"*.ttf", "*.otf"}, QDir::Files, QDirIterator::Subdirectories);
    while (fonts.hasNext()) {
        const QString path = fonts.next();
        const int id = QFontDatabase::addApplicationFont(path);
        qDebug() << "Font:" << path << "->" << QFontDatabase::applicationFontFamilies(id);
    }

    // Wspolny styl calej aplikacji
    QFile qss(":/style.qss");
    if (qss.open(QIODevice::ReadOnly | QIODevice::Text)) {
        a.setStyleSheet(QString::fromUtf8(qss.readAll()));
    } else {
        qWarning() << "BRAK style.qss w zasobach - resources.qrc nie jest dodany do projektu albo projekt nie zostal przebudowany";
    }

    MainWindow w;
    w.show();
    return a.exec();
}
