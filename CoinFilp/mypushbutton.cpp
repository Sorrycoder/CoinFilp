#include "mypushbutton.h"
#include<QDebug>
#include <QSequentialAnimationGroup>

MyPushButton::MyPushButton(QString normalImg, QString pressImg)
{
    this->normalPath=normalImg;
    this->pressPath=pressImg;

    //加载图片
    this->setButton(this->normalPath);

}



void MyPushButton::zoom(bool up)
{

    //创建向下对象
    //创建动画对象
    QPropertyAnimation *animationDown=new QPropertyAnimation(this,"geometry");
    //间隔时间
    animationDown->setDuration(200);
    //起始位置
    animationDown->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //结束位置,y上10
    animationDown->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    //设置缓和曲线，弹跳效果
    animationDown->setEasingCurve(QEasingCurve::OutBounce);

    //创建向上对象
    QPropertyAnimation *animationUp=new QPropertyAnimation(this,"geometry");
    animationUp->setDuration(200);
    animationUp->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    animationUp->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));
    animationUp->setEasingCurve(QEasingCurve::OutBounce);

    //创建顺序动画组
    QSequentialAnimationGroup *bounceAnimGroup=new QSequentialAnimationGroup(this);

    if(up){
        bounceAnimGroup->addAnimation(animationUp);
        bounceAnimGroup->addAnimation(animationDown);
    }else{
        bounceAnimGroup->addAnimation(animationDown);
        bounceAnimGroup->addAnimation(animationUp);
    }

    bounceAnimGroup->start(QAbstractAnimation::DeleteWhenStopped);
}



void MyPushButton::mousePressEvent(QMouseEvent *event)//按下切换图片
{
    if(this->pressPath!="")
        //有切换的图片
        this->setButton(this->pressPath);
    QPushButton::mousePressEvent(event);
}

void MyPushButton::mouseReleaseEvent(QMouseEvent *event)//按下还原图片
{
    if(this->pressPath!="")
        //有切换的图片
        this->setButton(this->normalPath);
    QPushButton::mouseReleaseEvent(event);
}
void MyPushButton::setButton(QString picPath)
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
