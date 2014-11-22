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
    int width;
    int segments;
    QStack <QVector <QPoint> > curves;
    QVector <QPoint> temp;
    bool drawCurve;

private:
    Ui::Curve *ui;

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *e);
private slots:
    void on_exit_clicked();
    void on_colour_change_currentTextChanged(const QString &arg1);
    void on_style_change_currentTextChanged(const QString &arg1);
    void on_segments_valueChanged(int arg1);
    void on_width_valueChanged(int arg1);
    void on_add_curve_clicked();
    void on_delete_last_clicked();
    void on_delete_all_clicked();
};

#endif // CURVE_H
