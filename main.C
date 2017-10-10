#include "widget.h"
#include <QApplication>
#include <assert.h>
#include <vector>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyWidget w;
    w.show();

    return app.exec();
}
