#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>
#include  <QTest>
#include <QEvent>
#include <QObject>

QT_USE_NAMESPACE


QSettings settings("ABC","Serial");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        // ui->textEdit->append(info.portName());
        ui->serialNoComboBox->addItem(info.portName());

    }
    readSettings();
    connect(&serial, SIGNAL(readyRead()),this, SLOT(serialRead()));

    gcode = "x0 y0 z0";
    ui->lineEdit_n_obj->setText("0");
    //  on_connectButton_clicked();
}

void MainWindow::serialConnect()
{
    if (serial.portName() != ui->serialNoComboBox->currentText())
    {
        serial.close();
        serial.setPortName(ui->serialNoComboBox->currentText());
        if (!serial.open(QIODevice::ReadWrite))
        {
            ui->statusBar->showMessage(tr("Can't open %1, error code %2")
                                       .arg(serial.portName()).arg(serial.error()));
            return;
        }

        //if (!serial.setBaudRate(toInt(ui->baundRateComboBox->currentText()))
        if (!serial.setBaudRate((ui->baundRateComboBox->currentText()).toInt()))
        {
            ui->statusBar->showMessage(tr("Can't set rate baud to port %1, error code %2")
                                       .arg(serial.portName()).arg(serial.error()));
        }

        if (!serial.setDataBits(QSerialPort::Data8))
        {
            ui->statusBar->showMessage(tr("Can't set 8 data bits to port %1, error code %2")
                                       .arg(serial.portName()).arg(serial.error()));
            return;
        }

        if (!serial.setParity(QSerialPort::NoParity))
        {
            ui->statusBar->showMessage(tr("Can't set no patity to port %1, error code %2")
                                       .arg(serial.portName()).arg(serial.error()));
            return;
        }

        if (!serial.setStopBits(QSerialPort::OneStop))
        {
            ui->statusBar->showMessage(tr("Can't set 1 stop bit to port %1, error code %2")
                                       .arg(serial.portName()).arg(serial.error()));
            return;
        }

        if (!serial.setFlowControl(QSerialPort::NoFlowControl))
        {
            ui->statusBar->showMessage(tr("Can't set no flow control to port %1, error code %2")
                                       .arg(serial.portName()).arg(serial.error()));
            return;
        }

        ui->statusBar->showMessage(tr("Status: Running, connected to port %1.")
                                   .arg(ui->serialNoComboBox->currentText()));

        QTest::qSleep(200);
        /* QString msg;
        msg = "handshake\r";
        serial.write(msg.toLocal8Bit());*/

    }
}

MainWindow::~MainWindow()
{
    writeSettings();
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    serialConnect();
}

void MainWindow::on_sendButton_clicked()
{
    serial.write(ui->requestLineEdit->text().toLocal8Bit());
}

void MainWindow::on_sendButtonrn_clicked()
{
    // QString data(ui->requestLineEdit->text()+"\r");
    //serial.write(data.toLocal8Bit());
    serial.write(ui->requestLineEdit->text().toLocal8Bit());
    serial.write("\r");
    writeSettings();
}

void MainWindow::readSettings()
{
    settings.beginGroup("/serialset");
    int index = ui->serialNoComboBox->findText(settings.value("/comNum").toString());
    ui->serialNoComboBox->setCurrentIndex(index);
    ui->baundRateComboBox->setCurrentIndex(settings.value("/baudRate").toInt());

    settings.endGroup();
}

void MainWindow::writeSettings()
{
    settings.beginGroup("/serialset");
    settings.setValue("/comNum", (ui->serialNoComboBox->currentText()));
    settings.setValue("/baudRate", ui->baundRateComboBox->currentIndex());
    settings.endGroup();
}

void MainWindow::create_gcode()
{
    gcode="x"+QString::number(x)+" y"+QString::number(y)+" z"+QString::number(z);
}

void MainWindow::on_pushButton_xup_clicked()
{
    // get_cur_coords();
    x+=ui->lineEdit_scale->text().toDouble();
    create_gcode();
    send_gcode();
    // get_cur_coords();
}

