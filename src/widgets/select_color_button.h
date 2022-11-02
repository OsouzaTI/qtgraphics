#ifndef SELECT_COLOR_BUTTON_H
#define SELECT_COLOR_BUTTON_H

#include <QWidget>
#include <QPushButton>
#include <functional>

class SelectColorButton : public QPushButton
{
    private:
        QColor color;        
        std::function<void(const QColor&)> callback;
    public slots:
        void updateColor();
        void changeColor();
    public:
        SelectColorButton(QWidget* parent = 0);
        void setColor(const QColor& color);
        const QColor& getColor() const;
        void setSelectColorCallback(std::function<void(const QColor&)> callback);
};


#endif