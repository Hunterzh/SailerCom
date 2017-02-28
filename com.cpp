#include "com.h"
#include "ui_com.h"

com::com(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::com)
{
    ui->setupUi(this);

#ifdef Q_OS_LINUX
   ui->portNameComboBox->addItem( "ttyUSB0");
   ui->portNameComboBox->addItem( "ttyUSB1");
   ui->portNameComboBox->addItem( "ttyUSB2");
   ui->portNameComboBox->addItem( "ttyUSB3");
   ui->portNameComboBox->addItem( "ttyS0");
   ui->portNameComboBox->addItem( "ttyS1");
   ui->portNameComboBox->addItem( "ttyS2");
   ui->portNameComboBox->addItem( "ttyS3");
   ui->portNameComboBox->addItem( "ttyS4");
   ui->portNameComboBox->addItem( "ttyS5");
   ui->portNameComboBox->addItem( "ttyS6");
#elif defined (Q_OS_WIN)
   ui->portNameComboBox->addItem("COM0");
   ui->portNameComboBox->addItem("COM1");
   ui->portNameComboBox->addItem("COM2");
   ui->portNameComboBox->addItem("COM3");
   ui->portNameComboBox->addItem("COM4");
   ui->portNameComboBox->addItem("COM5");
   ui->portNameComboBox->addItem("COM6");
   ui->portNameComboBox->addItem("COM7");
   ui->portNameComboBox->addItem("COM8");
   ui->portNameComboBox->addItem("COM9");
   ui->portNameComboBox->addItem("COM10");
   ui->portNameComboBox->addItem("COM11");
   ui->portNameComboBox->addItem("COM12");
   ui->portNameComboBox->addItem("COM13");
   ui->portNameComboBox->addItem("COM14");
   ui->portNameComboBox->addItem("COM15");

#endif

    ui->closeBtn->setEnabled(false); //开始“关闭串口”按钮不可用
    ui->sendBtn->setEnabled(false);
    ui->changeBtn->setEnabled(false);
    ui->zeroBtn->setEnabled(false);
    ui->resetBtn->setEnabled(false);
    ui->stopBtn->setEnabled(false);
    ui->testBtn->setEnabled(false);
    ui->clearUpBtn->setEnabled(false);

    RevAckBuffer.resize(12);
}

com::~com()
{
    delete ui;
}

void com::readSailerCom() //读串口函数
{
    QByteArray temp;

    QString buf;

    temp = SailerCom->readAll();

    if(temp.size()==12)
    {
        ui->cmdText->setText(QString::number(RecvData(temp), 10));
    }

    if(!temp.isEmpty()){
            ui->receiveText->setTextColor(Qt::black);
            if(ui->ccradioButton->isChecked()){
                buf = temp;
            }else if(ui->chradioButton->isChecked()){
                QString str;
                for(int i = 0; i < temp.count(); i++){
                    QString s;
                    s.sprintf("0x%02x, ", (unsigned char)temp.at(i));
                    buf += s;
                }

            }

        ui->receiveText->setText(ui->receiveText->document()->toPlainText() + buf);
        QTextCursor cursor = ui->receiveText->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->receiveText->setTextCursor(cursor);

        //ui->lcdNumber->display(temp.size());
        ui->recvbyteslcdNumber->display(ui->recvbyteslcdNumber->value() + temp.size());

    }


}

QByteArray com::sendCMD(int cmd)
{
    QByteArray data(16,0x00);

    data[0] = 0x4e; data[1] = 0x41; data[2] = 0x43; data[3] = 0x53;
    data[7] = 0x04;     //length

    QByteArray cmdData = crc32::intToByte(cmd);
    data[8] = cmdData[3]; data[9] = cmdData[2]; data[10] = cmdData[1]; data[11] = cmdData[0];

    QByteArray crc = crc32::GetCRC32(data, 0, 12);
    if (crc.isEmpty() || crc.size() != 4)
        return 0;
    for (int i = 0; i < 4; i++)
        data[12 + i] = crc[i];

    return data;
}

QByteArray com::sendData(QVector<int> coordData, int start, int count, int packNumber)
{
    QByteArray data(256,0x00);
    QByteArray number = crc32::intToByte(packNumber);
    QByteArray length = crc32::intToByte(count*4);

    data[0] = 0x4e;      data[1] = 0x41;      data[2] = 0x43;       data[3] = 0x53; //SCAN
    data[4] = number[3]; data[5] = number[2]; data[6] = number[1];  data[7] = number[0];//number
    data[8] = length[3]; data[9] = length[2]; data[10] = length[1]; data[11] = length[0];//length

    for(int i=0,j=0,k=start;i<count;i++,j+=4,k++)
    {
    QByteArray byteData = crc32::intToByte(coordData[k]);
    //int value = coordData[k];
    data[j+12] = byteData[3];
    data[j+13] = byteData[2];
    data[j+14] = byteData[1];
    data[j+15] = byteData[0];
    }
    /**/
    QByteArray crc = crc32::GetCRC32(data, 0, 252);
    if (crc.isEmpty() || crc.size() != 4)
        return 0;
    for (int i = 0; i < 4; i++)
        data[252 + i] = crc[i];

    return data;
}