void MainWindow::on_pushButton_xdown_clicked()
{
    // get_cur_coords();
    x-=ui->lineEdit_scale->text().toDouble();
    create_gcode();
    send_gcode();
    //  get_cur_coords();
}

void MainWindow::on_pushButton_yup_clicked()
{
    //get_cur_coords();
    y+=ui->lineEdit_scale->text().toDouble();
    create_gcode();
    send_gcode();
    //get_cur_coords();
}

void MainWindow::on_pushButton_ydown_clicked()
{
    //   get_cur_coords();
    y-=ui->lineEdit_scale->text().toDouble();
    create_gcode();
    send_gcode();
    // get_cur_coords();
}



void MainWindow::on_pushButton_zup_clicked()
{
    //  get_cur_coords();
    z+=ui->lineEdit_scale->text().toDouble();
    create_gcode();
    send_gcode();
    // get_cur_coords();
}


void MainWindow::on_pushButton_zdown_clicked()
{
    //get_cur_coords();
    z-=ui->lineEdit_scale->text().toDouble();
    create_gcode();
    send_gcode();
    // get_cur_coords();
}


void MainWindow::on_pushButton_home_clicked()
{
    x=0.0;
    y=0.0;
    z=0.0;
    create_gcode();
    send_gcode();
}

void MainWindow::on_pushButton_home2_clicked()
{
    /*  x=0.0;
    y=0.0;
    z=-500.0;
    create_gcode();
    send_gcode();*/
}
void MainWindow::send_gcode()
{
    serial.write(gcode.toLocal8Bit());
    serial.write("\r");
    qDebug()<<gcode;
}

void MainWindow::on_disconnectButton_clicked()
{
    serial.clearError();
    serial.close();
}

void MainWindow::on_pushButton_zero_clicked()
{
    QString msg;
    msg = "c1\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
}

void MainWindow::on_pushButton_handshake_clicked()
{
    QString msg;
    msg = "c14\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
}

void MainWindow::on_pushButton_on_camera_incomand_on_clicked()
{
    QString msg;
    msg = "s3 1\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
}

void MainWindow::on_pushButton_on_camera_incomand_on_2_clicked()
{
    QString msg;
    msg = "s3 0\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
}

void MainWindow::on_pushButton_vacuum_on_clicked()
{
    QString msg;
    msg = "c 2\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
}

void MainWindow::on_pushButton_clicked()
{
    QString msg;
    msg = "c 3\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
}

void MainWindow::on_pushButton_auto_vac_clicked()
{
    QString msg;
    msg = "c 4\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
}

void MainWindow::on_pushButton_auto_vac_2_clicked()
{
    QString msg;
    msg = "c 5\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
}

void MainWindow::on_pushButton_save_settings_clicked()
{
    QString msg;
    msg = "c6 \r ";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
}

void MainWindow::on_pushButton_save_settings_2_clicked()
{
    QString msg;
    msg = "c6 \r ";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
}

void MainWindow::on_pushButton_cam_emul_clicked()
{
    QString msg;
    msg = "o:x2.1,y70.1\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
}

void MainWindow::on_pushButton_save_pos_clicked()
{
    QString msg;
    static int n_drop_obj=0;
    msg = "c9\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
}

void MainWindow::on_pushButton_show_array_clicked()
{
    QString msg;
    static int n_drop_obj=0;
    msg = "c7\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
}

void MainWindow::on_pushButton_demo_clicked()
{
    QString msg;
    static int n=0;
    n=ui->lineEdit_demo_n->text().toInt();
    msg = "c13 "+ui->lineEdit_demo_n->text()+"\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
    n++;
    ui->lineEdit_demo_n->setText(QString::number(n));
}

void MainWindow::on_pushButton_prch_accel_clicked()
{
    QString msg;
    static int n_drop_obj=0;
    n_drop_obj=ui->lineEdit_n_obj->text().toInt();
    msg = "c10 "+ui->lineEdit_n_obj->text()+"f10v"+ui->lineEdit_pr_accel->text()+"\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
    n_drop_obj++;
    ui->lineEdit_n_obj->setText(QString::number(n_drop_obj));
    // color_row(n_drop_obj+1, 1);
}

