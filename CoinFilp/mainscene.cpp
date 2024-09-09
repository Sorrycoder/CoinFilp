#include "mainscene.h"
#include "ui_mainscene.h"
#include"mypushbutton.h"
#include <QPainter>
#include<QDebug>
#include<QTimer>
#include<QSoundEffect>
MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);
    //设置固定尺寸
    this->setFixedSize(320,588);
    //设置标题
    this->setWindowTitle("翻金币主场景");
    //设置图标
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    // 自动旋转的金币按钮
    coinButton = new MyPushButton(":/res/Coin0001.png");
    coinButton->setParent(this);
    coinButton->move(this->width() *0.5 - coinButton->width()-15, this->height() * 0.3);
    // 定时器
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainScene::updateCoinImage);
    timer->start(50); // 每30毫秒更新一次


    //实例化第二个选关场景
    chooseScene=new ChooseLevelScene;

    //准备音效
    QSoundEffect *startSound=new QSoundEffect(this);
    startSound->setSource(QUrl::fromLocalFile(":/res/TapButtonSound.wav"));

    //开始按钮
    MyPushButton *btnStart=new MyPushButton(":/res/MenuSceneStartButton.png");
    btnStart->setParent(this);
    btnStart->move(this->width()*0.5-btnStart->width()*0.5,this->height()*0.6);
    connect(btnStart,&MyPushButton::clicked,this,[=](){
        //btnStart->zoom1();
        //btnStart->zoom2();

        //音效
        startSound->play();
        btnStart->zoom(true);

        //延时进入
        QTimer::singleShot(500,[=](){
            this->chooseScene->setGeometry(this->geometry());//选关位置和主页一致
            this->chooseScene->show();//选关显示
            this->hide();//主界面隐藏
        });

        //监听到选关界面发射ChooseSceneBack信号，
        connect(this->chooseScene,&ChooseLevelScene::ChooseSceneBack,[=](){
            QTimer::singleShot(300,[=](){
                this->setGeometry(this->chooseScene->geometry());
                this->chooseScene->hide();//选关隐藏
                this->show();//主界面出现
            });
        });
    }); 
}

MainScene::~MainScene()
{
    delete ui;
}

void MainScene::paintEvent(QPaintEvent *event)
{
    //背景
    QPainter painter(this);
    QPixmap pic;
    pic.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pic);
    //标题
    pic.load(":/res/Title.png");
    pic=pic.scaled(pic.width()*0.5,pic.height()*0.5);
    painter.drawPixmap(10,30,pic);

}

void MainScene::updateCoinImage()//翻转效果
{
    if (this->index >= 8) this->index = 1; // 重置索引
    QString imagePath = QString(":/res/Coin000%1.png").arg(index++);
    QPixmap pic;
    bool ret = pic.load(imagePath);
    if (!ret) {
        QString str = QString("图片加载失败，失败的路径是：%1").arg(imagePath);
        qDebug() << str;
        return;
    }

    QPixmap scaledPic = pic.scaled(138, 132, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // 设置按钮尺寸
    coinButton->setFixedSize(scaledPic.width(), scaledPic.height());
    // 无边框,圆形
    coinButton->setStyleSheet("QPushButton{border:0px;}");
    // 设置图标
    coinButton->setIcon(QIcon(scaledPic));
    // 设置图标大小
    coinButton->setIconSize(QSize(scaledPic.width(), scaledPic.height()));


}
