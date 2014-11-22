#include "curve.h"
#include "ui_curve.h"

Curve::Curve(QWidget *parent) :
    QWidget(parent),

    drawCurve(false),
    style("SolidLine"),
    colour("black"),
    width(1),segments(1),

    ui(new Ui::Curve)
{
    ui->setupUi(this);
}

Curve::~Curve()
{
    delete ui;
}

void changeStyle(QString style, QPen pen) {

    if(style == "SolidLine")
        pen.setStyle(Qt::SolidLine);
    if(style == "DashLine")
        pen.setStyle(Qt::DashLine);
    if(style == "DotLine")
        pen.setStyle(Qt::DotLine);
    if(style == "DashDotLine")
        pen.setStyle(Qt::DashDotLine);
}


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
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 5));


    for(int i = 0; i < temp.size(); i++) {
        painter.drawPoint(temp[i]);
    }


    QPen penCurve;
    changeStyle(style, penCurve);
    penCurve.setWidth(width);
    penCurve.setColor(colour);
    painter.setPen(penCurve);
    if(curves.size()) {
        for(int i = 0; i < curves.size(); i++) {
            for(int j = 0; j < curves[i].size(); j++) { // draw points
                painter.drawPoint(curves[i][j]);
            }
            for(int j = 0; j <= curves[i].size() - 1; j++) { // count differential vector
                if(j == 0)
                    m.push_back((curves[i][j+1] - curves[i][j]) / (curves[i][j+1].x() - curves[i][j].x()));
                else if(j == curves[i].size() - 1)
                    m.push_back((curves[i][j] - curves[i][j-1]) / (curves[i][j].x() - curves[i][j-1].x()));
                else
                    m.push_back((curves[i][j+1] - curves[i][j-1]) / (curves[i][j+1].x() - curves[i][j-1].x()));

            }
            for(int j = 0; j < curves[i].size() - 1; j++) { // draw curve
                double step = 0.1;
                QPointF prev, next;
                prev.setX(curves[i][j].x());
                prev.setY(curves[i][j].y());

                for(double x = curves[i][j].x(); x < curves[i][j+1].x() + step; x+=step) {
                    next = splain(x, curves[i][j], curves[i][j+1], m[j], m[j+1]);
                    painter.drawLine(prev, next);
                    prev = next;
                }
            }

            m.clear();
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

void Curve::on_style_change_currentTextChanged(const QString &arg1) {
    style = arg1;
    repaint();
}

void Curve::on_segments_valueChanged(int arg1) {
    segments = arg1;
}

void Curve::on_width_valueChanged(int arg1) {
    width = arg1;
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
    repaint();
}
