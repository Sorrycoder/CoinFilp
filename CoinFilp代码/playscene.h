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
    //弹窗stylesheet
    QString styleSheet = R"(
                            QMessageBox {
                                background-color: rgb(245, 222, 179); /* 背景颜色 */
                                color: rgb(139, 69, 19); /* 文字颜色 */
                                font-size: 16px;
                            }
                            QMessageBox QPushButton {
                                background-color: rgb(255, 160, 122); /* 按钮背景颜色 */
                                border: 1px solid rgb(205, 133, 63); /* 边框颜色 */
                                padding: 5px 10px;
                                border-radius: 5px;
                                color: rgb(255, 255, 255); /* 文字颜色 */
                                font-size: 14px;
                                min-width: 100px;
                            }
                            QMessageBox QPushButton:hover {
                                background-color: rgb(255, 140, 0); /* 鼠标悬停时的背景颜色 */
                            }
                            QMessageBox QPushButton:pressed {
                                background-color: rgb(255, 99, 71); /* 按下时的背景颜色 */
                            }
                        )";

};

#endif // PLAYSCENE_H
