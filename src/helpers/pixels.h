#ifndef PIXELS_H
#define PIXELS_H

#include <iostream>
#include <QtWidgets>

namespace Pixels {
    
    static int width = 160, height = 120;

    /**
     * Função generica para atribuir cor a um buffer
     * de pixels
     */

    static inline void setPixel(uchar* pixels, int x, int y, QColor color) {
        if((x > 0 && x < width) && (y > 0 && y < height)) {
            // calculo do indice do pixel
            int pixel = (x + y * width) * 4;
            pixels[pixel+0] = color.red();
            pixels[pixel+1] = color.green();
            pixels[pixel+2] = color.blue();
            pixels[pixel+3] = color.alpha();
        }
    }

}


#endif