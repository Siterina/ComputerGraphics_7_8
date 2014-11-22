#include "curve.h"
#include "ui_curve.h"

Curve::Curve(QWidget *parent) :
    QWidget(parent),
    colour("black"),
    drawCurve(false),

    ui(new Ui::Curve)
{
    ui->setupUi(this);
}

Curve::~Curve()
{
    delete ui;
}


void Curve::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 5));


    for(int i = 0; i < temp.size(); i++) {
        painter.drawPoint(temp[i]);
    }

    if(drawCurve) {

    }


}




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
}
