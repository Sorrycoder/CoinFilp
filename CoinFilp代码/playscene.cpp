#include "playscene.h"

#include <MyPushButton.h>
#include <QLabel>
#include <QMenuBar>
#include <QSoundEffect>
#include <QTimer>
#include <qpainter.h>
#include"dataconfig.h"
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include<QMessageBox>
#include "mycoin.h"

PlayScene::PlayScene(int index)
{
    this->levalIndex=index;
    qDebug()<<"进入第"<<this->levalIndex<<"关";
    //准备音效
    QSoundEffect *backSound=new QSoundEffect(this);
    backSound->setSource(QUrl::fromLocalFile(":/res/BackButtonSound.wav"));
    QSoundEffect *FilpSound=new QSoundEffect(this);
    FilpSound->setSource(QUrl::fromLocalFile(":/res/ConFlipSound.wav"));
    QSoundEffect *WinSound=new QSoundEffect(this);
    WinSound->setSource(QUrl::fromLocalFile(":/res/end.wav"));

    //设置固定尺寸
    this->setFixedSize(320,588);
    //设置标题
    this->setWindowTitle("第"+QString::number(this->levalIndex)+"关");
    //设置图标
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    //菜单
    QMenuBar * bar=new QMenuBar();
    this->setMenuBar(bar);
    QAction *actSave=bar->addAction("保存");
    QAction *actLoad=bar->addAction("读取");
    //actSave->setIcon(QIcon(":/res/save.png"));//点击保存
    connect(actSave, &QAction::triggered, [=](){
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("确认保存");
        msgBox.setText("确定要保存当前进度吗？");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        msgBox.setStyleSheet(styleSheet);
        int response = msgBox.exec();
        if (response == QMessageBox::Yes) {
            saveGame();
        }
    });
    connect(actLoad, &QAction::triggered, [=](){//点击读档
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("确认读档");
        msgBox.setText("确定要覆盖当前进度吗？");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        msgBox.setStyleSheet(styleSheet);
        int response = msgBox.exec();
        if (response == QMessageBox::Yes) {
            loadGame();
        }
    });


        //初始化关卡数据
        dataConfig config;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            this->gameArray[i][j]=config.mData[this->levalIndex][i][j];
        }
    }

    //胜利图片
    QLabel *winLabel=new QLabel;
    winLabel->setParent(this);
    QPixmap pic;
    pic.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setPixmap(pic);
    winLabel->setGeometry(0,0,pic.width(),pic.height());
    winLabel->move(this->width()/2-pic.width()/2,-pic.height());//先放到窗口外


    //返回按钮
    MyPushButton *backBtn=new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());
    connect(backBtn,&MyPushButton::clicked,[=](){
        qDebug()<<"返回选关界面";
        backSound->play();
        QTimer::singleShot(300,this,[=](){
            emit this->ChooseSceneBack();

        });
    });//按下按钮发出信号，选关界面监听

    //显示关卡号label
    QLabel *label=new QLabel(this);
    label->setText("关卡:"+QString::number(this->levalIndex));
    label->setGeometry(QRect(30,this->height()-50,this->width(),50));
    QFont font("华文新魏",20);
    label->setFont(font);
    label->setAttribute(Qt::WA_TransparentForMouseEvents);//过滤鼠标事件

    //创建金币背景
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            QLabel *label=new QLabel(this);
            QPixmap pic;
            pic.load(":/res/BoardNode.png");
            label->setGeometry(0,0,pic.width(),pic.height());
            label->setPixmap(pic);
            //this->width()*0.5-label->width()*2,居中
            label->move(this->width()*0.5-label->width()*2+i*pic.width(),200+j*pic.height());

            //创建金币
            QString str;
            if(this->gameArray[i][j]==1){
                str=":/res/Coin0001.png";
            }else{
                str=":/res/Coin0008.png";
            }
            MyCoin *coin=new MyCoin(str);
            coin->setParent(label);
            coin->move(label->width()/2-coin->width()/2,label->height()/2-coin->height()/2);//上下居中
            coin->posX=i;
            coin->posY=j;
            coin->flag=gameArray[i][j];//状态初始化
            coinBtn[i][j]=coin;

            //点击硬币调用change状态
            connect(coin,&MyCoin::clicked,[=](){
                //点击的时候禁用所有按键，防止bug
                for(int i=0;i<4;i++){
                    for(int j=0;j<4;j++){
                        this->coinBtn[i][j]->isWin=true;
                    }
                }
                FilpSound->play();
                coin->changeFlag();
                gameArray[i][j]=gameArray[i][j]==0? 1:0;//储存当前状态

                //翻转周围硬币
                QTimer::singleShot(300,this,[=](){
                    if(coin->posX+1<=3){//右侧可以翻的时候翻
                        this->coinBtn[coin->posX+1][coin->posY]->changeFlag();
                        gameArray[coin->posX+1][coin->posY]=gameArray[coin->posX+1][coin->posY]==0? 1:0;//储存当前状态
                    }
                    if(coin->posX-1>=0){//左侧可以翻的时候
                        this->coinBtn[coin->posX-1][coin->posY]->changeFlag();
                        gameArray[coin->posX-1][coin->posY]=gameArray[coin->posX-1][coin->posY]==0? 1:0;//储存当前状态
                    }
                    if(coin->posY+1<=3){//上侧可以翻的时候
                        this->coinBtn[coin->posX][coin->posY+1]->changeFlag();
                        gameArray[coin->posX][coin->posY+1]=gameArray[coin->posX][coin->posY+1]==0? 1:0;//储存当前状态
                    }
                    if(coin->posY-1>=0){//下侧可以翻的时候
                        this->coinBtn[coin->posX][coin->posY-1]->changeFlag();
                        gameArray[coin->posX][coin->posY-1]=gameArray[coin->posX][coin->posY-1]==0? 1:0;//储存当前状态
                    }

                    //胜利判定
                    isWin=true;
                    for(int i=0;i<4;i++){
                        for(int j=0;j<4;j++){
                            if( coinBtn[i][j]->flag==false){
                                this->isWin=false;
                                break;
                            }
                        }
                    }
                    if(this->isWin)
                    {   //胜利
                        // QTimer::singleShot(1000,this,[=](){
                        //     qDebug()<<"胜利了";
                        // });

                        WinSound->play();
                        for(int i=0;i<4;i++)
                        {
                            for(int j=0;j<4;j++){
                                coinBtn[i][j]->isWin=true;
                            }
                        }
                        //胜利图片弹下来
                        QPropertyAnimation *animation=new QPropertyAnimation(winLabel,"geometry");
                        animation->setDuration(1000);
                        animation->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                        //结束位置,y上10
                        animation->setEndValue(QRect(winLabel->x(),winLabel->y()+200,winLabel->width(),winLabel->height()));
                        //设置缓和曲线，弹跳效果
                        animation->setEasingCurve(QEasingCurve::OutBounce);
                        animation->start(QAbstractAnimation::DeleteWhenStopped);

                        //下一关功能
                        QMessageBox mesg(this);
                        mesg.setWindowTitle("恭喜你过关了");
                        mesg.setText("恭喜过关，请选择下一步的操作。");
                        mesg.setWindowFlag(Qt::FramelessWindowHint);//无边框
                        QPushButton *btnNext=new QPushButton("下一关");
                        QPushButton *btnCancel=new QPushButton("其他关卡");
                        mesg.addButton(btnNext, QMessageBox::AcceptRole);
                        mesg.addButton(btnCancel, QMessageBox::RejectRole);
                        mesg.setStyleSheet(styleSheet);

                        connect(btnNext, &QPushButton::clicked, [=]() {//发射关卡号
                            qDebug() << "下一关按钮被点击：";
                            emit levalNext(levalIndex);
                        });

                        connect(btnCancel, &QPushButton::clicked, [=]() {
                            qDebug() << "返回选关界面";
                            QTimer::singleShot(300,this,[=](){
                                emit this->ChooseSceneBack();
                            });
                        });
                        mesg.exec();
                    }else{
                        for(int i=0;i<4;i++){
                            for(int j=0;j<4;j++){
                                this->coinBtn[i][j]->isWin=false;
                            }
                        }
                    }

                });

            });

        }
    }
}

