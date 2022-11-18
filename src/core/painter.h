#ifndef PAINTER_H
#define PAINTER_H

#include <vector>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QApplication>
#include <QtGui/QPainter>
#include <QtCore/QTimer>
#include <iostream>
#include <functional>
#include <helpers/pixels.h>

#include <chrono>
#include <unistd.h>

typedef struct {
    QColor above;
    QColor down;
    QColor left;
    QColor right;
} PixelAround;

typedef enum {
    NONE,
    LINE,
    CIRCLE,
    POLYGON
} ObjectType;

static const ObjectType objectTypes[] = {LINE, CIRCLE, POLYGON};

typedef std::function<QColor(PixelAround p, int x, int y)> PaintCallback;
typedef std::function<void(uchar* buffer, int width, int height)> PaintBufferCallback;

class Painter : public QWidget
{
    private:
        int bytes;
        QSize size;         // tamanho da janela de visualizacao
        QSize imageSize;    // tamanho da imagem (influencia na quantidade de pixels)
        std::map<ObjectType, std::vector<PaintCallback>> paintCallbacks;
        std::map<ObjectType, std::vector<PaintBufferCallback>> paintBufferCallbacks;
        uchar* pixels;
        
    public:
        explicit Painter(int width, int height, QWidget *parent = 0);
        void paintEvent(QPaintEvent* event) override;
        
        void addPaintCallback(ObjectType type, PaintCallback pcb);        
        void addPaintBufferCallback(ObjectType type, PaintBufferCallback pbc); 

        void floodFill(int x, int y, QColor color, QColor nColor);       
        PixelAround pixelAround(int x, int y);
        QColor pixelAt(int x, int y);
        void setPixel(int x, int y, QColor color);
        void clearBuffer();
        void clearPaintBufferCallbacks(ObjectType type = ObjectType::NONE);
        void clearPaintCallbacks(ObjectType type = ObjectType::NONE);        

        void saveImage();

        inline int imgWidth() const { return imageSize.width(); };
        inline int imgHeight() const { return imageSize.height(); };

        int sizeOfPaintCallbacks(ObjectType type);
        int sizeOfPaintBufferCallbacks(ObjectType type);

};


#endif