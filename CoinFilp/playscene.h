#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include "mycoin.h"
class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    PlayScene(int index);//index表示当前关卡号

    int levalIndex;//当前关卡号

    int gameArray[4][4];//关卡数据

    MyCoin *coinBtn[4][4];//金币数组

    bool isWin=true;//是否胜利
public:
    void saveGame();

    bool loadGame();

    void updateCoinStates();

signals:
    void ChooseSceneBack();//返回信号
    void levalNext(int index);//下一关
    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
private:

};

#endif // PLAYSCENE_H
