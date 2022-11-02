#ifndef QTGE_WINDOW_H
#define QTGE_WINDOW_H

#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <widgets/select_color_button.h>
#include <widgets/line.h>
#include <iostream>
#include "painter.h"
#include "rgb.h"

class QTGELine;

class QTGEWindow : public QWidget {

private:
    Painter* painter;

    // layout dividido em duas partes lado a lado
    QHBoxLayout* layout;
    
    // lado 1
    QVBoxLayout* vbox1;
    // lado 2
    QVBoxLayout* vbox2;

    // botoes
    QPushButton* color;
    SelectColorButton* selectBackgroundColor;
    SelectColorButton* selectPrimaryColor;

    QSize size;
    
    // widgets
    QTGELine* line;


public:
    // primary, secondary ...
    static Rgb colors[2];
    explicit QTGEWindow(int width, int height, QWidget *parent = 0);
    void resizeEvent(QResizeEvent* event);
    inline Painter* getPainter(){ return painter; };
    void setBackgroundColor(const QColor& color);
    void setPrimaryColor(const QColor& color);

    // update painter
    inline void updatePainter(){ this->painter->update(); }
};


#endif