#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include <QTimer>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    MyCoin(QString btnImg);

    void setButton(QString picPath);//设置自定义按键

    int posX;
    int posY;
    bool flag;

    void changeFlag();//翻转效果

    QTimer *timer1;//正到反
    QTimer *timer2;//反到正
    int min=1;//图片下标
    int max=8;

    bool isAnimation=false;//是否在翻转动画中

    bool isWin=false;//是否胜利


signals:

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // MYCOIN_H
