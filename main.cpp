#include <iostream>
#include <core/window.h>
#include <core/painter.h>
#include <helpers/pixels.h>

int main(int argc, char** argv) {
    QApplication app (argc, argv);
    QTGEWindow window(980, 480);

    Painter* p = window.getPainter();
    
    window.show();
    return app.exec();
}