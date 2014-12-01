#include "curve.h"
#include "ui_curve.h"
#include "nmatrix.h"
#include <QDebug>
#define pi 3.1415


Curve::Curve(QWidget *parent) :
    QWidget(parent),

    colour("black"),
    penWidth(1),segments(1),
    h(height()), w(width()),
    cycle(false), alpha(0),
    max_x(0), max_y(0),
    min_x(10000), min_y(10000),
    tensionPoint(0), tensionValueX(1), tensionValueY(1),
    mChange(false),

    ui(new Ui::Curve)
{
    ui->setupUi(this);
}

Curve::~Curve()
{
    delete ui;
}

QPointF mabs(QPointF t) {
    if(t.x() < 0)
        t.setX(t.x() * (-1));
    if(t.y() < 0)
        t.setY(t.y() * (-1));
    return t;
}


QPointF Curve::splain(double x, QPointF Pk, QPointF Pk1, QPointF Mk, QPointF Mk1) {
    QPointF result;
    double t = (x - Pk.x()) / (Pk1.x() - Pk.x());

    result = (2*t*t*t - 3*t*t + 1) * Pk +
            (t*t*t - 2*t*t + t) * (Pk1.x() - Pk.x()) * Mk +
            (-2*t*t*t + 3*t*t) * Pk1 +
            (t*t*t - t*t) * (Pk1.x() - Pk.x()) * Mk1;

    return mabs(result);
}


void Curve::paintEvent(QPaintEvent*) {

    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 5));


    for(int i = 0; i < temp.size(); i++) {
        painter.drawPoint( temp[i] );
    }

    NMatrix toRotate = NMatrix();
    toRotate.Rotate(alpha);


    QPen penCurve;
    penCurve.setWidth(penWidth);
    penCurve.setColor(colour);
    painter.setPen(penCurve);
    if(curves.size()) {
        for(int i = 0; i < curves.size(); i++) {
            if(cycle && curves[i].size() > 2)
                curves[i].push_back(curves[i][0]);


            for(int j = 0; j <= curves[i].size() - 1; j++) { // count differential vector
                if(j == 0)
                    m.push_back((curves[i][j+1] - curves[i][j]) / (curves[i][j+1].x() - curves[i][j].x()));
                else if(j == curves[i].size() - 1)
                    m.push_back((curves[i][j] - curves[i][j-1]) / (curves[i][j].x() - curves[i][j-1].x()));
                else
                    m.push_back((curves[i][j+1] - curves[i][j-1]) / (curves[i][j+1].x() - curves[i][j-1].x()));

            }

            qDebug() << mChange << " " << m.size() << " " << tensionPoint << endl;

            if(mChange) {
                m[tensionPoint].setX(tensionValueX);
                m[tensionPoint].setY(tensionValueY);
            }

            for(int j = 0; j < curves[i].size() - 1; j++) { // form vector to draw curve
                double step = 0.05;
                if(curves[i][j].x() < curves[i][j+1].x())
                    for(double x = curves[i][j].x(); x < curves[i][j+1].x() + step; x+=step) {
                        toDraw.push_back(splain(x, curves[i][j], curves[i][j+1], m[j], m[j+1]));
                    }
                else
                    for(double x = curves[i][j].x(); x > curves[i][j+1].x() - step; x-=step) {
                        toDraw.push_back(splain(x, curves[i][j], curves[i][j+1], m[j], m[j+1]));
                    }  

            }

             //qDebug() << toDraw.size() << endl;
             qDebug() << m[0] << endl;
             qDebug() << m[1] << endl;


            for(int k = 0; k < toDraw.size(); k++) { // rotate curve
                toDraw[k].setX(toDraw[k].x() - (max_x + min_x)/ 2.0);
                toDraw[k].setY(toDraw[k].y() - (max_y + min_y)/ 2.0);
                toDraw[k] = toRotate * toDraw[k];
                toDraw[k].setX(toDraw[k].x() + (max_x + min_x)/ 2.0);
                toDraw[k].setY(toDraw[k].y() + (max_y + min_y)/ 2.0);
            }

            for(int k = 0; k < toDraw.size() - 1; k++) { // draw curve
                painter.drawLine(toDraw[k], toDraw[k+1]);
            }


            toDraw.clear();
            m.clear();
            if(cycle && curves[i].size() > 2)
                curves[i].pop_back();
        }
    }


} // end of paintEvent


/***********************************************/
/*                   Form                      */
/***********************************************/

void Curve::mousePressEvent(QMouseEvent *e) {
    if(e->button() == Qt::LeftButton) {
        QPoint click (e->x(), e->y());
        temp.push_back(click);
        if(e->x() > max_x)
            max_x = e->x();
        if(e->y() > max_y)
            max_y = e->y();
        if(e->x() < min_x)
            min_x = e->x();
        if(e->y() < min_y)
            min_y = e->y();
    }
    else {
        if(temp.size() != 0)
            temp.pop_back();
    }
    repaint();
}


void Curve::on_exit_clicked() {
    close();
}

void Curve::on_colour_change_currentTextChanged(const QString &arg1) {
    colour = arg1;
    repaint();
}

void Curve::on_segments_valueChanged(int arg1) {
    segments = arg1;
}

void Curve::on_width_valueChanged(int arg1) {
    penWidth = arg1;
    repaint();
}

void Curve::on_add_curve_clicked() {
    if(temp.size() == segments + 1) {
        curves.push(temp);
    }
        temp.clear();
        repaint();
}

void Curve::on_delete_last_clicked() {
    if(curves.size())
        curves.pop();
    repaint();
}

void Curve::on_delete_all_clicked() {
    curves.clear();
    max_x = 0;
    max_y = 0;
    min_x = 10000;
    min_y = 10000;
    repaint();
}

void Curve::on_cycleBox_toggled(bool checked) {
    if (checked)
        cycle = true;
    else cycle = false;
    repaint();
}

void Curve::on_angleChange_valueChanged(int value) {
    alpha = (double)value * pi / 180;
    repaint();
}

void Curve::on_pointToSetTension_valueChanged(int arg1) {
    if(curves[0].size() > arg1 - 1) {
        tensionPoint = arg1 - 1;
        mChange = true;
        repaint();
    }
    else mChange = false;
}

void Curve::on_setTensionX_valueChanged(double arg1) {
    tensionValueX = arg1;
}

void Curve::on_setTensionY_valueChanged(double arg1) {
    tensionValueY = arg1;
}

void Curve::on_changeTension_clicked() {
        if(curves[0].size() > tensionPoint) {
            mChange = true;
            repaint();
        }
        else mChange = false;
}
