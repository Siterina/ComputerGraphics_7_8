#ifndef NMATRIX_H
#define NMATRIX_H
#include <cmath>
#include <QPoint>

class NMatrix {

public:

    double data[2][2];

    NMatrix();
    void Rotate(double alpha);
    QPointF operator*(const QPointF v);
};

#endif // NMATRIX_H
