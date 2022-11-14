#include "window.h"

Rgb QTGEWindow::colors[2];

QTGEWindow::QTGEWindow(int width, int height, QWidget *parent) 
    : QWidget(parent), size(QSize(width, height))
{       
    setFixedSize(size);

    // painter
    painter = new Painter(640, 480);

    // componentes de tela
    line = new QTGELine();
    line->setPainter(painter);

    polygon = new QTGEPolygon(line);
    polygon->setPainter(painter);

    // layout
    layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    
    hbPreenchimento = new QHBoxLayout();

    // vbox1
    vbox1 = new QVBoxLayout();
    vbox2 = new QVBoxLayout();
    vbox2->setAlignment(Qt::AlignTop);
    vbox2->setSpacing(0);
    
    layout->addLayout(vbox1);
    layout->addLayout(vbox2);

    // UI
    selectBackgroundColor = new SelectColorButton(this);
    selectBackgroundColor->setText("Background Color");
    selectBackgroundColor->setSelectColorCallback([this](const QColor& color) -> void {
        this->setBackgroundColor(color);
    });

    selectPrimaryColor = new SelectColorButton(this);
    selectPrimaryColor->setText("Primary Color");
    selectPrimaryColor->setSelectColorCallback([this](const QColor& color) -> void {
        this->setPrimaryColor(color);
    });

    pixelSeedX = new QLineEdit();
    pixelSeedY = new QLineEdit();
    btFloodFill = new QPushButton("Flood Fill");
    btScanLine = new QPushButton("Scan Line");
    hbPreenchimento->addWidget(pixelSeedX);
    hbPreenchimento->addWidget(pixelSeedY);
    hbPreenchimento->addWidget(btFloodFill);
    hbPreenchimento->setSpacing(4);
    hbPreenchimento->setMargin(10);

    // adicionando elementos
    vbox1->addWidget(painter);
    vbox2->addWidget(selectBackgroundColor);
    vbox2->addWidget(selectPrimaryColor);
    vbox2->addWidget(line);
    vbox2->addLayout(hbPreenchimento);
    vbox2->addWidget(polygon);


    QTGEWindow::colors[0].changeColor(0, 0, 0);
    QTGEWindow::colors[1].changeColor(255, 255, 255);

    // conectando
    connect(btFloodFill, &QPushButton::clicked, this, &QTGEWindow::floodFill);

}

void QTGEWindow::setBackgroundColor(const QColor& color)
{
    QTGEWindow::colors[0] = color;
    painter->update();
}

void QTGEWindow::setPrimaryColor(const QColor& color)
{
    QTGEWindow::colors[1] = color;
    painter->update();
}

void QTGEWindow::resizeEvent(QResizeEvent* event) {

}

void QTGEWindow::keyPressEvent(QKeyEvent *event) {
    switch (event->key())
    {
        default: break; 
    }
}

void QTGEWindow::floodFill() {
    int x = pixelSeedX->text().toInt();
    int y = pixelSeedY->text().toInt();
    std::cout << "Flood fill pixelSeed(" << x << "," << y << ")" << std::endl;
    painter->floodFill(x, y, QColor(0, 0, 0), QColor(255, 0, 0));
    painter->update();
}