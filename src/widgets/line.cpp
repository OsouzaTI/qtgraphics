#include "line.h"

QTGELine::QTGELine(QWidget* parent)
    : QWidget(parent)
{  
    gridLayout = new QGridLayout(this);

    btClearLines = new QPushButton("Clear lines");
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
    algorithm2 = new QRadioButton("Analitico");
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

    gridLayout->addWidget(btFinish,     4, 0, 1, 2);
    gridLayout->addWidget(btClearLines, 4, 2);

    connect(btFinish, &QPushButton::clicked, this, &QTGELine::addLineGUI);
    connect(btClearLines, &QPushButton::clicked, this, &QTGELine::clearLines);
    connect(algorithm0, &QRadioButton::clicked, this, &QTGELine::setBrenseham);
    connect(algorithm1, &QRadioButton::clicked, this, &QTGELine::setDDA);
    connect(algorithm2, &QRadioButton::clicked, this, &QTGELine::setAnalytic);

    show();
}

QTGELine::~QTGELine()
{

}

void QTGELine::analitica(uchar* pixels, int width, int height, int x0, int y0, int x1, int y1, QColor color) {
    
    // trocas para manter os pontos em ordem de menor para maior
    int _x0 = x0 > x1 ? x1 : x0;
    int _y0 = y0 > y1 ? y1 : y0;
    int _x1 = x0 > x1 ? x0 : x1;
    int _y1 = y0 > y1 ? y0 : y1;
    
    float m = (_y0 - _y1)/static_cast<float>(_x0 - _x1);
    
    for(int x = _x0; x < _x1; x++) {
        for(int y = _y0; y < _y1; y++) {

            bool equacao = (m * (_x0 - x) - _y0 + y) == 0;
            if(equacao) Pixels::setPixel(pixels, x, y, width, color);

        }
    }
    

}

void QTGELine::digitalDifferentialAnalyzer(uchar* pixels, int width, int x0, int y0, int x1, int y1, QColor color) {
    std::cout << "Color: " << color.alpha() << std::endl; 
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

void QTGELine::bresenham(uchar* pixels, int width, int x0, int y0, int x1, int y1, QColor color) {

    // definindo as variações
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);    
    int y = y0;
    int p = 2*dy - dx;

    for(int x = x0; x < x1; x++) {
        Pixels::setPixel(pixels, x, y, width, color);
        if(p >= 0) {
            y++;
            p = p - 2*dy + 2*dx;
        } else {
            p = p + 2*dy;
        }
    }

}

void QTGELine::infoPoints() {
    for(Line l : lines) {
        std::cout << "A("<<l.x0<<","<<l.y0<<") " << "B("<<l.x1<<","<<l.y1<< ")" << std::endl;
    }
}

void QTGELine::setAlgoritm(int algorithm) {
    std::cout << "Algoritmo " << algorithm << " foi selecionado." << std::endl;
    // seta o algoritmo
    this->algorithm = algorithm;

    // limpa a lista de callbacks do painter
    painter->clearPaintBufferCallbacks();
    // adiciona o novo algoritmo na lista
    painter->addPaintBufferCallback(ObjectType::LINE, [this, algorithm](uchar* pixels, int width, int height){
        
        // cada linha adicionada no vetor de linhas
        for(Line l : lines) {

            int x0 = l.x0,
                y0 = l.y0,
                x1 = l.x1,
                y1 = l.y1;

            switch (algorithm)
            {
                case 0: {
                    // brenseham
                    bresenham(pixels, width, x0, y0, x1, y1, QTGEWindow::colors[1]);
                } break;            
                case 1: {
                    // DDA
                    digitalDifferentialAnalyzer(pixels, width, x0, y0, x1, y1, QTGEWindow::colors[1]);
                } break;
                case 2: {  
                    analitica(pixels, width, height, x0, y0, x1, y1, QTGEWindow::colors[1]);
                } break;
                default:{
                    // brenseham
                }break;
            }
    
        }

    });

}

void QTGELine::setPainter(Painter* painter) {
    this->painter = painter;
    // padrao: brenseham
    setAlgoritm(0);
}

void QTGELine::addLine(Line line) {
    lines.push_back(line);
    painter->update();
}

void QTGELine::addLineGUI() {
    int x0, y0, x1, y1;
    x0 = tx0->text().toInt();
    y0 = ty0->text().toInt();
    x1 = tx1->text().toInt();
    y1 = ty1->text().toInt();
    lines.push_back(Line(x0, y0, x1, y1));

    // caso não tiver nenhum callback para pintar uma linha adicionada
    if(painter->sizeOfPaintBufferCallbacks(ObjectType::LINE) == 0) {
        setAlgoritm(algorithm);
    }

    painter->update();
}

void QTGELine::clearLines() {
    std::cout << "Limpando todos os callbacks" << std::endl;
    lines.clear();
    // remove todos os callbacks
    painter->clearPaintCallbacks(ObjectType::LINE);
    painter->clearPaintBufferCallbacks(ObjectType::LINE);    
    painter->clearBuffer();
    painter->update();
}