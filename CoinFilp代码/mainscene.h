#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QMainWindow>
#include"chooselevelscene.h"
#include "coincartoon.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainScene;
}
QT_END_NAMESPACE

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);
    ~MainScene();


    int index=1;//当前动画图片下标

    void updateCoinImage();//更新动画图片


private:
    Ui::MainScene *ui;
    MyPushButton *coinButton;



    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

public:
    ChooseLevelScene *chooseScene;
};
#endif // MAINSCENE_H
