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
#include <helpers/pixels.h>

struct Line
{
    int x0, y0, x1, y1;
    Line(int x0, int y0, int x1, int y1): x0(x0), y0(y0), x1(x1), y1(y1) {};
    static Line fromQPoints(QPoint a, QPoint b) {        
        return Line(a.x(), a.y(), b.x(), b.y());
    }
};


class QTGELine : public QWidget
{
    private:       
        std::vector<Line> lines;

    private:
        QGridLayout* gridLayout;
        QPushButton* btFinish;
        QPushButton* btClearLines;
        
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
        
        // bresenham
        void bresenham(uchar* pixels, int width, int x0, int y0, int x1, int y1, QColor color);

        void setPainter(Painter* painter);

        void addLine(Line line);

    public slots:
        void addLineGUI();
        void clearLines();
        // callbacks dos radiobuttons
        inline void setBrenseham()  { setAlgoritm(0); };
        inline void setDDA()        { setAlgoritm(1); };
        inline void setAnalytic()   { setAlgoritm(2); };


};



#endif