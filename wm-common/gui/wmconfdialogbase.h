#ifndef WMCONFDIALOGBASE_H
#define WMCONFDIALOGBASE_H

#include "qdialog.h"
#include "screenshooter.h"
#include <QObject>

class wmconfdialogbase : public QDialog
{
    Q_OBJECT
public:
    wmconfdialogbase(QWidget* parent);
    ~wmconfdialogbase();
    void setScreenShooter(screenshooter *poi);

protected:
    screenshooter* mScrShooter;

private:

};

#endif // WMCONFDIALOGBASE_H
