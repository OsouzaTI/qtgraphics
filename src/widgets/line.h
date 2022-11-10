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
#include <helpers/pixels.h>

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

        void infoPoints();

        // algoritmo selecionado
        int algorithm = 0; // padrao: Brenseham
        void setAlgoritm(int algorithm);
    public:
        QTGELine(QWidget* parent = 0);
        ~QTGELine();
        void analitica(uchar* pixels, int width, int height, int x0, int y0, int x1, int y1, QColor color);

        /**
         * Função que ira executar o algoritmo DDA para rasterização
         * de linhas
         * */
        void digitalDifferentialAnalyzer(uchar* pixels, int width,  int x0, int y0, int x1, int y1, QColor color);
        // void rasterizar();
        void setPainter(Painter* painter);

    public slots:
        void setPoints();
        // callbacks dos radiobuttons
        inline void setBrenseham()  { setAlgoritm(0); };
        inline void setDDA()        { setAlgoritm(1); };
        inline void setAnalytic()   { setAlgoritm(2); };


};



#endif