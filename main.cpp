#include "widget.h"

#include <iostream>
#include <QApplication>

using std::vector;
using std::tuple;

const int MAX = 256;

vector<tuple<int, int, int> > colors;

void make_colors();

int main(int argc, char *argv[]) {
    make_colors();

    QApplication a(argc, argv);
    a.setStyle("fusion");

    Widget w;
    w.show();

    return a.exec();
}

void make_colors() {
    for (int i = 0; i < MAX; ++i) {
        colors.emplace_back(256 - i, 256 - i, 256 - i);
    }
}
