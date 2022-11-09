#include "line.h"

QTGELine::QTGELine(QWidget* parent)
    : QWidget(parent), x0(0), y0(0), x1(0), y1(0)
{  
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
    algorithm0->setChecked(true);
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
    connect(algorithm0, &QRadioButton::clicked, this, &QTGELine::setBrenseham);
    connect(algorithm1, &QRadioButton::clicked, this, &QTGELine::setDDA);
    connect(algorithm2, &QRadioButton::clicked, this, &QTGELine::setAnalytic);

    show();
}

QTGELine::~QTGELine()
{

}

void QTGELine::analitica(uchar* pixels, int width, int height, int x0, int y0, int x1, int y1, QColor color) {

    float m = (y0 - y1)/(x0 - x1);
    
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {

            bool equacao = (m * (x0 - x) - y0 + y) == 0;
            if(equacao) Pixels::setPixel(pixels, x, y, width, color);

        }
    }
    

}

void QTGELine::digitalDifferentialAnalyzer(uchar* pixels, int width, int x0, int y0, int x1, int y1, QColor color) {

    // calculando os respectivos deltas
    int deltaX = (x1 - x0);
    int deltaY = (y1 - y0);

    // obtendo o maior delta e salvando-o
    int sideLength = (abs(deltaX) >= abs(deltaY)) ? abs(deltaX) : abs(deltaY);

    // definindo a taxa de incremento dos eixos
    float incrementX = deltaX / static_cast<float>(sideLength);
    float incrementY = deltaY / static_cast<float>(sideLength);

    // valor atual dos incrementos
    float currentX = x0;
    float currentY = y0;

    for(int i = 0; i < sideLength; i++) {
        int x = static_cast<int>(currentX);
        int y = static_cast<int>(currentY);        
        // definindo o pixel
        Pixels::setPixel(pixels, x, y, width, color);        
        currentX += incrementX;
        currentY += incrementY;
    }

}

void QTGELine::setAlgoritm(int algorithm) {
    std::cout << "Algoritmo " << algorithm << " foi selecionado." << std::endl;
    // seta o algoritmo
    this->algorithm = algorithm;

    // limpa a lista de callbacks do painter
    painter->clearPaintBufferCallbacks();
    // adiciona o novo algoritmo na lista
    painter->addPaintBufferCallback([this, algorithm](uchar* pixels, int width, int height){
        
        switch (algorithm)
        {
            case 0: {
                // brenseham
            } break;            
            case 1: {
                // DDA
                digitalDifferentialAnalyzer(pixels, width, x0, y0, x1, y1, QColor(255, 0, 0));
            } break;
            case 2: {  
                analitica(pixels, width, height, x0, y0, x1, y1, QColor(255, 0, 0));
            } break;
            default:{
                // brenseham
            }break;
        }
    
    });

}

void QTGELine::setPainter(Painter* painter) {
    this->painter = painter;
    // padrao: brenseham
    setAlgoritm(0);
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