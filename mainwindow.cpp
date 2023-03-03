#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mythread.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 软件图标
    setWindowIcon(QIcon("C:/Users/Polaris/Documents/Qtcode/build-sonarfile-Desktop_Qt_5_14_2_MinGW_64_bit-Debug/img/1.jpeg"));
    this->setWindowTitle("某前视声纳数据显示与预处理软件1.0");

    customMsgBox.setWindowTitle(tr("关于本软件"));
    customMsgBox.addButton(tr("好的"),QMessageBox::ActionRole);
   // customMsgBox.setIconPixmap(QPixmap("C:/Users/Polaris/Documents/Qtcode/build-sonarfile-Desktop_Qt_5_14_2_MinGW_64_bit-Debug/img/1.jpeg"));
    customMsgBox.setText(tr("欢迎使用 前视声纳数据显示与预处理软件1.0！\n"
                                              "——By thz"));
//    this->setStyleSheet()
    ui->label_show->setStyleSheet("QLabel{background-color:rgb(250,250,250);}");

    myTimer = new QTimer(this);
    myTimer->setTimerType(Qt::TimerType::PreciseTimer);
    connect(myTimer,SIGNAL(timeout()),this,SLOT(ontimeout()));

    connect(&thread, &MyThread::sendimg, this, &MainWindow::onsendimg);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//on_click(){
//    thread.ype =1

void MainWindow::onsendimg()
{
//    if(type=1)
//    {
//        applyColorMap(, dst, COLORMAP_JET);
//    }

    QImage Image=ImageCenter(thread.m_img,ui->label_show);
    ui->label_show->setPixmap(QPixmap::fromImage(thread.m_img));
    ui->label_show->setAlignment(Qt::AlignCenter);

}

//图片居中显示,图片大小与label大小相适应
QImage MainWindow::ImageCenter(QImage  qimage,QLabel *qLabel)
{
    QImage image;
    QSize imageSize = qimage.size();
    QSize labelSize = qLabel->size();

    double dWidthRatio = 1.0*imageSize.width() / labelSize.width();
    double dHeightRatio = 1.0*imageSize.height() / labelSize.height();
            if (dWidthRatio>dHeightRatio)
            {
                image = qimage.scaledToWidth(labelSize.width());
            }
            else
            {
                image = qimage.scaledToHeight(labelSize.height());
            }
            return image;

    }

void MainWindow::ontimeout()
{
    thread.m_semaphore.release();

}

// 选择文件按钮
void MainWindow::on_action_O_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("选择文件"), ".", tr("声呐数据(*.oculus)"));
    thread.filename = filename;
    thread.start();
}

//播放按钮
void MainWindow::on_playButton_clicked()
{
    //启动定时器
    //时间间隔为100ms
    //每隔100ms，定时器myTimer自动触发timeout()
    //如果定时器没有激活，才启动,防止多次点击start按钮出现错误

    if (myTimer->isActive() == false)
        {
           myTimer->start(100);
        }
}

// 暂停播放按钮
void MainWindow::on_stopButton_clicked()
{
    if(myTimer->isActive() == true)
        {
            myTimer->stop();

         }
}

//伪色彩按钮

void MainWindow::on_jetButton_clicked()
{
    if(thread.type1==0){
         thread.type1=1;
    }
    else{
    thread.type1=0;
    }
}