void com::Circle(int x, int y, int r)
{
    Coord.resize(720);
    int i=0,j=0;
    for(i;i<360;i++,j++)
    {
        Coord[j]=(int)(x+r*qCos(i*3.1415926/180));
        Coord[j+1]=(int)(y+r*qSin(i*3.1415926/180));
        j++;
    }
}
//接收数据ACK
void com::SendDataAck()
{

}
//接收命令ACK
void com::SendCmdAck()
{

}
//运行完毕ACK
void com::SendEndAck()
{
    QByteArray data(16,0x00);
    data[0] = 0x4e; data[1] = 0x41; data[2] = 0x43; data[3] = 0x53;
    data[4] = 0x08;     //length
    data[8] = 0x02;
    data[11] = 0x01;    //cmd

    QByteArray crc = crc32::GetCRC32(data, 0, 12);
    if (crc.isEmpty() || crc.size() != 4)
        return;
    for (int i = 0; i < 4; i++)
        data[12 + i] = crc[i];

}

int com::RecvData(QByteArray buffer)
{

       if (buffer.isEmpty())
           return -1;
//(unsigned char)buffer.at(i)

       if (buffer.at(0)==0x4e &&
           buffer.at(1)==0x41 &&
           buffer.at(2)==0x43 &&
           buffer.at(3)==0x53 )
           {
               //return 1;
           }
           else
           {

               return 2;
           }

       //crc校验
       QByteArray crc = crc32::GetCRC32(buffer, 0, 8);
       if (crc[0] == buffer[8] &&
           crc[1] == buffer[9] &&
           crc[2] == buffer[10] &&
           crc[3] == buffer[11])
           {
                //return 3;
           }
           else
           {
                //校验不正确
                return 4;
           }

           int dwCmd = (int)((buffer[4] << 24) | (buffer[5] << 16) | (buffer[6] << 8) | (buffer[7]));

           switch (dwCmd)
           {
           case 1001:   //到位
               {
                return 101;
               }
               break;
           case 1002:   //
               {
                return 2;
               }
               break;
           case 1003:   //
               {
                return 3;
               }
               break;
           }

    return dwCmd;
}

void com::on_closeBtn_clicked()
{
    SailerCom->close(); //关闭串口，该函数在win_qextserialport.cpp文件中定义
    delete SailerCom;
    SailerCom = NULL;

    ui->openBtn->setEnabled(true); //关闭串口后“打开串口”按钮可用

    ui->closeBtn->setEnabled(false); //关闭串口后“关闭串口”按钮不可用

    ui->sendBtn->setEnabled(false); //关闭串口后“发送数据”按钮不可用ui->baudRateComboBox->setEnabled(true);

    ui->dataBitsComboBox->setEnabled(true);//设置各个组合框可用

    ui->parityComboBox->setEnabled(true);

    ui->stopBitsComboBox->setEnabled(true);

    ui->portNameComboBox->setEnabled(true);

    ui->baudRateComboBox->setEnabled(true);


}



void com::on_sendBtn_clicked()
{
    QByteArray buf;
    buf=sendCMD(ui->cmdText->text().toInt());
    SailerCom->write(buf);
}

