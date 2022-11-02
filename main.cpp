#include <iostream>
#include <core/window.h>
#include <core/painter.h>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <math/helpers.h>

int PW = 640;
int PH = 480;

QColor circuloImplicito(PixelAround p, int x, int y) {
    
    QColor backgroundColor = QTGEWindow::colors[0];
    QColor primaryColor = QTGEWindow::colors[1];

    // desenhando um circulo qualquer

    int cx = PW / 2;
    int cy = PH / 2;
    int r = (PW/10);

    float f1 = (x*x) - 2*x*cx + (cx*cx);
    float f2 = (y*y) - 2*y*cy + (cy*cy);

    if( (f1+f2) < (r*r) && (f1+f2) > (r*r)-128 ) {        
        return primaryColor;
    } else {
        return backgroundColor;
    }

}

void telaVermelha(uchar* pixels, int width, int height) {
    
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int pixel = (x + y * width) * 4;
            QColor color = QColor(255, 0, 0);
            pixels[pixel + 0] = color.red();
            pixels[pixel + 1] = color.green();
            pixels[pixel + 2] = color.blue();
            pixels[pixel + 3] = color.alpha();
        }
    }

}

void floodFill(uchar* pixels, int width, int height, int x, int y) {
        
    if(x > width || y > height || x < 0 || y < 0) return;

    // calculando posição do pixel
    int pixel = (x + y * width) * 4;
    if(pixels[pixel] == 0 && pixels[pixel+1] == 0 && pixels[pixel+2] == 0) {
        pixels[pixel]       = 255;  // r
        pixels[pixel + 1]   = 0;    // g
        pixels[pixel + 2]   = 0;    // b    
        floodFill(pixels, width, height, x + 1, y);
        floodFill(pixels, width, height, x - 1, y);
        floodFill(pixels, width, height, x, y - 1);
        floodFill(pixels, width, height, x, y + 1);
    }

}

int main(int argc, char** argv) {
    QApplication app (argc, argv);
    QTGEWindow window(860, 480);

    Painter* p = window.getPainter();
    // rasterizando o circulo
    p->addPaintCallback(circuloImplicito);
    // executando uma leitura dos pixels    
    p->addPaintBufferCallback([p](uchar* pixels, int w, int h) {        
        // famoso centro do circulo
        QPoint pixelSeed(PW / 2, PH / 2);
        p->floodFill(pixelSeed.x(), pixelSeed.y(), QColor(0, 0, 0), QColor(255, 0, 0));
    });

    window.show();
    return app.exec();
}