void MainWindow::on_hotButton_clicked()
{
    if(thread.type1==0){
         thread.type1=2;
    }
    else{
    thread.type1=0;
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(thread.type1==0){
         thread.type1=3;
    }
    else{
    thread.type1=0;
    }
}


void MainWindow::on_actionJet_triggered()
{
    if(thread.type1==0){
         thread.type1=1;
    }
    else{
    thread.type1=0;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(thread.type1==0){
         thread.type1=4;
    }
    else{
    thread.type1=0;
    }
}


/********************************* 平滑去噪***************************************************/


//均值滤波
void MainWindow::on_actionblue_triggered()
{
    if(thread.type2==0){
         thread.type2=1;
    }
    else{
    thread.type2=0;
    }
}

void MainWindow::on_blueButton_clicked()
{
    if(thread.type2==0){
         thread.type2=1;
    }
    else{
    thread.type2=0;
    }
}

//高斯滤波
void MainWindow::on_actionGaussian_triggered()
{
    if(thread.type2==0){
         thread.type2=2;
    }
    else{
    thread.type2=0;
    }
}

void MainWindow::on_GaussianButton_clicked()
{
    if(thread.type2==0){
         thread.type2=2;
    }
    else{
    thread.type2=0;
    }
}

//中值滤波
void MainWindow::on_actionMedian_triggered()
{
    if(thread.type2==0){
         thread.type2=3;
    }
    else{
    thread.type2=0;
    }
}

void MainWindow::on_MedianButton_clicked()
{
    if(thread.type2==0){
         thread.type2=3;
    }
    else{
    thread.type2=0;
    }
}

/******************************图像增强*********************************************/

//直方图均衡

void MainWindow::on_EqualizedButton_clicked()
{
    if(thread.type3==0){
         thread.type3=1;
    }
    else{
    thread.type3=0;
    }
}
void MainWindow::on_equalizeHist_triggered()
{
    if(thread.type3==0){
         thread.type3=1;
    }
    else{
    thread.type3=0;
    }
}

//伽马变换

void MainWindow::on_gammaButton_clicked()
{
    if(thread.type4==0){
         thread.type4=1;
    }
    else{
    thread.type4=0;
    }
}


void MainWindow::on_gamma_triggered()
{
    if(thread.type4==0){
         thread.type4=1;
    }
    else{
    thread.type4=0;
    }
}

//对数变换
void MainWindow::on_logButton_clicked()
{
    if(thread.type5==0){
         thread.type5=1;
    }
    else{
    thread.type5=0;
    }
}

void MainWindow::on_actionlog_triggered()
{
    if(thread.type5==0){
         thread.type5=1;
    }
    else{
    thread.type5=0;
    }
}
/******************************图像分割*********************************************/
//边缘检测

//canny算子
void MainWindow::on_CannyButton_clicked()
{
    if(thread.type6==0){
         thread.type6=1;
    }
    else{
    thread.type6=0;
    }
}

void MainWindow::on_actionCanny_triggered()
{
    if(thread.type6==0){
         thread.type6=1;
    }
    else{
    thread.type6=0;
    }
}

//laplacian边缘检测

void MainWindow::on_LapButton_clicked()
{
    if(thread.type6==0){
         thread.type6=2;
    }
    else{
    thread.type6=0;
    }
}

void MainWindow::on_actionLaplacian_triggered()
{
    if(thread.type6==0){
         thread.type6=2;
    }
    else{
    thread.type6=0;
    }
}

void MainWindow::on_ostuButton_clicked()
{
    if(thread.type8==0){
         thread.type8=1;
    }
    else{
    thread.type8=0;
    }
}

void MainWindow::on_MaxEntropyButton_clicked()
{
    if(thread.type8==0){
         thread.type8=2;
    }
    else{
    thread.type8=0;
    }
}


/******************************图像形态学处理*********************************************/
//腐蚀
void MainWindow::on_erodButton_clicked()
{
    if(thread.type7==0){
         thread.type7=1;
    }
    else{
    thread.type7=0;
    }

}

void MainWindow::on_actionerode_triggered()
{
    if(thread.type7==0){
         thread.type7=1;
    }
    else{
    thread.type7=0;
    }
}

//膨胀
void MainWindow::on_dilateButton_clicked()
{
    if(thread.type7==0){
         thread.type7=2;
    }
    else{
    thread.type7=0;
    }
}
//开运算
void MainWindow::on_openButton_clicked()
{
    if(thread.type7==0){
         thread.type7=3;
    }
    else{
    thread.type7=0;
    }
}

//闭运算
void MainWindow::on_closeButton_2_clicked()
{
    if(thread.type7==0){
         thread.type7=4;
    }
    else{
    thread.type7=0;
    }
}



void MainWindow::on_actionabout_triggered()
{
    customMsgBox.show();

    customMsgBox.exec();
}




