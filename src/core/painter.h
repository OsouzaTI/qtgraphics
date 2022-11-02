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

typedef struct {
    QColor above;
    QColor down;
    QColor left;
    QColor right;
} PixelAround;

typedef std::function<QColor(PixelAround p, int x, int y)> PaintCallback;
typedef std::function<void(uchar* buffer, int width, int height)> PaintBufferCallback;

class Painter : public QWidget
{
    private:
        int bytes;
        QSize size;
        std::vector<PaintCallback> paintCallbacks;
        std::vector<PaintBufferCallback> paintBufferCallbacks;
        uchar* pixels;
    public:
        explicit Painter(int width, int height, QWidget *parent = 0);
        void paintEvent(QPaintEvent*);
        void addPaintCallback(PaintCallback pcb);
        void addPaintBufferCallback(PaintBufferCallback pbc); 
        void floodFill(int x, int y, QColor color, QColor nColor);       
        PixelAround pixelAround(int x, int y);
        QColor pixelAt(int x, int y);
        void setPixel(int x, int y, QColor color);
};


#endif