#include <QColorDialog>
#include "select_color_button.h"

SelectColorButton::SelectColorButton(QWidget* parent)
    : QPushButton(parent)
{
    connect(this, &SelectColorButton::clicked, this, &SelectColorButton::changeColor);
}

void SelectColorButton::updateColor() 
{
    setStyleSheet( "background-color: " + color.name() );
}

void SelectColorButton::changeColor()
{
    QColor newColor = QColorDialog::getColor(color, parentWidget());
    if (newColor.isValid() && newColor != color) {
        setColor(newColor);
    }
}

void SelectColorButton::setColor(const QColor& color)
{
    this->color = color;
    callback(color);
    updateColor();
}

const QColor& SelectColorButton::getColor() const
{
    return color;
}

void SelectColorButton::setSelectColorCallback(std::function<void(const QColor&)> callback)
{   
    this->callback = callback;
}