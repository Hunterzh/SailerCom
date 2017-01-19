#include "com.h"
#include <QApplication>

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

