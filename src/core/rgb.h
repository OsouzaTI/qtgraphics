#ifndef RGB_H
#define RGB_H

#include <iostream>
#include <QWidget>

class Rgb
{
    private:
        int r, g, b;
    public:
        Rgb() : r(0), g(0), b(0) {};
        Rgb(int r, int g, int b) : r(r), g(g), b(b) {};
        ~Rgb();

        int inline red(){ return r; }
        int inline green(){ return g; }
        int inline blue(){ return b; }

        void randomColor();
        void changeColor(int r, int g, int b);
        operator QColor() const { return QColor(r, g, b); }

        void operator= (QColor& color) {
            r = color.red();
            g = color.green();
            b = color.blue();
        }

        void operator= (const QColor& color) {
            r = color.red();
            g = color.green();
            b = color.blue();
        }

};



#endif