void MainWindow::on_pushButton_pr_set_delay_clicked()
{
    QString msg;
    static int n_drop_obj=0;
    n_drop_obj=ui->lineEdit_n_obj->text().toInt();
    msg = "c10 "+ui->lineEdit_n_obj->text()+"f11v"+ui->lineEdit_pr_delay->text()+"\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
    n_drop_obj++;
    ui->lineEdit_n_obj->setText(QString::number(n_drop_obj));
    //  color_row(n_drop_obj+1, 1);
}

void MainWindow::on_pushButton_pr_vacuum_clicked()
{
    QString msg;
    static int n_drop_obj=0;
    n_drop_obj=ui->lineEdit_n_obj->text().toInt();
    msg = "c10 "+ui->lineEdit_n_obj->text()+"f12";
    //c10 1f12v1y100\r
    if (ui->checkBox_pr_vacc_delay->isChecked()) msg+="v1y";
    else msg+="v0y";
    msg+=ui->lineEdit_pr_vacc_delay->text()+"\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
    n_drop_obj++;
    ui->lineEdit_n_obj->setText(QString::number(n_drop_obj));
    //color_row(n_drop_obj+1, 1);
}

void MainWindow::on_pushButton_pr_vac_clicked()
{
    QString msg;
    static int n_drop_obj=0;
    n_drop_obj=ui->lineEdit_n_obj->text().toInt();
    msg = "c8 "+ui->lineEdit_n_obj->text()+"f0\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
    n_drop_obj++;
    ui->lineEdit_n_obj->setText(QString::number(n_drop_obj));
}


void MainWindow::keyPressEvent( QKeyEvent * event )
{
    qDebug()<<"key: ";
    qDebug()<<event->key();
    if( event->key() == 56 )
    {
        qDebug()<<"up\r";
        on_pushButton_yup_clicked();
    }
    else if( event->key() == 50 )
    {
        qDebug()<<"down\r";
        on_pushButton_ydown_clicked();
    }
    else if( event->key() == 53 )
    {
        qDebug()<<"center\r";
        on_pushButton_home_clicked();

    }
    else if( event->key() == 54)
    {
        qDebug()<<"right\r";
        on_pushButton_xup_clicked();
    }
    else if( event->key() == 52 )
    {
        qDebug()<<"left\r";
        on_pushButton_xdown_clicked();
    }
    else if( event->key() == 55 )
    {
        qDebug()<<"leftup\r";
        on_pushButton_zup_clicked();
    }
    else if( event->key() == 49 )
    {
        qDebug()<<"leftdown\r";
        on_pushButton_zdown_clicked();
    }
}

void MainWindow::on_pushButton_save_position_clicked()
{
    QString msg;
    static int n_drop_obj=0;
    n_drop_obj=ui->lineEdit_n_obj->text().toInt();
    msg = "c8 "+ui->lineEdit_n_obj->text()+"f0\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
    n_drop_obj++;
    ui->lineEdit_n_obj->setText(QString::number(n_drop_obj));
}

void MainWindow::on_pushButton_end_cycle_clicked()
{
    QString msg;
    static int n_drop_obj=0;
    n_drop_obj=ui->lineEdit_n_obj->text().toInt();
    msg = "c8 "+ui->lineEdit_n_obj->text()+"f2\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
    n_drop_obj++;
    ui->lineEdit_n_obj->setText(QString::number(n_drop_obj));
}

void MainWindow::on_pushButton_next_cycle_clicked()
{
    QString msg;
    static int n_drop_obj=0;
    n_drop_obj=ui->lineEdit_n_obj->text().toInt();
    msg = "c8 "+ui->lineEdit_n_obj->text()+"f1\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
    n_drop_obj++;
    ui->lineEdit_n_obj->setText(QString::number(n_drop_obj));
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->tableWidget_steps->clear();
    QString msg;
    msg = "c15 0\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());

}

void MainWindow::on_pushButton_3_clicked()
{
    /*QString msg;
    msg = "c8 1f1\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());*/

    write_myfile();

}

