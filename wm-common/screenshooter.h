#ifndef SCREENSHOOTER_H
#define SCREENSHOOTER_H

#include "qmenu.h"
#include <QObject>

class screenshooter : public QObject
{
    Q_OBJECT
public:
    explicit screenshooter(QObject *parent = nullptr);

    void showUIbyExec(QMenu* uipoi);
    void storeScreenShot(QWidget *poi, QString fileName);
signals:
};

#endif // SCREENSHOOTER_H
