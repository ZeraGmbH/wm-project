#include "screenshooter.h"

screenshooter::screenshooter(QObject *parent)
    : QObject{parent}
{}

void screenshooter::showUIbyExec(QMenu *uipoi)
{
    QPoint pos_UI, pos_Window, pos_Korr, pos_Aim;
    pos_UI = uipoi->pos();
    //    pos_Window = parent->pos();
    pos_Korr = {10,34};
    pos_Aim = pos_UI + pos_Korr;
    uipoi->exec(pos_Aim);

}

void screenshooter::storeScreenShot(QWidget* poi, QString fileName)
{
    QPixmap map;
    QString strFileName;
    WId intId = poi->internalWinId();
    map = map.grabWindow(intId);
    strFileName= "/home/wschunk/tmp/" + fileName +".png";
    bool ok = map.save(strFileName);
}
