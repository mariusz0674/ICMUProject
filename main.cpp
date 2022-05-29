#include <QtWidgets/QApplication>

#include "MainEngine.h"
#include "ICMUProject.h"
#include <Qfile.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Set style sheet
    QFile styleSheet("./Geoo.qss");
    styleSheet.open(QFile::ReadOnly);
    QString styleSheetS = QLatin1String(styleSheet.readAll());
    a.setStyleSheet(styleSheetS);
    a.palette().setColor(QPalette::Window, QColor(53, 53, 53));

    

    ICMUProject w;
   // w.setWindowFlags(Qt::WindowSystemMenuHint);
    w.showMaximized();
    w.show();
    w.inicialActions();

   
    
    return a.exec();
}
