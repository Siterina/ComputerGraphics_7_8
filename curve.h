#ifndef CURVE_H
#define CURVE_H

#include <QWidget>
#include <QStack>
#include <QVector>
#include <QPoint>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QStyle>

namespace Ui {
class Curve;
}

class Curve : public QWidget
{
    Q_OBJECT

public:
    explicit Curve(QWidget *parent = 0);
    ~Curve();

    QString colour;
    QString style;
    int penWidth;
    int segments;
    QStack <QVector <QPointF> > curves;
    QVector <QPointF> temp;
<<<<<<< HEAD
    QVector <QPointF> toDraw;
=======
>>>>>>> origin/master
    QVector <QPointF> m;
    bool drawCurve;
    int h, w;
    bool cycle;
    double alpha;
<<<<<<< HEAD
    int max_x, max_y, min_x, min_y;
=======
>>>>>>> origin/master

private:
    Ui::Curve *ui;

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *e);
    QPointF splain(double x, QPointF Pk, QPointF Pk1, QPointF Mk, QPointF Mk1);
private slots:
    void on_exit_clicked();
    void on_colour_change_currentTextChanged(const QString &arg1);
    //void on_style_change_currentTextChanged(const QString &arg1);
    void on_segments_valueChanged(int arg1);
    void on_width_valueChanged(int arg1);
    void on_add_curve_clicked();
    void on_delete_last_clicked();
    void on_delete_all_clicked();
    void on_cycleBox_toggled(bool checked);
<<<<<<< HEAD
    void on_angleChange_valueChanged(int value);
=======
>>>>>>> origin/master
};

#endif // CURVE_H
