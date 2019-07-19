#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

   // QSettings *settings= new QSettings("config.ini", QSettings::IniFormat);//("BelLab", "MySerial");
   // QSettings *settings = new QSettings("ABC","Serial");
  //  QSettings QSettings
private slots:

    void on_connectButton_clicked();

    void serialRead();

    void on_sendButton_clicked();

    void on_sendButtonrn_clicked();

    void on_pushButton_xup_clicked();

    void on_pushButton_xdown_clicked();

    void on_pushButton_yup_clicked();

    void on_pushButton_ydown_clicked();

    void on_disconnectButton_clicked();

    void on_pushButton_zup_clicked();

    void on_pushButton_zdown_clicked();

    void on_pushButton_home_clicked();

    void on_pushButton_home2_clicked();

    void on_pushButton_zero_clicked();

    void on_pushButton_handshake_clicked();

    void on_pushButton_on_camera_incomand_on_clicked();

    void on_pushButton_on_camera_incomand_on_2_clicked();

    void on_pushButton_vacuum_on_clicked();

    void on_pushButton_clicked();

    void on_pushButton_auto_vac_clicked();

    void on_pushButton_auto_vac_2_clicked();

    void on_pushButton_save_settings_clicked();

    void on_pushButton_save_settings_2_clicked();

    void on_pushButton_cam_emul_clicked();

    void on_pushButton_save_position_clicked();    

    void on_pushButton_save_pos_clicked();

    void on_pushButton_show_array_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_demo_clicked();

    void on_pushButton_prch_accel_clicked();

    void on_pushButton_pr_set_accel_clicked();

    void on_pushButton_pr_set_delay_clicked();

    void on_pushButton_pr_vacuum_clicked();

    void on_pushButton_pr_vac_clicked();

    void keyPressEvent( QKeyEvent * event );

    void on_pushButton_end_cycle_clicked();

    void on_pushButton_next_cycle_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_get_table_clicked();

    void on_tableWidget_steps_cellClicked(int row, int column);

    void on_tableWidget_steps_cellDoubleClicked();

    void on_tableWidget_steps_cellPressed();

    void on_tableWidget_steps_cellChanged();

    void on_tableWidget_steps_cellEntered();

    void on_pushButton__pr_set_accel_clicked_clicked();

    void on_pushButton_ReadFile_clicked();

    void on_pushButton_readsettings_clicked();

    void on_pushButton_writesettings_clicked();

private:

    void serialConnect();
    void readSettings();
    void writeSettings();

    void create_gcode();
    void send_gcode();

    void parse_data(QString str);
    void addtosteptable(int n, float a, float b, float c, int f);
    void addtosettingtable(int n, float a);
    void send_stepsrow(int n);
    void color_row(int n, int color);
    void get_cur_coords();
    void write_myfile();
    void read_myfile();
    void sendsettingrow(int n);

    Ui::MainWindow *ui;
    QSerialPort serial;
    QString serialData;
    QString gcode;

    double x=0;
    double y=0;
    double z=0;
};

#endif // MAINWINDOW_H
