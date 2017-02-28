#include "com.h"
#if QT_VERSION >= 0x050000
#include <QApplication>
#else
#include <QtGui/QApplication>
#endif
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    com w;
    w.setWindowTitle("SailerCom");
    w.show();
    //tcp n;
    //n.show();


    return a.exec();
}

