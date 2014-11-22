#ifndef CURVE_H
#define CURVE_H

#include <QWidget>
#include <QStack>
#include <QVector>
#include <QPoint>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

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
};

#endif // CURVE_H
