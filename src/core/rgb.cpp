#include "rgb.h"

Rgb::~Rgb() {}

void Rgb::randomColor() {
    r = rand() % 255;
    g = rand() % 255;
    b = rand() % 255;
}

void Rgb::changeColor(int r, int g, int b) {
    this->r = r;
    this->g = g;
    this->b = b;
}