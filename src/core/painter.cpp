#include "painter.h"

Painter::Painter(int width, int height, QWidget *parent) 
    : scale(1), QWidget(parent), size(QSize(width, height)), imageSize(QSize(160, 120))
{   
    // setando tamanho fixo
    setFixedSize(size);
    // calculo de bytes
    bytes = imgWidth() * imgHeight() * sizeof(uint32_t);    
    // inicializando buffer
    pixels = new uchar[bytes];
}

void Painter::paintEvent(QPaintEvent* event) {
    
    if(!paintCallbacks.empty()) { 

        for (int x = 0; x < imgWidth(); x++) {
            for (int y = 0; y < imgHeight(); y++) {                
                for(PaintCallback paintCallback : paintCallbacks) {
                    QColor color = paintCallback(pixelAround(x, y), x, y);
                    setPixel(x, y, color);
                }
            }
        }

    } 
    
    if (!paintBufferCallbacks.empty()) {
        for(PaintBufferCallback paintBufferCallback : paintBufferCallbacks) {
            paintBufferCallback(pixels, imgWidth(), imgHeight());
        }
    }

    // cria a imagem
    QImage image(pixels, imgWidth(), imgHeight(), QImage::Format_RGBX8888);
    QImage scaledImage = image.scaled(size);
    // adiciona a imagem no QPainter
    QPainter painter(this);    
    painter.drawImage(0, 0, scaledImage);
    // necessita ser desativado para usar algoritmos de preenchimento (ou limpar buffer via tela)
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
    if(x >= 0 && x <= imgWidth() && y >= 0 && y <= imgHeight()) {
        int pixel = (x + y * imgWidth()) * 4;
        return QColor(pixels[pixel+0], pixels[pixel+1], pixels[pixel+2]);
    }
    return QColor(0, 0, 0);
}

void Painter::setPixel(int x, int y, QColor color) {
    if(x >= 0 && x <= imgWidth() && y >= 0 && y <= imgHeight()) {
        int pixel = (x + y * imgWidth()) * 4;
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
    if(x < 0 || x >= imgWidth() || y < 0 || y >= imgHeight()) return;

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

void Painter::setScale(float scale) {
    int _scale = scale > 0 ? scale : 1;
    this->scale = _scale;
    update();
}