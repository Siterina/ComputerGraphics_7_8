#include "nmatrix.h"

NMatrix::NMatrix() {
    for(int i = 0; i < 2; i++)
        for(int j = 0; j < 2; j++)
            data[i][j] = 0.0;
}

void NMatrix::Rotate(double alpha) {
    data[0][0] = cos(alpha);
    data[1][1] = cos(alpha);
    data[0][1] = (-1) * sin(alpha);
    data[1][0] = sin(alpha);
}

QPointF NMatrix::operator*(const QPointF v) {
    QPointF result;
    result.setX(v.x() * data[0][0] + v.y() * data[1][0]);
    result.setY(v.x() * data[0][1] + v.y() * data[1][1]);
    return result;
}
