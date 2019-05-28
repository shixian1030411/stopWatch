#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化计数器
    QTime showTime(0,0,0,0);
    QString showStr = showTime.toString("hh:mm:ss:zzz");
    this->ui->lcdNumber->display(showStr);

    //为timeout信息号绑定槽函数
    connect(&this->ptimer,SIGNAL(timeout()),this,SLOT(updateTimeAndDisplay()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTimeAndDisplay()
{
    //获取当前时间
    QTime current = QTime::currentTime();
    //计算基准时间与当前时间时间的差值就是我们计时的时长
    int t = this->baseTime.msecsTo(current);
    //将计时的时长转化为LCD的显示格式
    QTime showTime(0,0,0,0);
    showTime = showTime.addMSecs(t);
    showStr = showTime.toString("hh:mm:ss:zzz");
    this->ui->lcdNumber->display(showStr);
}

void MainWindow::on_btn_start_clicked()
{
    //基准时间为当前时间
    this->baseTime = QTime::currentTime();
    this->ptimer.start(1);

    this->ui->btn_start->setEnabled(false);
    this->ui->btn_pause->setEnabled(true);
    this->ui->btn_stop->setEnabled(true);
    this->ui->btn_log->setEnabled(true);

    this->ui->btn_stop->setText("停止");
    this->ui->btn_pause->setText("暂停");
}

void MainWindow::on_btn_stop_clicked()
{
    if(this->ui->btn_stop->text() == "停止"){
        this->ptimer.stop();

        this->ui->btn_stop->setText("清零");
        this->ui->btn_pause->setEnabled(false);
        this->ui->btn_log->setEnabled(false);
        this->ui->btn_start->setEnabled(true);
    }else{
        this->ui->lcdNumber->display("00:00:00:000");
        this->ui->textBrowser->clear();
        this->ui->btn_stop->setText("停止");
        this->ui->btn_start->setEnabled(true);
        this->ui->btn_stop->setEnabled(false);
    }

}

void MainWindow::on_btn_pause_clicked()
{
    static QTime pauseTime;

    if(this->ui->btn_pause->text() == "暂停"){
        pauseTime = QTime::currentTime();
        this->ptimer.stop();
        this->ui->btn_pause->setText("继续");
    }else{
        QTime cut = QTime::currentTime();
        int  t = pauseTime.msecsTo(cut);
        this->baseTime = this->baseTime.addMSecs(t);
        this->ptimer.start();
        this->ui->btn_pause->setText("暂停");
    }

}

void MainWindow::on_btn_log_clicked()
{
    this->ui->textBrowser->append(this->showStr);
}
