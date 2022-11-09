#include "window.h"

Rgb QTGEWindow::colors[2];

QTGEWindow::QTGEWindow(int width, int height, QWidget *parent) 
    : QWidget(parent), size(QSize(width, height))
{       
    setFixedSize(size);

    // painter
    painter = new Painter(640, 480);

    // componentes de tela
    line = new QTGELine(this);
    line->setPainter(painter);

    // layout
    layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    
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

    // adicionando elementos
    vbox1->addWidget(painter);
    vbox2->addWidget(selectBackgroundColor);
    vbox2->addWidget(selectPrimaryColor);
    vbox2->addWidget(line);


    QTGEWindow::colors[0].changeColor(0, 0, 0);
    QTGEWindow::colors[1].changeColor(255, 255, 255);

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