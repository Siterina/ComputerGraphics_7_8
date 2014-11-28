#include "curve.h"
#include "ui_curve.h"
#include "nmatrix.h"
#include <QDebug>
#define pi 3.1415


Curve::Curve(QWidget *parent) :
    QWidget(parent),

    drawCurve(false),
    style("SolidLine"),
    colour("black"),
    penWidth(1),segments(1),
    h(height()), w(width()),
    cycle(false), alpha(0),
    max_x(0), max_y(0),
    min_x(10000), min_y(10000),

    ui(new Ui::Curve)
{
    ui->setupUi(this);
}

Curve::~Curve()
{
    delete ui;
}

//void changeStyle(QString style, QPen pen) {

//    if(style == "SolidLine")
//        pen.setStyle(Qt::SolidLine);
//    if(style == "DashLine")
//        pen.setStyle(Qt::DashLine);
//    if(style == "DotLine")
//        pen.setStyle(Qt::DotLine);
//    if(style == "DashDotLine")
//        pen.setStyle(Qt::DashDotLine);
//}


QPointF Curve::splain(double x, QPointF Pk, QPointF Pk1, QPointF Mk, QPointF Mk1) {
    QPointF result;
    double t = (x - Pk.x()) / (Pk1.x() - Pk.x());

    result = (2*t*t*t - 3*t*t + 1) * Pk +
            (t*t*t - 2*t*t + t) * (Pk1.x() - Pk.x()) * Mk +
            (-2*t*t*t + 3*t*t) * Pk1 +
            (t*t*t - t*t) * (Pk1.x() - Pk.x()) * Mk1;

    return result;
}



void Curve::paintEvent(QPaintEvent*) {

   // double Ky = h / height();
    //double Kx = w / width();
    //double K = std::min(Kx, Ky);

    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 5));


    for(int i = 0; i < temp.size(); i++) {
        painter.drawPoint( temp[i] );
    }

    NMatrix toRotate = NMatrix();
    toRotate.Rotate(alpha);


    QPen penCurve;
    //changeStyle(style, penCurve);
    penCurve.setWidth(penWidth);
    penCurve.setColor(colour);
    painter.setPen(penCurve);
    if(curves.size()) {
        for(int i = 0; i < curves.size(); i++) {
            if(cycle)
                curves[i].push_back(curves[i][0]);
           /* for(int j = 0; j < curves[i].size(); j++) { // draw points
                curves[i][j].setX(curves[i][j].x() - max_x / 2.0);
                curves[i][j].setY(curves[i][j].y() - max_y / 2.0);
                curves[i][j] = toRotate * curves[i][j];
                curves[i][j].setX(curves[i][j].x() + max_x / 2.0);
                curves[i][j].setY(curves[i][j].y() + max_y/ 2.0);
                // qDebug() << toRotate.data[0][0];
            }*/


            for(int j = 0; j <= curves[i].size() - 1; j++) { // count differential vector
                if(j == 0)
                    m.push_back((curves[i][j+1] - curves[i][j]) / (curves[i][j+1].x() - curves[i][j].x()));
                else if(j == curves[i].size() - 1)
                    m.push_back((curves[i][j] - curves[i][j-1]) / (curves[i][j].x() - curves[i][j-1].x()));
                else
                    m.push_back((curves[i][j+1] - curves[i][j-1]) / (curves[i][j+1].x() - curves[i][j-1].x()));

            }
            for(int j = 0; j < curves[i].size() - 1; j++) { // draw curve
                double step = 0.05;
                QPointF prev, next;
                prev.setX(curves[i][j].x());
                prev.setY(curves[i][j].y());
               // toDraw.push_back(curves[i][j]);
                if(curves[i][j].x() < curves[i][j+1].x())
                    for(double x = curves[i][j].x(); x < curves[i][j+1].x() + step; x+=step) {
                        toDraw.push_back(splain(x, curves[i][j], curves[i][j+1], m[j], m[j+1]));
                        //next = splain(x, curves[i][j], curves[i][j+1], m[j], m[j+1]);
                        //painter.drawLine(prev, next);
                        //prev = next;
                    }
                else {
                    for(double x = curves[i][j].x(); x > curves[i][j+1].x() - step; x-=step) {
                        toDraw.push_back(splain(x, curves[i][j], curves[i][j+1], m[j], m[j+1]));
                        //next = splain(x, curves[i][j], curves[i][j+1], m[j], m[j+1]);
                        //painter.drawLine(prev, next);
                        //prev = next;
                    }  
                }

                for(int i = 0; i < toDraw.size(); i++) {
                    toDraw[i].setX(toDraw[i].x() - (max_x + min_x)/ 2.0);
                    toDraw[i].setY(toDraw[i].y() - (max_y + min_y)/ 2.0);
                    toDraw[i] = toRotate * toDraw[i];
                    toDraw[i].setX(toDraw[i].x() + (max_x + min_x)/ 2.0);
                    toDraw[i].setY(toDraw[i].y() + (max_y + min_y)/ 2.0);
                }

                for(int i = 0; i < toDraw.size() - 1; i++) {
                    painter.drawLine(toDraw[i], toDraw[i+1]);
                }

            }
            toDraw.clear();
            m.clear();
            if(cycle)
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

//void Curve::on_style_change_currentTextChanged(const QString &arg1) {
//   style = arg1;
//   repaint();
//}

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
