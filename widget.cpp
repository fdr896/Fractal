#include "widget.h"
#include "ui_widget.h"
#include "complex.h"

#include <iostream>
#include <QMouseEvent>
#include <QImage>
#include <QString>

using namespace std;

Complex C1 = Complex(-0.74543, 0.11301);
Complex C2 = Complex(-0.8, 0.156);
Complex C3 = Complex(0.285, 0.01);
Complex C4 = Complex(-0.0085, 0.71);

int W, H;
int x_label1, y_label1;
int x_label2, y_label2;

int x_mouse1, y_mouse1;
int x_mouse2, y_mouse2;

const ld X_MIN = (ld)-1.5;
const ld Y_MIN = (ld)-1.5;
const ld X_MAX = (ld)1.5;
const ld Y_MAX = (ld)1.5;

ld x_min = X_MIN;
ld x_max = X_MAX;
ld y_min = Y_MIN;
ld y_max = Y_MAX;

ld dx;
ld dy;

int curr_fract = 0;

QImage img;

extern vector<tuple<int, int, int> > colors;

ld get_x(int x) {
    return x_min + dx * x;
}

ld get_y(int y) {
    return y_max - dy * y;
}

void rebuild(Ui::Widget * ui) {
    dx = (x_max - x_min) / W;
    dy = (y_max - y_min) / H;

    QString xMin = QString::number(x_min, 'f', 5);
    QString yMin = QString::number(y_min, 'f', 5);
    QString xMax = QString::number(x_max, 'f', 5);
    QString yMax = QString::number(y_max, 'f', 5);

    ui->x1Line->setText(xMin);
    ui->y1Line->setText(yMin);
    ui->x2Line->setText(xMax);
    ui->y2Line->setText(yMax);
}

void make_begin() {
    x_min = X_MIN;
    y_min = Y_MIN;
    x_max = X_MAX;
    y_max = Y_MAX;
}

void Widget::buttonChange() {
    curr_fract = 1 - curr_fract;

    make_begin();
    drawFractal();
}

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    ui->xPos->setMinimum(-INT_MAX);
    ui->xPos->setMaximum(INT_MAX);

    ui->yPos->setMinimum(-INT_MAX);
    ui->yPos->setMaximum(INT_MAX);

    H = ui->surface->height();
    W = ui->surface->width();

    x_label1 = ui->surface->y();
    x_label2 = x_label1 + H;

    y_label1 = ui->surface->x();
    y_label2 = y_label1 + W;

    img = QImage(H, W, QImage::Format_RGB888);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(buttonChange()));

    drawFractal();
}

Widget::~Widget() {
    delete ui;
}

void Widget::buttonClicked() {
    make_begin();

    drawFractal();
}

void Widget::mouseMoveEvent(QMouseEvent * event) {
    int curr_x = event->y() - x_label1;
    int curr_y = event->x() - y_label1;

    if ((curr_x < 0) || (curr_x > x_label2) || (curr_y < 0) || (curr_y > y_label2)) {
        ui->xPos->setValue(-1);
        ui->yPos->setValue(-1);
    }
    else {
        ui->xPos->setValue(curr_x);
        ui->yPos->setValue(curr_y);
    }

    QWidget::mouseMoveEvent(event);
}

void Widget::mousePressEvent(QMouseEvent * event) {
    x_mouse1 = event->x() - x_label1;
    y_mouse1 = event->y() - y_label1;

    QWidget::mousePressEvent(event);
}

void Widget::mouseReleaseEvent(QMouseEvent * event) {
    x_mouse2 = event->x() - x_label1;
    y_mouse2 = event->y() - y_label1;

    if (x_mouse2 - x_mouse1 > y_mouse2 - y_mouse1) {
        y_mouse2 = y_mouse1 + (x_mouse2 - x_mouse1);
    }
    else {
        x_mouse2 = x_mouse1 + (y_mouse2 - y_mouse1);
    }

    if (x_mouse2 - x_mouse1 < 5) {
        return;
    }

    ld n_x_min = get_x(x_mouse1);
    ld n_x_max = get_x(x_mouse2);
    ld n_y_max = get_y(y_mouse1);
    ld n_y_min = get_y(y_mouse2);

    tie(x_min, y_min, x_max, y_max) = {n_x_min, n_y_min, n_x_max, n_y_max};

    drawFractal();
    QWidget::mouseReleaseEvent(event);
}

void Widget::drawFractal() {
    rebuild(ui);

    ui->surface->setPixmap(QPixmap::fromImage(img));

    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            Complex Z = Complex(get_x(x), get_y(y));

            tuple<int, int , int> curr_color;

            if (!curr_fract) {
                curr_color = colors[Z.get_Mandelbrot()];
            }
            else {
                curr_color = colors[Z.get_Julia(C1)];
            }

            img.setPixel(x, y, qRgb(get<0>(curr_color), get<1>(curr_color), get<2>(curr_color)));
        }
    }

    ui->surface->setPixmap(QPixmap::fromImage(img));
}

 void Widget::drawRect() {
    for (int x = x_mouse1; x <= x_mouse2; ++x) {
        for (int y = y_mouse1; y <= y_mouse2; ++y) {
            img.setPixel(x, y, qRgb(255, 255, 0));
        }
    }

    ui->surface->setPixmap(QPixmap::fromImage(img));
    ui->surface->show();
}
