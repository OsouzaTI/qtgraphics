#include "circle.h"


QTGECircle::QTGECircle(QWidget* parent) : QTGEObject(parent) 
{

    layout = new QGridLayout(this);

    label = new QLabel("Circulo");

    tx0 = new QLineEdit();
    tx0->setPlaceholderText("Cx");
    ty0 = new QLineEdit();
    ty0->setPlaceholderText("Cy");
    tr = new QLineEdit();
    tr->setPlaceholderText("raio");

    // radio buttons de algoritmos
    algorithmLayout = new QHBoxLayout();
    algorithm0 = new QRadioButton("Parametrico");
    algorithm0->setChecked(true);
    algorithm1 = new QRadioButton("Bresenham");

    btClear = new QPushButton("Clear");
    btFinish = new QPushButton("rasterizar");

    layout->addWidget(label, 0, 0, 1, 3, Qt::AlignCenter);
    layout->addWidget(tx0, 1, 0);
    layout->addWidget(ty0, 1, 1);
    layout->addWidget(tr, 1, 2);

    algorithmLayout->addWidget(algorithm0);
    algorithmLayout->addWidget(algorithm1);
    layout->addLayout(algorithmLayout, 2, 0, 1, 3, Qt::AlignCenter);    

    layout->addWidget(btFinish, 3, 0, 1, 2);
    layout->addWidget(btClear, 3, 2);

    connect(btFinish, &QPushButton::clicked, this, &QTGECircle::addCircle);
    connect(btClear, &QPushButton::clicked, this, &QTGECircle::clearCircles);

    connect(algorithm0, &QRadioButton::clicked, this, &QTGECircle::setBresenham);
    connect(algorithm1, &QRadioButton::clicked, this, &QTGECircle::setParametrica);
}

void QTGECircle::parametrica(uchar* pixels, int width, int xc, int yc, int r, QColor color) {

    for (int angle = 0; angle < 360; angle++) {
        int x = xc + r * cos((angle * M_PI)/180.0f);
        int y = yc + r * sin((angle * M_PI)/180.0f);
        Pixels::setPixel(pixels, x, y, color);
    }

}

void QTGECircle::circunferenciaBresenham(uchar* pixels, int width, int xc, int yc, int r, QColor color) {

    int x = 0;
    int y = r;
    // como r é inteiro então p = 1 - r;
    int p = 1 - r;

    while(x <= y) {
        
        Pixels::setPixel(pixels, xc + x, yc + y, color);
        Pixels::setPixel(pixels, xc + x, yc - y, color);
        Pixels::setPixel(pixels, xc - x, yc - y, color);
        Pixels::setPixel(pixels, xc - x, yc + y, color);

        // usando a simetria para os demais quadrantes
        Pixels::setPixel(pixels, xc + y, yc + x, color);
        Pixels::setPixel(pixels, xc + y, yc - x, color);
        Pixels::setPixel(pixels, xc - y, yc - x, color);
        Pixels::setPixel(pixels, xc - y, yc + x, color);

        if(p > 0) {
            y--;
            p = p + 2*x - 2*y + 5;
            x++;
        } else {
            p = p + 2*x + 3;
            x++;
        }
    }

}

std::pair<int, int> QTGECircle::auxiliarPreenchimentoVarredura(int cx, int cy,  int r, int y) {
    // raio ao quadrado
    int r2 = r * r;
    // diferença entre y e o ycentral ao quadrado
    int dy = (y - cy) * (y - cy);
    
    int x0 = cx - sqrt(r2 - dy);
    int x1 = cx + sqrt(r2 - dy);
    
    return std::make_pair(x0 ,x1);

}

void QTGECircle::preenchimentoLinhaVarredura(uchar* pixels, int width, int cx, int cy, int r, QColor color) {    

    for(int y = cy - r; y < cy + r; y++) {
        
        std::pair<int, int> auxiliar = auxiliarPreenchimentoVarredura(cx, cy, r, y);
        for (int x = auxiliar.first; x < auxiliar.second; x++) 
        {
            Pixels::setPixel(pixels, x, y, color);
        }   

    }

}

void QTGECircle::setAlgoritm(int algorithm) {

    std::cout << "Algoritmo " << algorithm << " foi selecionado." << std::endl;
    // seta o algoritmo
    this->algorithm = algorithm;

    // limpa a lista de callbacks do painter
    painter->clearPaintBufferCallbacks(ObjectType::CIRCLE);
    // adiciona o novo algoritmo na lista
    painter->addPaintBufferCallback(ObjectType::CIRCLE, [this, algorithm](uchar* pixels, int width, int height){
        
        // cada linha adicionada no vetor de linhas
        for(Circle c : circles) {
            // somente possuo o circulo parametrico
            
            switch (algorithm)
            {
                case 0: {
                    // parametrico
                    // parametrica(pixels, width, c.cx, c.cy, c.r, QTGEWindow::colors[1]);
                    preenchimentoLinhaVarredura(pixels, width, c.cx, c.cy, c.r, QTGEWindow::colors[1]);
                } break;            
                case 1: {
                    // brenseham
                    circunferenciaBresenham(pixels, width, c.cx, c.cy, c.r, QTGEWindow::colors[1]);                    
                } break;
            }

        }

    });

}

void QTGECircle::addCircle() {
    int cx, cy, r;
    cx = tx0->text().toInt();
    cy = ty0->text().toInt();
    r  = tr->text().toInt();
    circles.push_back(Circle(cx, cy, r));

    // caso não tiver nenhum callback para pintar o circulo adicionado
    if(painter->sizeOfPaintBufferCallbacks(ObjectType::CIRCLE) == 0) {
        setAlgoritm(algorithm);
    }

    painter->update();
}

void QTGECircle::clearCircles() {
    circles.clear();
    painter->clearPaintCallbacks(ObjectType::CIRCLE);
    painter->clearPaintBufferCallbacks(ObjectType::CIRCLE);
    painter->clearBuffer();
    painter->update();
}