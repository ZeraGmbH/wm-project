#ifndef VECTORDIAGRAMM_H
#define VECTORDIAGRAMM_H

#include <QWidget>
#include <QPainter>
#include <complex>

class ComplexVectorWidget : public QWidget
{
public:
    ComplexVectorWidget(QWidget *parent = nullptr);
    void setValue (int nr, double re, double im);

protected:
    double Scale();
    void paintEvent(QPaintEvent *) override;
private:
    std::complex<double> c1, c2, c3;
    std::complex<int> center, c1i;
};

#endif // VECTORDIAGRAMM_H
