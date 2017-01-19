#ifndef COM_H
#define COM_H

#include <QMainWindow>
//#include <win_qextserialport.h>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QGridLayout>
#include <QLabel>
#include "crc32.h"
#include <QtMath>
#include <QVector>
#include "qextserial/qextserialport.h"
namespace Ui {
class com;
}

class com : public QMainWindow
{
    Q_OBJECT

public:
    explicit com(QWidget *parent = 0);
    ~com();


private:
    Ui::com *ui;
    QextSerialPort *SailerCom;



private slots:
    void on_closeBtn_clicked();
    void on_sendBtn_clicked();
    void on_openBtn_clicked();
    void readSailerCom();
    void on_clearUpBtn_clicked();
    void on_testBtn_clicked();

    void on_resetBtn_clicked();

    void on_stopBtn_clicked();


    void on_zeroBtn_clicked();

    void on_changeBtn_clicked();

public:
    QVector<int> Coord;
    int ACK;
    QByteArray RevAckBuffer;
    QByteArray sendCMD(int cmd);
    QByteArray sendData(QVector<int> coordData, int start, int count, int packNumber);
    void Circle(int x,int y,int r);
    void SendDataAck();
    void SendCmdAck();
    void SendEndAck();
    int RecvData(QByteArray buffer);

};

#endif // COM_H
