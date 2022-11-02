#include "line.h"

QTGELine::QTGELine(QWidget* parent)
    : QWidget(parent), x0(0), y0(0), x1(0), y1(0)
{
    setStyleSheet("background-color: #3c3c3c;");    
    gridLayout = new QGridLayout(this);

    btFinish = new QPushButton("Rasterizar");

    lpointA = new QLabel("Point A:");
    lpointB = new QLabel("Point B:");

    lb0 = new QLabel("Rasterizar linha");
    tx0 = new QLineEdit();
    ty0 = new QLineEdit();
    tx1 = new QLineEdit();
    ty1 = new QLineEdit();

    lbAlgorithms = new QLabel("Selecione o algoritmo");
    boxAlgoritms = new QVBoxLayout();
    // radio buttons de algoritmos
    algorithm0 = new QRadioButton("Brenseham");
    algorithm1 = new QRadioButton("DDA");
    algorithm2 = new QRadioButton("Analirico");
    boxAlgoritms->addWidget(lbAlgorithms);
    boxAlgoritms->addWidget(algorithm0);
    boxAlgoritms->addWidget(algorithm1);
    boxAlgoritms->addWidget(algorithm2);

    // adicionando no grid
    gridLayout->addWidget(lb0, 0, 0, 1, 3, Qt::AlignCenter);    
    gridLayout->addWidget(lpointA, 1, 0);
    gridLayout->addWidget(lpointB, 2, 0);

    gridLayout->addWidget(tx0, 1, 1);
    gridLayout->addWidget(ty0, 1, 2);

    gridLayout->addWidget(tx1, 2, 1);
    gridLayout->addWidget(ty1, 2, 2);

    gridLayout->addLayout(boxAlgoritms, 3, 0, 1, 3, Qt::AlignCenter);

    gridLayout->addWidget(btFinish, 4, 0, 1, 3, Qt::AlignRight);

    connect(btFinish, &QPushButton::clicked, this, &QTGELine::setPoints);

    show();
}

QTGELine::~QTGELine()
{

}

QColor QTGELine::analitica(int x, int y) {

    // para determinar a equação geral da reta podemos:
    if((x0 - x1) == 0) return QColor(0, 0, 0);

    float m = (y0 - y1)/(x0 - x1);
    bool equacao = (m * (x0 - x) - y0 + y) == 0;
    return equacao ? QTGEWindow::colors[1] : QTGEWindow::colors[0];

}

void QTGELine::setPainter(Painter* painter) {

    this->painter = painter;

    painter->addPaintCallback([this](PixelAround p, int x, int y){
        return analitica(x, y);
    });

}

void QTGELine::setPoints() {
    std::cout << "Point A: " << tx0->text().toStdString() <<","<< ty0->text().toStdString() << std::endl;
    std::cout << "Point B: " << tx1->text().toStdString() <<","<< ty1->text().toStdString() << std::endl;
    x0 = tx0->text().toInt();
    y0 = ty0->text().toInt();
    x1 = tx1->text().toInt();
    y1 = ty1->text().toInt();
    painter->update();
}