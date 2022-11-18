#ifndef QTGE_WINDOW_H
#define QTGE_WINDOW_H

#include <iostream>
#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QBoxLayout>
#include <QLineEdit>
#include <widgets/select_color_button.h>
#include <widgets/line.h>
#include <widgets/polygon.h>
#include <widgets/circle.h>
#include "painter.h"
#include "rgb.h"

class QTGELine;
class QTGEPolygon;
class QTGECircle;

class QTGEWindow : public QWidget {

private:
    Painter* painter;

    // layout dividido em duas partes lado a lado
    QHBoxLayout* layout;
    QHBoxLayout* paintLayout;

    // algoritmos de preenchimento
    QHBoxLayout* hbPreenchimento;
    
    // lado 1
    QVBoxLayout* vbox1;
    // lado 2
    QVBoxLayout* vbox2;

    // botoes
    QPushButton* color;
    SelectColorButton* selectBackgroundColor;
    SelectColorButton* selectPrimaryColor;

    QLineEdit* pixelSeedX;
    QLineEdit* pixelSeedY;
    QPushButton* btFloodFill;
    QPushButton* btScanLine;

    QSize size;
    
    // widgets
    QTGELine* line;
    QTGEPolygon* polygon;
    QTGECircle* circle;


public:
    // primary, secondary ...
    static Rgb colors[2];
    explicit QTGEWindow(int width, int height, QWidget *parent = 0);
    void resizeEvent(QResizeEvent* event);
    void keyPressEvent(QKeyEvent *event) override;
    inline Painter* getPainter(){ return painter; };
    void setBackgroundColor(const QColor& color);
    void setPrimaryColor(const QColor& color);

    // update painter
    inline void updatePainter(){ this->painter->update(); }

public slots:
    void floodFill();

};


#endif