void MainWindow::addtosteptable(int n, float a, float b, float c, int f)
{
    QTableWidgetItem *itm;
    itm = new QTableWidgetItem(QString::number(n));
    ui->tableWidget_steps->setItem(n,0,itm);
    itm = new QTableWidgetItem(QString::number(a));
    ui->tableWidget_steps->setItem(n,1,itm);
    itm = new QTableWidgetItem(QString::number(b));
    ui->tableWidget_steps->setItem(n,2,itm);
    itm = new QTableWidgetItem(QString::number(c));
    ui->tableWidget_steps->setItem(n,3,itm);
    if (f == 0)
    {
        itm = new QTableWidgetItem("next");
    }
    else if (f == 1)
    {
        itm = new QTableWidgetItem("cont");
    }
    else if (f == 2)
    {
        itm = new QTableWidgetItem("end");
    }
    else if (f == 10)
    {
        itm = new QTableWidgetItem("accel");
    }
    else if (f == 11)
    {
        itm = new QTableWidgetItem("delay");
    }
    else if (f == 12)
    {
        itm = new QTableWidgetItem("vacuum");
    }
    else
    {
        itm = new QTableWidgetItem("?");
    }

    ui->tableWidget_steps->setItem(n,4,itm);
    color_row(ui->lineEdit_n_obj->text().toInt(), 1);
    /*if (f!= 2 && n<100)
    {
    QString msg;
    msg = "c15 "+QString::number(n+1)+"\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
    }*/

}


void MainWindow::addtosettingtable(int n, float a)
{
    QTableWidgetItem *itm;
    //  itm = new QTableWidgetItem(QString::number(n));
    // ui->tableWidget_settings->setItem(n,0,itm);
    itm = new QTableWidgetItem(QString::number(a));
    ui->tableWidget_settings->setItem(n,1,itm);
}

void MainWindow::on_pushButton_get_table_clicked()
{
    send_stepsrow(0);
}

void MainWindow::send_stepsrow(int n)
{

    if (ui->tableWidget_steps->item(n,1) == 0x00 ||
            ui->tableWidget_steps->item(n,2)== 0x00  ||
            ui->tableWidget_steps->item(n,3)== 0x00)
        return;
    // ui->tableWidget_steps->item(n,0)->setBackgroundColor(QColor(0,0,255,127));
    // ui->tableWidget_steps->item(n,0)->setBackgroundColor(QColor(Qt::white));

    float x = ui->tableWidget_steps->item(n,1)->text().toFloat();
    float y = ui->tableWidget_steps->item(n,2)->text().toFloat();
    float z = ui->tableWidget_steps->item(n,3)->text().toFloat();
    QString flag = ui->tableWidget_steps->item(n,4)->text();
    int f;
    if (flag == "next")
    {
        f = 0;
    }
    else if (flag == "cont")
    {
        f = 1;
    }
    else if (flag == "end")
    {
        f = 2;
    }
    else if (flag == "accel")
    {
        f = 10;
    }
    else if (flag == "delay")
    {
        f = 11;
    }
    else if (flag == "vacuum")
    {
        f = 12;
    }
    else
    {
        f = -1;
    }
    QString msg;
    msg = "c16 "+QString::number(n)+"x"+QString::number(x)+"y"+QString::number(y)+"z"+QString::number(z)+"f"+QString::number(f)+"\r";
    //qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
}

void MainWindow::serialRead()
{
    serialData = serial.readAll();
    static QString str;

    str += serialData;
    ui->textEdit->insertPlainText(serialData);
    ui->textEdit->moveCursor(QTextCursor::End);
    serialData.clear();

    while (str.indexOf('\r')!=-1)
    {
        int i = str.indexOf('\r');
        qDebug()<<str.left(i);
        parse_data(str.left(i));
        str= str.mid(i+1,str.length());
    }
}

