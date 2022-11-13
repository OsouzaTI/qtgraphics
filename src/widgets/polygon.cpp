#include "polygon.h"

QTGEPolygon::QTGEPolygon(QTGELine* line, QWidget* parent)
    : QTGEObject(parent), line(line)
{
    // layout
    layout = new QVBoxLayout(this);

    // dialogo de arquivo
    openFile = new QPushButton("Open file");

    // model
    model = new QStringListModel();

    // lista de vertices
    listVertices = new QListView();

    // adicionando componentes na lista de visualização
    layout->addWidget(openFile);
    layout->addWidget(listVertices);

    // conectando
    connect(openFile, &QPushButton::clicked, this, &QTGEPolygon::openFileDialog);

    show();
}

void QTGEPolygon::render() {

    // TODO: renderizar os vertices do poligono no painter

}

void QTGEPolygon::readFile(FILE* file) {
    // limpa a lista atual de vertices adicionados
    vertices.clear();

    QStringList listString;
    int index = 0;
    while (1) {
        //buffer
        char lineHeader[128];
        // lendo a linha
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;

        if (strcmp(lineHeader, "V") == 0) {
            int x, y;
            fscanf(file, "%d %d", &x, &y);
            QPoint vertex(x, y);
            vertices.push_back(vertex);

            // adicionando na lista de visualizacao
            listString << "V" + QString::number(index) + "(" + QString::number(x) + "," + QString::number(y) + ")";
            index++;
        }
    }

    model->setStringList(listString);
    // atualizando a lista de vertices
    listVertices->setModel(model);

    // passando os dados lidos para o rasterizador de linhas
    int vSize = vertices.size();
    for(int i = 0; i < vSize - 1; i++) {        
        QPoint A = vertices[i];
        QPoint B = vertices[i+1];        
        line->addLine(Line::fromQPoints(A, B));
    }

    // fechando a figura automaticamente
    line->addLine(Line::fromQPoints(vertices.back(), vertices.front()));

}


void QTGEPolygon::openFileDialog() {

    QString fileName = QFileDialog::getOpenFileName(this, "Open a Polygon File", QString(), "*.gepoly");
    std::cout << fileName.toStdString() << std::endl;
    FILE* file = fopen(fileName.toStdString().c_str(), "r");
	if (file == NULL) {
		std::cout << "Impossible to open the file !" << std::endl;
	} else {
        readFile(file);
    }

}