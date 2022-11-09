#include "painter.h"

Painter::Painter(int width, int height, QWidget *parent) 
    : QWidget(parent), size(QSize(width, height)) 
{   
    // setando tamanho fixo
    setFixedSize(size);
    // calculo de bytes
    bytes = width * height * sizeof(uint32_t);    
    // inicializando buffer
    pixels = new uchar[bytes];
}

void Painter::paintEvent(QPaintEvent* event) {
    
    if(!paintCallbacks.empty()) { 

        for (int x = 0; x < width(); x++) {
            for (int y = 0; y < height(); y++) {
                int pixel = (x + y * width()) * 4;
                for(PaintCallback paintCallback : paintCallbacks) {
                    QColor color = paintCallback(pixelAround(x, y), x, y);
                    pixels[pixel + 0] = color.red();
                    pixels[pixel + 1] = color.green();
                    pixels[pixel + 2] = color.blue();
                    pixels[pixel + 3] = color.alpha();
                }

            }
        }

    } 
    
    if (!paintBufferCallbacks.empty()) {
        for(PaintBufferCallback paintBufferCallback : paintBufferCallbacks) {
            paintBufferCallback(pixels, width(), height());
        }
    }

    // cria a imagem
    QImage image(pixels, width(), height(), QImage::Format_RGBX8888);    
    // adiciona a imagem no QPainter
    QPainter painter(this);    
    painter.drawImage(0, 0, image);
    clearBuffer();
}

PixelAround Painter::pixelAround(int x, int y) {    
    return {
        pixelAt(x, y-1),
        pixelAt(x, y+1),
        pixelAt(x-1, y),
        pixelAt(x+1, y)
    };
}

QColor Painter::pixelAt(int x, int y) {
    if(x >= 0 && x <= width() && y >= 0 && y <= height()) {
        int pixel = (x + y * width()) * 4;
        return QColor(pixels[pixel+0], pixels[pixel+1], pixels[pixel+2], pixels[pixel+3]);
    }
    return QColor(0, 0, 0);
}

void Painter::setPixel(int x, int y, QColor color) {
    if(x >= 0 && x <= width() && y >= 0 && y <= height()) {
        int pixel = (x + y * width()) * 4;
        pixels[pixel+0] = color.red();
        pixels[pixel+1] = color.green();
        pixels[pixel+2] = color.blue();
        pixels[pixel+3] = color.alpha();
    }
}

void Painter::clearBuffer() {
    std::fill_n(pixels, bytes, 0);
}

void Painter::clearPaintCallbacks() {
    paintCallbacks.clear();
}

void Painter::clearPaintBufferCallbacks() {
    paintBufferCallbacks.clear();
}

void Painter::addPaintCallback(PaintCallback pcb) {
    paintCallbacks.push_back(pcb);
}

void Painter::addPaintBufferCallback(PaintBufferCallback pbc) {
    paintBufferCallbacks.push_back(pbc);
}

void Painter::floodFill(int x, int y, QColor color, QColor nColor) {
    // usado para verificar se a figura esta "furada", causando stack overflow de recursoes
    int offset = 200;
    if(x < offset || x >= width() - offset || y < offset || y >= height() - offset) return;

    // calculando posição do pixel
    QColor pixel = pixelAt(x, y);
    if(pixel == color) {
        setPixel(x, y, nColor);
        floodFill(x + 1, y, color, nColor);
        floodFill(x - 1, y, color, nColor);
        floodFill(x, y - 1, color, nColor);
        floodFill(x, y + 1, color, nColor);
    }

}