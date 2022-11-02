#ifndef QTGE_LINE_H
#define QTGE_LINE_H

#include <iostream>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QGridLayout>
#include <core/painter.h>
#include <core/window.h>

class QTGELine : public QWidget
{
    private:
        int x0, y0;
        int x1, y1;

    private:
        QGridLayout* gridLayout;
        QPushButton* btFinish;
        
        QLabel* lbAlgorithms;
        QVBoxLayout* boxAlgoritms;
        QRadioButton* algorithm0; // brenseham
        QRadioButton* algorithm1; // DDA
        QRadioButton* algorithm2; // analitico

        QLabel* lpointA;
        QLabel* lpointB;
        QLabel* lb0;

        QLineEdit* tx0;
        QLineEdit* ty0;
        QLineEdit* tx1;
        QLineEdit* ty1;

        // painter
        Painter* painter;

    public:
        QTGELine(QWidget* parent = 0);
        ~QTGELine();
        QColor analitica(int x, int y);
        // void rasterizar();
        void setPainter(Painter* painter);
        
    public slots:
        void setPoints();


};



#endif