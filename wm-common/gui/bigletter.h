// headerfile für grossanzeigenelement,  grossanzeige wird mit einem festen
// zeichensatz dargestellt. es wird platz  für n blanks reserviert, und ein string
// rechtsbündig im anzeigenfeld platziert. beim instanziieren wird die anzahl
// der zeichen und die startgröße mit angegeben.

#ifndef BIGLETTER_H
#define BIGLETTER_H

#include <QFrame>
#include <QResizeEvent>

enum cAlign {rightAlign, leftAlign}; // rechtsbündig ist default
enum cFrame {frame, noframe}; // mit rahmen ist default
   
class CBigLetter:public QFrame
{
    Q_OBJECT
    Q_PROPERTY(QString m_sDisplay READ isDisplay WRITE display)
    Q_PROPERTY(int m_nStartSize READ getStartSize WRITE setStartSize)
    Q_PROPERTY(cAlign m_nAlign READ getAlignment WRITE setAlignment)
    Q_PROPERTY(cFrame m_nFrame READ getFrame WRITE setFrame)
    Q_ENUMS(cAlign)
    Q_ENUMS(cFrame)
public:
    CBigLetter(int, QString, QWidget* parent=0, const char* name=nullptr, Qt::WFlags f=0);
    CBigLetter(QWidget* parent=0, const char* name=nullptr, Qt::WFlags f=0);
    void display(QString);
    QString isDisplay() const;
    void setStartSize(int);
    int getStartSize() const;
    void setAlignment(cAlign);
    cAlign getAlignment() const;
    void setFrame(cFrame);
    cFrame getFrame() const;
    bool isFormatChanged();
protected:    
    virtual void resizeEvent(QResizeEvent *) override;
    void paintEvent(QPaintEvent *p) override;
    virtual QSize sizeHint() const override; //  gibt die optimale grösse zurück
    virtual QSize minimumSizeHint() const override; // gibt die minimale grösse zurück
private:
    void drawContents(QPainter *); // überschreibt drawcontents aus qframe
    void actFontSize(int);
    int m_nStartSize; // minimale grösse=startgrösse
    QString m_sDisplay;
    cAlign m_nAlign;
    cFrame m_nFrame;
    bool formatChanged;
};
  
#endif
