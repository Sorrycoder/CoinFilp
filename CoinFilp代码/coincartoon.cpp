#include "coincartoon.h"

#include <qtimer.h>
coinCartoon::coinCartoon(QString btnImg)
{
    this->setButton(btnImg);//给按钮自定义样式

    //创建定时器
    this->timer=new QTimer(this);
    //监听翻转
    connect(timer,&QTimer::timeout,[=](){//点击后开始计时，每30ms超时一次，改变一次图标，播放完重置图下标，
        QString str=QString(":/res/Coin000%1.png").arg(this->min++);
        this->setButton(str);
    });

}




void coinCartoon::setButton(QString picPath)
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


