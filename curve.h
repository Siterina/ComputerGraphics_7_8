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
    int penWidth;
    int segments;
    QStack <QVector <QPointF> > curves;
    QVector <QPointF> temp;
    QVector <QPointF> toDraw;
    QVector <QPointF> m;
    int h, w;
    bool cycle;
    double alpha;
    int max_x, max_y, min_x, min_y;

    int tensionPoint;
    double tensionValueX;
    double tensionValueY;
    bool mChange;

private:
    Ui::Curve *ui;

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *e);
    QPointF splain(double x, QPointF Pk, QPointF Pk1, QPointF Mk, QPointF Mk1);
private slots:
    void on_exit_clicked();
    void on_colour_change_currentTextChanged(const QString &arg1);
    void on_segments_valueChanged(int arg1);
    void on_width_valueChanged(int arg1);
    void on_add_curve_clicked();
    void on_delete_last_clicked();
    void on_delete_all_clicked();
    void on_cycleBox_toggled(bool checked);
    void on_angleChange_valueChanged(int value);
    void on_pointToSetTension_valueChanged(int arg1);
    void on_setTensionX_valueChanged(double arg1);
    void on_setTensionY_valueChanged(double arg1);
    void on_changeTension_clicked();
};

#endif // CURVE_H
