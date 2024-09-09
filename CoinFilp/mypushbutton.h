#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include<QPropertyAnimation>
class MyPushButton : public QPushButton
{
    Q_OBJECT
public:


    MyPushButton(QString normalImg,QString pressImg="");//正常显示图片路径，按下后切换的图片路径

    QString normalPath;
    QString pressPath;

    void zoom(bool up);//跳跃函数

    void setButton(QString picPath);//设置自定义按键


signals:


    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // MYPUSHBUTTON_H
