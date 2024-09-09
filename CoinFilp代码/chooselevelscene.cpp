#include "chooselevelscene.h"
#include<QMenu>
#include<QAction>
#include <qmenubar.h>
#include<QPainter>
#include <QLabel>
#include <QSoundEffect>
#include"mypushbutton.h"
ChooseLevelScene::ChooseLevelScene(QWidget *parent)
    : QMainWindow{parent}
{

    //准备音效
    QSoundEffect *TapSound=new QSoundEffect(this);
    TapSound->setSource(QUrl::fromLocalFile(":/res/tap1.wav"));
    QSoundEffect *backSound=new QSoundEffect(this);
    backSound->setSource(QUrl::fromLocalFile(":/res/BackButtonSound.wav"));



    //设置固定尺寸
    this->setFixedSize(320,588);
    //设置标题
    this->setWindowTitle("选择关卡");
    //设置图标
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));

    //菜单
    QMenuBar * bar=new QMenuBar();
    this->setMenuBar(bar);
    QMenu *menu=bar->addMenu("开始");
    QAction *actQuit=menu->addAction("退出");
    connect(actQuit,&QAction::triggered,this,&ChooseLevelScene::close);

    //返回按钮
    MyPushButton *backBtn=new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());
    connect(backBtn,&MyPushButton::clicked,[=](){
        emit ChooseSceneBack();
        backSound->play();

    });//按下按钮发出信号，主界面能听到

    playScene=NULL;
    //选关20个按钮
    for(int i=0;i<20;i++){
        MyPushButton *btnMenu=new MyPushButton(":/res/LevelIcon.png");
        btnMenu->setParent(this);
        LevalBtn[i]=btnMenu;
        btnMenu->move(25+(i%4)*70,150+(i/4)*70);//矩阵位置
        connect(btnMenu,&MyPushButton::clicked,[=](){
            TapSound->play();
            qDebug()<<"所选择的关卡是：第"<<i+1<<"关";
            playScene=new PlayScene(i+1);

            this->playScene->setGeometry(this->geometry());
            playScene->show();
            this->hide();

            //监听选关,如果在外侧，等于拿空对象连接信号槽，错误
            connect(this->playScene,&PlayScene::ChooseSceneBack,this,[=](){
                this->setGeometry(this->playScene->geometry());
                this->show();
                playScene->hide();
                delete playScene;
                playScene=NULL;
            });
            //监听到按下下一关信号,按下对应键位
            connect(playScene,&PlayScene::levalNext,[this](int index){
                qDebug()<<"收到下一关索引"<<index;
                playScene->hide();
                LevalBtn[index]->click();
            });
        });

        //按钮显示数字
        QLabel *label=new QLabel;
        label->setParent(btnMenu);
        label->setFixedSize(btnMenu->width(),btnMenu->height());
        label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        label->setText(QString::number(i+1));
        label->setAttribute(Qt::WA_TransparentForMouseEvents);//过滤鼠标事件
    }
}




void ChooseLevelScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pic;
    pic.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pic);
    pic.load(":/res/Title.png");
    painter.drawPixmap(this->width()/2-pic.width()/2,30,pic.width(),pic.height(),pic);

}