void com::on_openBtn_clicked()
{
    QString portName = ui->portNameComboBox->currentText(); //获取串口名

#ifdef Q_OS_LINUX
    SailerCom = new QextSerialPort("/dev/" + portName);
#elif defined (Q_OS_WIN)
    SailerCom = new QextSerialPort(portName);
#endif
    //定义串口对象，并传递参数，在构造函数里对其进行初始化

    SailerCom ->open(QIODevice::ReadWrite); //打开串口

    //设置波特率
    if(ui->baudRateComboBox->currentText()==tr("9600")) //根据组合框内容对串口进行设置

    SailerCom->setBaudRate(BAUD9600);

    else if(ui->baudRateComboBox->currentText()==tr("115200"))

    SailerCom->setBaudRate(BAUD115200);

    else if(ui->baudRateComboBox->currentText()==tr("2400"))

    SailerCom->setBaudRate(BAUD2400);

    else if(ui->baudRateComboBox->currentText()==tr("4800"))

    SailerCom->setBaudRate(BAUD4800);

    //else if(ui->baudRateComboBox->currentText()==tr("14400"))

    //SailerCom->setBaudRate(BAUD9600);

    else if(ui->baudRateComboBox->currentText()==tr("19200"))

    SailerCom->setBaudRate(BAUD19200);

    else if(ui->baudRateComboBox->currentText()==tr("38400"))

    SailerCom->setBaudRate(BAUD38400);

    //else if(ui->baudRateComboBox->currentText()==tr("56000"))

    //SailerCom->setBaudRate(BAUD56000);

    else if(ui->baudRateComboBox->currentText()==tr("57600"))

    SailerCom->setBaudRate(BAUD57600);

    //else if(ui->baudRateComboBox->currentText()==tr("128000"))

    //SailerCom->setBaudRate(BAUD128000);

    //else if(ui->baudRateComboBox->currentText()==tr("256000"))

    //SailerCom->setBaudRate(BAUD256000);

    //设置数据位

    if(ui->dataBitsComboBox->currentText()==tr("8"))

    SailerCom->setDataBits(DATA_8);

    else if(ui->dataBitsComboBox->currentText()==tr("7"))

    SailerCom->setDataBits(DATA_7);

    //设置奇偶校验

    if(ui->parityComboBox->currentText()==tr("wu"))

    SailerCom->setParity(PAR_NONE);

    else if(ui->parityComboBox->currentText()==tr("ji"))

    SailerCom->setParity(PAR_ODD);

    else if(ui->parityComboBox->currentText()==tr("ou"))

    SailerCom->setParity(PAR_EVEN);

    //设置停止位

    if(ui->stopBitsComboBox->currentText()==tr("1"))

    SailerCom->setStopBits(STOP_1);

    else if(ui->stopBitsComboBox->currentText()==tr("2"))

    SailerCom->setStopBits(STOP_2);


    SailerCom->setFlowControl(FLOW_OFF); //设置数据流控制，我们使用无数据流控制的默认设置

    SailerCom->setTimeout(20); //设置延时


    connect(SailerCom,SIGNAL(readyRead()),this,SLOT(readSailerCom()));

    //信号和槽函数关联，当串口缓冲区有数据时，进行读串口操作

    ui->openBtn->setEnabled(false); //打开串口后“打开串口”按钮不可用

    ui->closeBtn->setEnabled(true); //打开串口后“关闭串口”按钮可用

    ui->sendBtn->setEnabled(true); //打开串口后“发送数据”按钮可用

    ui->baudRateComboBox->setEnabled(false); //设置各个组合框不可用

    ui->dataBitsComboBox->setEnabled(false);

    ui->parityComboBox->setEnabled(false);

    ui->stopBitsComboBox->setEnabled(false);

    ui->portNameComboBox->setEnabled(false);

    ui->changeBtn->setEnabled(true);
    ui->zeroBtn->setEnabled(true);
    ui->resetBtn->setEnabled(true);
    ui->stopBtn->setEnabled(true);
    ui->testBtn->setEnabled(true);
    ui->clearUpBtn->setEnabled(true);
}

void com::on_clearUpBtn_clicked()
{
    ui->receiveText->clear();
    ui->recvbyteslcdNumber->display(0);
}

void com::on_testBtn_clicked()
{
    Circle(ui->xText->text().toInt(), ui->yText->text().toInt(), ui->rtext->text().toInt());

    QByteArray temp;
    QString buf;
    temp = sendData(Coord,ui->startText->text().toInt(),60,ui->sideText->text().toInt());
    SailerCom->write(temp);
/*
    if(!temp.isEmpty()){
            ui->receiveText->setTextColor(Qt::black);
            if(ui->ccradioButton->isChecked()){
                buf = temp;
            }else if(ui->chradioButton->isChecked()){
                QString str;
                for(int i = 0; i < temp.count(); i++){
                    QString s;
                    s.sprintf("0x%02x, ", (unsigned char)temp.at(i));
                    buf += s;
                }
            }

        ui->receiveText->setText(ui->receiveText->document()->toPlainText() + buf);
        QTextCursor cursor = ui->receiveText->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->receiveText->setTextCursor(cursor);

        ui->recvbyteslcdNumber->display(ui->recvbyteslcdNumber->value() + temp.size());
    }
    */

}

void com::on_stopBtn_clicked()
{
    QByteArray buf;
    buf=sendCMD(34);
    SailerCom->write(buf);
}


void com::on_resetBtn_clicked()
{
    QByteArray buf;
    buf=sendCMD(36);
    SailerCom->write(buf);
}

void com::on_zeroBtn_clicked()
{
    QByteArray buf;
    buf=sendCMD(31);
    SailerCom->write(buf);
}

void com::on_changeBtn_clicked()
{
    QByteArray buf;
    buf=sendCMD(32);
    SailerCom->write(buf);
}
