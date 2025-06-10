#include "vectordiagramm.h"

    ComplexVectorWidget::ComplexVectorWidget(QWidget *parent) : QWidget(parent) {
        // Beispiel komplexe Zahlen
        c1 = std::complex<double>(1.0, 0.0);
        c2 = std::complex<double>(1.0, 0.0);
        c3 = std::complex<double>(1.0, 0.0);
    }
    void ComplexVectorWidget::setValue (int nr, double re, double im)
    {
        if (nr == 1){
            c1.real(re);
            c1.imag(im);
        }
        if (nr == 2){
            c2.real(re);
            c2.imag(im);
        }
        if (nr == 3){
            c3.real(re);
            c3.imag(im);
        }
    }

    double ComplexVectorWidget::Scale()
    {
        double scale = 50.0;
        double z1 = std::abs(c1);
        double z2 = std::abs(c2);
        double z3 = std::abs(c3);
        double max = z1;
        if (z2 > max) max = z2;

        int w1 = width()/2;
        int h1 = height()/2;
        int size = w1;
        if (h1 < size) size = h1;

        scale = (size - 10) / max;

        c1i.real(int (c1.real()*scale) + center.real());
        c1i.imag(int (c1.imag()*scale) + center.imag());

        return scale;
    }

    void ComplexVectorWidget::paintEvent(QPaintEvent *) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Mittelpunkt des Koordinatensystems
        center.real(width() / 2);
        center.imag(height() / 2);

        // Skalenfaktor für die Darstellung
        double scale = Scale();

        // Funktion zum Zeichnen eines Vektors
        auto drawVector = [&](const std::complex<double>& c, QColor color, const std::complex<int> center) {
            int x = center.real() + c.real() * scale;
            int y = center.imag() - c.imag() * scale; // y-Achse nach oben
            painter.setPen(QPen(color, 2));
            painter.drawLine(center.real(), center.imag(), x, y);
            // Pfeilspitze hinzufügen (optional)
        };

        // Zeichne die drei komplexen Vektoren
        drawVector(c1, Qt::red, center);
        drawVector(c2, Qt::green, center);
        drawVector(c3, Qt::blue, c1i);
    }

