#include "line.h"

QTGELine::QTGELine(QWidget* parent)
    : QWidget(parent)
{  
    gridLayout = new QGridLayout(this);

    btClearLines = new QPushButton("Clear lines");
    btFinish = new QPushButton("Rasterizar");

    lb0 = new QLabel("Rasterizar linha");
    tx0 = new QLineEdit();
    ty0 = new QLineEdit();
    tx1 = new QLineEdit();
    ty1 = new QLineEdit();
    tx0->setPlaceholderText("x0");
    ty0->setPlaceholderText("y0");
    tx1->setPlaceholderText("x1");
    ty1->setPlaceholderText("y1");

    boxAlgoritms = new QHBoxLayout();
    // radio buttons de algoritmos
    algorithm0 = new QRadioButton("Brenseham");
    algorithm0->setChecked(true);
    algorithm1 = new QRadioButton("DDA");
    algorithm2 = new QRadioButton("Analitico");
    boxAlgoritms->addWidget(algorithm0);
    boxAlgoritms->addWidget(algorithm1);
    boxAlgoritms->addWidget(algorithm2);

    // adicionando no grid
    gridLayout->addWidget(lb0, 0, 0, 1, 4, Qt::AlignCenter);    
    gridLayout->addWidget(tx0, 1, 0);
    gridLayout->addWidget(ty0, 1, 1);
    gridLayout->addWidget(tx1, 1, 2);
    gridLayout->addWidget(ty1, 1, 3);
    gridLayout->addLayout(boxAlgoritms, 2, 0, 1, 4, Qt::AlignCenter);
    gridLayout->addWidget(btFinish,     3, 0, 1, 2);
    gridLayout->addWidget(btClearLines, 3, 2, 1, 2);

    connect(btFinish, &QPushButton::clicked, this, &QTGELine::addLineGUI);
    connect(btClearLines, &QPushButton::clicked, this, &QTGELine::clearLines);
    connect(algorithm0, &QRadioButton::clicked, this, &QTGELine::setBresenham);
    connect(algorithm1, &QRadioButton::clicked, this, &QTGELine::setDDA);
    connect(algorithm2, &QRadioButton::clicked, this, &QTGELine::setAnalytic);

    show();
}

QTGELine::~QTGELine()
{

}

void QTGELine::analitica(uchar* pixels, int x0, int y0, int x1, int y1, QColor color) {

    if(x0 > x1 && y0 > y1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    // verifica se a reta é vertical
    if(x0 == x1) {

        for (int y = y0; y < y1; y++)
        {
            Pixels::setPixel(pixels, x0, y, color);
        }
        

    } else {

        float m = (y1 - y0) / (float)(x1 - x0);
        float b = y1 - m * x1;

        for (int x = x0; x < x1; x++)
        {
            float y = m * x + b;                        
            Pixels::setPixel(pixels, x, round(y), color);
        }

    }
    

}

void QTGELine::digitalDifferentialAnalyzer(uchar* pixels, int x0, int y0, int x1, int y1, QColor color) {
    
    // calculando os respectivos deltas
    int deltaX = x1 - x0;
    int deltaY = y1 - y0;

    // escolhendo o maior delta para o calculo dos incrementos
    int mdelta = abs(deltaX) > abs(deltaY) ? abs(deltaX) : abs(deltaY);

    // calculando incrementos
    float incx = deltaX / (float)mdelta;
    float incy = deltaY / (float)mdelta;

    float x = x0, y = y0;
    for (int i = 0; i < mdelta; i++)
    {
 
        Pixels::setPixel(pixels, round(x), round(y), color);

        x += incx;
        y += incy;

    }
    
    
}

void QTGELine::bresenham(uchar* pixels, int x0, int y0, int x1, int y1, QColor color) {
    
    // definindo as variações
    int dx = x1 - x0;
    int dy = y1 - y0;    
    
    int xIncremento = x0 < x1 ? 1 : -1;
    int yIncremento = y0 < y1 ? 1 : -1;

    // troca de valores
    int y = y0;
    int x = x0;
   
    // caso a variação em y seja maior que em x
    if(dx < dy) {

        Pixels::setPixel(pixels, x, y, color);
        int p = 2*dy - dx;

        // percorre os valores de dx
        for(int i = 0; i < dx; i++) {
            x += xIncremento;

            // procurando o proximo pixel
            if(p < 0) {
                p = p + 2*dy;
            } else {
                y += yIncremento;
                p = p + 2*dy - 2*dx;
            }

            if(y1 < y0) {
                Pixels::setPixel(pixels, x, -y, color);
            } else {
                Pixels::setPixel(pixels, x, y, color);
            }

        }

    } 
    // caso a variação de x seja maior que a variação de y
    else {

        /***
         * Sera realizada a troca de todos os 'x' por 'y'
         * */

        Pixels::setPixel(pixels, y, x, color);
        int p = 2*dx - dy;

        // percorre os valores de dx
        for(int i = 0; i < dy; i++) {
            y += yIncremento;

            // procurando o proximo pixel
            if(p < 0) {
                p = p + 2*dx;
            } else {
                x += xIncremento;
                p = p + 2*dx - 2*dy;
            }

 
            if(y1 < y0) {
                Pixels::setPixel(pixels, -y, x, color);
            } else {
                Pixels::setPixel(pixels, y, x, color);
            }
            
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
    painter->clearPaintBufferCallbacks(ObjectType::LINE);
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
                    bresenham(pixels, x0, y0, x1, y1, QTGEWindow::colors[1]);
                } break;            
                case 1: {
                    // DDA
                    digitalDifferentialAnalyzer(pixels, x0, y0, x1, y1, QTGEWindow::colors[1]);
                } break;
                case 2: {  
                    analitica(pixels, x0, y0, x1, y1, QTGEWindow::colors[1]);
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