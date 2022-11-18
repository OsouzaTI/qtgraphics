#ifndef CIRCLE_H
#define CIRCLE_H

#include <iostream>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include "object.h"

#include <core/window.h>

class QTGEWindow;

struct Circle
{
    int cx, cy, r;
    Circle(int cx, int cy, int r): cx(cx), cy(cy), r(r) {};
};

class QTGECircle : public QTGEObject {

private:
    std::vector<Circle> circles;
    QGridLayout* layout;

    QLabel* label;

    QLineEdit* tx0;
    QLineEdit* ty0;
    QLineEdit* tr; // raio da circunferência

    QHBoxLayout* algorithmLayout;
    QRadioButton* algorithm0; // parametrico
    QRadioButton* algorithm1; // bresenham

    QPushButton* btClear;
    QPushButton* btFinish;

    // algoritmo selecionado
    int algorithm = 0; // padrao: Parametrica

    // auxiliar do preenchimento por linha de varredura
    std::pair<int, int> auxiliarPreenchimentoVarredura(int cx, int cy,  int r, int y);

public:
    QTGECircle(QWidget* parent = 0);
    void setAlgoritm(int algorithm);
    void parametrica(uchar* pixels, int width, int xc, int yc, int r, QColor color);
    void circunferenciaBresenham(uchar* pixels, int width, int xc, int yc, int r, QColor color);

    
    void preenchimentoLinhaVarredura(uchar* pixels, int width, int cx, int cy, int r, QColor color);

public slots:
    void addCircle();
    void clearCircles();
    // callbacks dos radiobuttons
    inline void setBresenham()  { setAlgoritm(0); };
    inline void setParametrica()  { setAlgoritm(1); };    
};


#endif