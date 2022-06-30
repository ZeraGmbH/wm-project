#include "bigletter.h"
#include <QPainter>
#include <QFontMetrics>
#include <QResizeEvent>

CBigLetter::CBigLetter(int s, QString d, QWidget *parent, const char* name, Qt::WFlags f)
    : QFrame(parent, name, f)
{
    m_nStartSize = s;
    actFontSize(m_nStartSize);
    m_sDisplay = d;
    setFrame(frame);
    formatChanged = false;
}

CBigLetter::CBigLetter(QWidget *parent, const char* name, Qt::WFlags f)
    : QFrame(parent, name, f)
{
    setFrame(frame);
}

void CBigLetter::drawContents(QPainter* p)
{
    int lFlags = Qt::AlignVCenter;
    if (m_nAlign == leftAlign)
        lFlags |= Qt::AlignLeft;
    else
        lFlags |= Qt::AlignRight;
    p->drawText(rect(), lFlags, m_sDisplay);
}

void CBigLetter::display(QString s)
{
    formatChanged = (m_sDisplay.length() != s.length());
    m_sDisplay = s;
    this->repaint();
}

QString CBigLetter::isDisplay() const
{
    return(m_sDisplay);
}

void CBigLetter::setStartSize(int s)
{
    m_nStartSize = s;
    actFontSize(m_nStartSize);
}    

int CBigLetter::getStartSize() const
{
    return(m_nStartSize);
}

void CBigLetter::setAlignment(cAlign a)
{
    m_nAlign = a;
}

cAlign CBigLetter::getAlignment() const
{
    return(m_nAlign);
}

void CBigLetter::setFrame(cFrame f)
{
    m_nFrame=f;
    if (m_nFrame==frame)
        setFrameStyle(QFrame::Box | QFrame::Plain);
    else
        setFrameStyle(QFrame::Plain);
}

cFrame CBigLetter::getFrame() const
{
    return(m_nFrame);
}

bool CBigLetter::isFormatChanged()
{
    return formatChanged;
}

void CBigLetter::resizeEvent (QResizeEvent* r)
{
    QSize ls = size();
    actFontSize((int) ((1.0*ls.height())/1.6));
    this->QFrame::resizeEvent(r);
}

void CBigLetter::paintEvent(QPaintEvent *event)
{
    // Stolen from Q3Frame::paintEvent
    QPainter paint(this);
    if (!contentsRect().contains(event->rect())) {
        paint.save();
        paint.setClipRegion(event->region().intersected(frameRect()));
        drawFrame(&paint);
        paint.restore();
    }
    if (event->rect().intersects(contentsRect())) {
        paint.setClipRegion(event->region().intersected(contentsRect()));
        drawContents(&paint);
    }
}

QSize CBigLetter::sizeHint() const 
{
    QFont fo(font());
    fo.setPointSize(m_nStartSize); // empfohlene grösse ist immer die startgrösse
    QFontMetrics tfm(fo);
    return(tfm.size(Qt::TextSingleLine, m_sDisplay));
}

QSize CBigLetter::minimumSizeHint() const
{
    QFont fo(font());
    fo.setPointSize(m_nStartSize);
    QFontMetrics tfm(fo);
    return(tfm.size(Qt::TextSingleLine, m_sDisplay));
}

void CBigLetter::actFontSize(int fs)
{
    QFont fo(font());
    fo.setPointSize(fs);
    setFont(fo);
}
