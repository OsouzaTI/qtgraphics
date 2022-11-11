#ifndef OBJECT_H
#define OBJECT_H

#include <core/painter.h>
#include <QWidget>

class QTGEObject : public QWidget {

protected:
    Painter* painter;
public:

    inline QTGEObject(QWidget* parent = 0) : QWidget(parent) {};
    inline void setPainter(Painter* painter) { 
        this->painter = painter;
    }

};

#endif