void PlayScene::saveGame()
{
    //存储游戏状态
    QJsonObject saveData;

    // 存储关卡
    saveData["levelIndex"] = this->levalIndex;

    // 存储游戏当前状态
    QJsonArray gameArrayJson;
    for (int i = 0; i < 4; ++i) {
        QJsonArray row;
        for (int j = 0; j < 4; ++j) {
            row.append(gameArray[i][j]);
        }
        gameArrayJson.append(row);
    }
    saveData["gameArray"] = gameArrayJson;

    //用QJsonDocument中转成QByteArray
    QJsonDocument doc(saveData);
    QByteArray saveDataBytes = doc.toJson();

    // 写入文件
    //QIODevice::Text
    QString fileName = QString("saves/savegame_level_%1.json").arg(this->levalIndex);
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(saveDataBytes);
        file.close();
        qDebug() << "游戏状态已保存到：" << fileName;

    } else {
        qDebug() << "无法打开文件以保存游戏状态：" << fileName;
    }
}

bool PlayScene::loadGame()
{
    // 根据关卡索引构建文件名
    QString fileName = QString("saves/savegame_level_%1.json").arg(this->levalIndex);

    // 打开文件
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // 读取文件内容
        QByteArray saveDataBytes = file.readAll();
        file.close();

        // 反序列化QByteArray为QJsonDocument
        QJsonDocument doc = QJsonDocument::fromJson(saveDataBytes);
        if (!doc.isNull()) {
            QJsonObject saveData = doc.object();

            // 读取关卡索引
            this->levalIndex = saveData["levelIndex"].toInt();

            // 读取游戏数组的状态
            QJsonArray gameArrayJson = saveData["gameArray"].toArray();
            for (int i = 0; i < 4; ++i) {
                QJsonArray row = gameArrayJson[i].toArray();
                for (int j = 0; j < 4; ++j) {
                    gameArray[i][j] = row[j].toInt();
                }
            }
            //debug,
            for(int i=0;i<4;i++){
                for(int j=0;j<4;j++){
                    qDebug()<<gameArray[i][j];
                }
                qDebug()<<"";
            }

            // 重新初始化游戏界面
            updateCoinStates();
            qDebug() << "游戏状态已从：" << fileName << "读取成功";
            return true;
        } else {
            qDebug() << "无法解析保存的游戏状态：" << fileName;
            return false;
        }
    } else {
        QMessageBox msgBox;
        msgBox.move(QPoint(this->x()+20,this->y()+250));
        msgBox.setText("无法打开存档：\n"+fileName);
        QPushButton *btnOpen=new QPushButton("打开存档目录");
        msgBox.addButton(btnOpen,QMessageBox::AcceptRole);
        msgBox.setStyleSheet(styleSheet);
        connect(btnOpen,&QPushButton::clicked,[=](){
            // 获取程序目录
            QString appPath = QCoreApplication::applicationDirPath();
            // 获取存档目录
            QString savesPath=appPath+"/saves";
            //资源管理器打开文件夹
            QDesktopServices::openUrl(QUrl::fromLocalFile(savesPath));
        });
        msgBox.exec();
        return false;
    }
}

void PlayScene::updateCoinStates()
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            // 获取当前的金币
            MyCoin *coin = coinBtn[i][j];

            // 根据存档中的状态更新金币的图像
            QString str;
            if (gameArray[i][j] == 1) {
                str = ":/res/Coin0001.png";
            } else {
                str = ":/res/Coin0008.png";
            }
            QPixmap pic;
            bool ret = pic.load(str);
            if (!ret) {
                qDebug() << "图片加载失败，失败的路径是：" << str;
                continue;
            }
            coin->setIcon(pic);
            coin->flag = gameArray[i][j]; // 状态初始化
        }
    }
}

void PlayScene::paintEvent(QPaintEvent *event)
{
    //背景
    QPainter painter(this);
    QPixmap pic;
    pic.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pic);
    //标题
    pic.load(":/res/Title.png");
    pic=pic.scaled(pic.width()*0.5,pic.height()*0.5);
    painter.drawPixmap(10,30,pic);
}