void MainWindow::parse_data(QString str)
{
    // qDebug()<< str;
    if (str.startsWith("step"))
    {

        str=str.remove(0,str.indexOf(':')+1);
        QString buf;

        buf = str.section(',',0,0);
        int n = buf.toInt();

        buf = str.section(',',1,1);
        float a  = buf.toFloat();

        buf = str.section(',',2,2);
        float b  = buf.toFloat();

        buf = str.section(',',3,3);
        float c  = buf.toFloat();

        buf = str.section(',',4,34);
        int f  = buf.toInt();

        addtosteptable(n,a,b,c,f);
    }
    else if (str.startsWith("setting"))
    {

        str=str.remove(0,str.indexOf(':')+1);
        QString buf;

        buf = str.section(',',0,0);
        int n = buf.toInt();

        buf = str.section(',',1,1);
        float a  = buf.toFloat();

        addtosettingtable(n,a);
    }
    else if (str.startsWith("stp_req"))
    {

        str=str.remove(0,str.indexOf(':')+1);

        int n = str.toInt();
        send_stepsrow(n);
    }
    else if (str.startsWith("sett_req"))
    {

        str=str.remove(0,str.indexOf(':')+1);

        int n = str.toInt();
        sendsettingrow(n);
    }
    else if (str.startsWith("cstp"))
    {

        str=str.remove(0,str.indexOf(':')+1);

        int n = str.toInt();
        color_row(n, 0);
    }

    else if (str.startsWith("curpos"))
    {

        str=str.remove(0,str.indexOf(':')+1);
        QString buf;

        buf = str.section(',',0,0);
        // qDebug()<<buf;
        x = buf.toFloat();
        // qDebug()<<"x:"<<x;


        buf = str.section(',',1,1);
        y  = buf.toFloat();
        //  qDebug()<<"y:"<<y;

        buf = str.section(',',2,2);
        z  = buf.toFloat();
        // qDebug()<<"z:"<<z;

    }
}

void MainWindow::color_row(int n, int color)
{
    qDebug()<<n;
    //n++;
    for (int i = 0; i<ui->tableWidget_steps->rowCount();i++)
    {
        ui->tableWidget_steps->item(i,0)->setBackgroundColor(QColor(Qt::white));
        ui->tableWidget_steps->item(i,1)->setBackgroundColor(QColor(Qt::white));
        ui->tableWidget_steps->item(i,2)->setBackgroundColor(QColor(Qt::white));
        ui->tableWidget_steps->item(i,3)->setBackgroundColor(QColor(Qt::white));
        ui->tableWidget_steps->item(i,4)->setBackgroundColor(QColor(Qt::white));
        if (ui->tableWidget_steps->item(i+1,1) == 0x00) break;

    }
    if (ui->tableWidget_steps->item(n,1) == 0x00)
    {
        return;
        QTableWidgetItem *itm;
        itm = new QTableWidgetItem(QString::number(n));
        ui->tableWidget_steps->setItem(n,0,itm);
        itm = new QTableWidgetItem(QString::number(0));
        ui->tableWidget_steps->setItem(n,1,itm);
        ui->tableWidget_steps->setItem(n,2,itm);
        ui->tableWidget_steps->setItem(n,3,itm);
        ui->tableWidget_steps->setItem(n,4,itm);

    }
    if (color == 0)
    {
        ui->tableWidget_steps->item(n,0)->setBackgroundColor(QColor(Qt::green));
        ui->tableWidget_steps->item(n,1)->setBackgroundColor(QColor(Qt::green));
        ui->tableWidget_steps->item(n,2)->setBackgroundColor(QColor(Qt::green));
        ui->tableWidget_steps->item(n,3)->setBackgroundColor(QColor(Qt::green));
        ui->tableWidget_steps->item(n,4)->setBackgroundColor(QColor(Qt::green));
    }
    else if (color == 1)
    {
        ui->tableWidget_steps->item(n,0)->setBackgroundColor(QColor(0,0,255,127));
        ui->tableWidget_steps->item(n,1)->setBackgroundColor(QColor(0,0,255,127));
        ui->tableWidget_steps->item(n,2)->setBackgroundColor(QColor(0,0,255,127));
        ui->tableWidget_steps->item(n,3)->setBackgroundColor(QColor(0,0,255,127));
        ui->tableWidget_steps->item(n,4)->setBackgroundColor(QColor(0,0,255,127));
    }
}

