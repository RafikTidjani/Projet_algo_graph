#include "coutinput.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidgetItem>
#include<QPushButton>

coutInput::coutInput(const vector<int> &fs, const vector<int> &aps, QWidget *parent) : QWidget{parent}, d_oriented{true}
{
    // On récupère les données
    this->fs = fs;
    this->aps = aps;
    // On consruit l'interface
    createInterface();
}

coutInput::coutInput(const vector<vector<int>> &matAdj, bool oriented, QWidget *parent) : QWidget{parent}, d_oriented{oriented}
{
    // On récupère les données
    this->matAdj = matAdj;
    // On consruit l'interface
    createInterface();
}

coutInput::~coutInput()
{
    delete d_mainLayout;
}

void coutInput::createInterface()
{
    // Création du layout principal
    d_mainLayout = new QVBoxLayout(this);

    // Titre de la fenêtre
    auto title = new QLabel("Saisie de la matrice des coûts associés");
    d_mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine {new QFrame{}};
    separationLine->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    d_mainLayout->addWidget(separationLine);

    // À optimiser pour fs aps
    if(fs.size() > 0) {
        // createInterfaceFSAPS();
        graphalgo::graph g{fs, aps};
        matAdj = g.mat_adj();
        createInterfaceMATADJ();

    } else  createInterfaceMATADJ();
}

void coutInput::createInterfaceFSAPS()
{
    // On déclare le tableau de saisie
    d_matrice = new QTableWidget();

    // On définit le nombre de lignes et de colonnes
    d_matrice->setRowCount(aps[0]);
    d_matrice->setColumnCount(aps[0]);

    for(int i = 0; i < aps[0]; i++) {
        // On définit la taille des cases
        d_matrice->setColumnWidth(i,30);
        d_matrice->setRowHeight(i, 30);
    }

    for(int lignes = 0; lignes < aps[0]; lignes++) {
        for(int colonnes = 0; colonnes < aps[0]; colonnes++) {
            QTableWidgetItem *item = new QTableWidgetItem{};
            item->setText("X");
            d_matrice->setItem(lignes, colonnes, item);
        }
    }

    // à compléter

    // On ajoute le tableau au layout principal
    d_mainLayout->addWidget(d_matrice);
}

void coutInput::createInterfaceMATADJ()
{
    // On déclare le tableau de saisi
    d_matrice = new QTableWidget{};

    // On définit le nombre de lignes et de colonnes
    d_matrice->setRowCount(matAdj[0][0]);
    d_matrice->setColumnCount(matAdj[0][0]);

    for(int i = 0; i < matAdj[0][0]; i++) {
        // On définit la taille des cases
        d_matrice->setColumnWidth(i, 30);
        d_matrice->setRowHeight(i, 30);
    }

    // On génère les cases de d_matrice
    for(int lignes = 0; lignes < matAdj[0][0]; lignes++) {
        for(int colonnes = 0; colonnes < matAdj[0][0]; colonnes++) {
            // On génère la case
            QTableWidgetItem *item = new QTableWidgetItem();

            // S'il existe une liaison
            if(matAdj[lignes + 1][colonnes + 1] != 0) {
                // On la rend éditable
                item->setText("0");
                item->setFlags(item->flags() | Qt::ItemIsEditable);

            // Sinon
            } else {
                item->setText("X");
                item->setFlags(item->flags() | Qt::NoItemFlags);
            }

            // On l'ajoute dans la matrice
            d_matrice->setItem(lignes, colonnes, item);
        }
    }

    // On ajoute le tableau au layout principal
    d_mainLayout->addWidget(d_matrice);
}

void coutInput::createGraph()
{
    graphalgo::graph newG{static_cast<unsigned int>(matAdj[0][0]), d_oriented};

    // On parcourt la matrice d'adjacence
    for(int lignes = 0; lignes < matAdj[0][0]; lignes++) {
        for(int colonnes = 0; colonnes < matAdj[0][0]; colonnes++) {
            // S'il existe une liaison
            if(matAdj[lignes + 1][colonnes + 1] != 0) {
                // On ajoute le successeur avec le coût saisi dans l'interface
                int cout = d_matrice->item(lignes, colonnes)->text().toUInt();
                newG.add_successor(lignes + 1, colonnes + 1, cout);
            }
        }
    }
    // On émet le nouveau graph
    emit graphe(newG);
}
