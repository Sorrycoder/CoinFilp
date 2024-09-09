#ifndef DATACONFIG_H
#define DATACONFIG_H

#include <QObject>
#include <QMap>
#include <QVector>

class dataConfig : public QObject
{
    Q_OBJECT
public:
    explicit dataConfig(QObject *parent = 0);

public:

    QMap<int, QVector< QVector<int> > > mData;  //起到键值对的作用，参数1为key,代表关卡编号；参数2为value,其中value又是一个嵌套的容器，为一个二维数组



signals:

public slots:
};

#endif // DATACONFIG_H
