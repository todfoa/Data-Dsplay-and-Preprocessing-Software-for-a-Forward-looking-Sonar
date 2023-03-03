#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"mythread.h"
#include <QLabel>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QImage ImageCenter(QImage  qimage,QLabel *qLabel);//调整图片比例



private slots:

    void onsendimg();

    void ontimeout();
    void on_action_O_triggered(); //打开声呐文件

    void on_playButton_clicked(); //播放

    void on_stopButton_clicked(); //暂停







    void on_actionblue_triggered();

    void on_actionGaussian_triggered();

    void on_actionMedian_triggered();

    void on_blueButton_clicked();

    void on_GaussianButton_clicked();

    void on_MedianButton_clicked();



    void on_EqualizedButton_clicked();

    void on_equalizeHist_triggered();


    void on_gammaButton_clicked();

    void on_gamma_triggered();

    void on_logButton_clicked();

    void on_actionlog_triggered();

    void on_CannyButton_clicked();

    void on_actionCanny_triggered();

    void on_LapButton_clicked();

    void on_actionLaplacian_triggered();

    void on_erodButton_clicked();

    void on_actionerode_triggered();

    void on_dilateButton_clicked();

    void on_openButton_clicked();

    void on_closeButton_2_clicked();

    void on_ostuButton_clicked();

    void on_actionJet_triggered();

    void on_jetButton_clicked();



    void on_hotButton_clicked();

    void on_pushButton_clicked();

    void on_actionabout_triggered();

    void on_MaxEntropyButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    MyThread thread;
    QTimer *myTimer;

   QMessageBox customMsgBox;
};
#endif // MAINWINDOW_H
