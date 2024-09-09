#ifndef COINCARTOON_H
#define COINCARTOON_H

#include <QPushButton>

class coinCartoon : public QPushButton
{
    Q_OBJECT
public:

    coinCartoon(QString btnImg);

    void setButton(QString picPath);//设置自定义按键

    void changeFlag();//翻转效果

    QTimer *timer;

    int min=1;

signals:
};

#endif // COINCARTOON_H
