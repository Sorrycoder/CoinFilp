#include "mycoin.h"



MyCoin::MyCoin(QString btnImg)
{
    this->setButton(btnImg);//给按钮自定义样式

    //创建定时器
    this->timer1=new QTimer(this);
    this->timer2=new QTimer(this);

    //监听金币翻银币
    connect(timer1,&QTimer::timeout,[=](){//点击后开始计时，每30ms超时一次，改变一次图标，播放完重置图下标，
        QString str=QString(":/res/Coin000%1.png").arg(this->min++);
        this->setButton(str);
        if(this->min>this->max){
            min=1;
            this->timer1->stop();
            isAnimation=false;
        }
    });

    //监听银币
    connect(timer2,&QTimer::timeout,[=](){//点击后开始计时，每30ms超时一次，改变一次图标，播放完重置图下标，
        QString str=QString(":/res/Coin000%8.png").arg(this->max--);
        this->setButton(str);
        if(this->max<this->min){
            max=8;
            this->timer2->stop();
            isAnimation=false;
        }
    });
}

void MyCoin::changeFlag()//点击改变硬币状态
{
    if(flag){
        this->flag=false;
        this->timer1->start(30);//每30ms触发一次
    }
    else{
        this->timer2->start(30);
        this->flag=true;
    }
    isAnimation=true;
}

void MyCoin::mousePressEvent(QMouseEvent *event)
{
    if(isAnimation||isWin)//正在翻转/胜利,禁用鼠标
    {
        return;//不再响应
    }else
    {
        QPushButton::mousePressEvent(event);
    }
}

void MyCoin::setButton(QString picPath)
{
    QPixmap pic;
    bool ret=pic.load(picPath);
    if(!ret){
        QString str=QString("图片加载失败，失败的路径是： %1").arg(picPath);
        qDebug()<<str;
    }
    //设置按钮尺寸
    this->setFixedSize(pic.width(),pic.height());
    //无边框,圆形
    this->setStyleSheet("QPushButton{border:0px;}");
    //设置图标
    this->setIcon(pic);
    //设置图标大小
    this->setIconSize(QSize(pic.width(),pic.height()));
}


