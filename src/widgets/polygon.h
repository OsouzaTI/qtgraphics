#ifndef POLYGON_H
#define POLYGON_H

#include <iostream>
#include <QListView>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPushButton>
#include "object.h"
#include "line.h"

class QTGELine;

class QTGEPolygon : public QTGEObject {

private:
    std::vector<QPoint> vertices;

    QStringListModel* model;
    QListView* listVertices;
    QVBoxLayout* layout;
    QPushButton* openFile;

    QTGELine* line;

public:
    QTGEPolygon(QTGELine* line, QWidget* parent = 0);
    void render();  
    void readFile(FILE* file);    
public slots:
    void openFileDialog();
};


#endif