void MainWindow::on_tableWidget_steps_cellClicked(int row, int column)
{
    ui->lineEdit_n_obj->setText(QString::number(row));
    color_row(row, 1);
    if (ui->tableWidget_steps->item(row,column) == 0x00) return;
    if (column != 0) return;
    if (!ui->checkBox_debug->isChecked()) return;
    QString msg;
    msg = "c13 "+QString::number(row)+"\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
    ui->lineEdit_demo_n->setText(QString::number(row));

}

void MainWindow::on_tableWidget_steps_cellDoubleClicked(int row, int column)
{
    get_cur_coords();
}

void MainWindow::on_tableWidget_steps_cellPressed(int row, int column)
{

}

void MainWindow::on_tableWidget_steps_cellChanged(int row, int column)
{
    //   qDebug()<<"here";
    // send_row(row);
}

void MainWindow::on_pushButton_pr_set_accel_clicked()
{

}

void MainWindow::on_tableWidget_steps_cellEntered(int row, int column)
{

}

void MainWindow::get_cur_coords()
{
    QString msg;
    msg = "c17\r";
    serial.write(msg.toLocal8Bit());
}

void MainWindow::on_pushButton__pr_set_accel_clicked_clicked()
{
    get_cur_coords();
}


void MainWindow::write_myfile()
{

    QFile fileOut(ui->lineEdit_path_conf_file->text()); // Связываем объект с файлом fileout.txt
    if(fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream writeStream(&fileOut); // Создаем объект класса QTextStream

        for (int i = 0; i<100; i++)
        {
            if (ui->tableWidget_steps->item(i,1) == 0x00) break;
            writeStream << ui->tableWidget_steps->item(i,0)->text()<<','
                        << ui->tableWidget_steps->item(i,1)->text()<<','
                        << ui->tableWidget_steps->item(i,2)->text()<<','
                        << ui->tableWidget_steps->item(i,3)->text()<<','
                        << ui->tableWidget_steps->item(i,4)->text()<<"\r\n";
        }
        fileOut.close(); // Закрываем файл
    }

}


void MainWindow::read_myfile()
{
    QFile file(ui->lineEdit_path_conf_file->text());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {

        QString line = in.readLine();

        QString buf;

        buf = line.section(',',0,0);
        int n = buf.toInt();

        buf = line.section(',',1,1);
        float a  = buf.toFloat();

        buf = line.section(',',2,2);
        float b  = buf.toFloat();

        buf = line.section(',',3,3);
        float c  = buf.toFloat();

        buf = line.section(',',4,4);

        QTableWidgetItem *itm;
        itm = new QTableWidgetItem(QString::number(n));
        ui->tableWidget_steps->setItem(n,0,itm);
        itm = new QTableWidgetItem(QString::number(a));
        ui->tableWidget_steps->setItem(n,1,itm);
        itm = new QTableWidgetItem(QString::number(b));
        ui->tableWidget_steps->setItem(n,2,itm);
        itm = new QTableWidgetItem(QString::number(c));
        ui->tableWidget_steps->setItem(n,3,itm);
        itm = new QTableWidgetItem(buf);

        ui->tableWidget_steps->setItem(n,4,itm);
        color_row(ui->lineEdit_n_obj->text().toInt(), 1);

    }
}


void MainWindow::on_pushButton_ReadFile_clicked()
{
    read_myfile();
}


void MainWindow::on_pushButton_readsettings_clicked()
{
    QString msg = "c20\r";
    qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
}

void MainWindow::on_pushButton_writesettings_clicked()
{
    sendsettingrow(0);
}

void MainWindow::sendsettingrow(int n)
{
    if (ui->tableWidget_settings->item(n,0) == nullptr) return;

    float s = ui->tableWidget_settings->item(n,1)->text().toFloat();

    QString msg;
    msg = "c21 "+QString::number(n)+"s"+QString::number(s)+"\r";
    //qDebug()<<msg;
    serial.write(msg.toLocal8Bit());
}
