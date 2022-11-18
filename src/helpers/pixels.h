#ifndef PIXELS_H
#define PIXELS_H

#include <iostream>
#include <QtWidgets>

namespace Pixels {

    /**
     * Função generica para atribuir cor a um buffer
     * de pixels
     */

    static inline void setPixel(uchar* pixels, int x, int y, int width, QColor color) {
        if(x < 0 || y < 0 || x > width) return;
        // calculo do indice do pixel
        int pixel = (x + y * width) * 4;
        pixels[pixel+0] = color.red();
        pixels[pixel+1] = color.green();
        pixels[pixel+2] = color.blue();
        pixels[pixel+3] = color.alpha();
    }

}


#endif