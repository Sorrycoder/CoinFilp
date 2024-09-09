#ifndef CHOOSELEVELSCENE_H
#define CHOOSELEVELSCENE_H

#include <QMainWindow>
#include"playscene.h"
#include"mypushbutton.h"
class ChooseLevelScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChooseLevelScene(QWidget *parent = nullptr);

public:
    PlayScene *playScene;//游戏界面


    MyPushButton *LevalBtn[20];//存储选关按键








signals:
    void ChooseSceneBack();//返回信号

    void initLeval();

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // CHOOSELEVELSCENE_H
