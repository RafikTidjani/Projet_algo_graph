#include "matadjinput.h"
#include "coutinput.h"
#include <QBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QTableWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <QFrame>

// PUBLIC

matAdjInput::matAdjInput(QWidget *parent)
    : QWidget{parent}
{
    createInterface();
}

matAdjInput::~matAdjInput()
{
    // delete les pointeurs
    delete d_mainLayout;
    delete d_preSaisieLayout;
    delete d_valideLayout;
    delete d_horizonthalLayout;
    delete d_coutLayout;
    delete d_lNbSommets;
    delete d_lOrientation;
    delete d_inputnbSommets;
    delete d_radioButtonOriente;
    delete d_bValide;
    delete d_matrice;
}

// PRIVÉ

void matAdjInput::createInterface()
{
    // On génère le layout principal
    d_mainLayout = new QVBoxLayout{this};

    // On ajoute le titre
    auto title {new QLabel{"Saisie d'un graph selon une matrice d'adjacence"}};
    d_mainLayout->addWidget(title);

    // On ajoute la ligne de séparation
    auto titleLigne {new QFrame{this}};
    titleLigne->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    d_mainLayout->addWidget(titleLigne);

    // On génère un QHBoxLayout pour les données de pré-saisie
    d_preSaisieLayout = new QHBoxLayout{};
    d_mainLayout->addLayout(d_preSaisieLayout);

    // On génère les inputs et bales de saisie
    d_lNbSommets = new QLabel{"Saisir le nombre de sommets : "};
    d_preSaisieLayout->addWidget(d_lNbSommets);
    d_inputnbSommets = new QLineEdit{};
    d_preSaisieLayout->addWidget(d_inputnbSommets);
    d_lOrientation = new QLabel{"Orienté : "};
    d_preSaisieLayout->addWidget(d_lOrientation);
    d_radioButtonOriente  = new QCheckBox{};
    d_preSaisieLayout->addWidget(d_radioButtonOriente);

    // Choix de l'ajout ou non de coûts
    d_coutLayout = new QHBoxLayout{};
    d_mainLayout->addLayout(d_coutLayout);
    d_labelCout = new QLabel{"Avec coûts :"};
    d_coutLayout->addWidget(d_labelCout);
    d_checkCouts = new QCheckBox{};
    d_coutLayout->addWidget(d_checkCouts);
    d_coutLayout->addStretch(1);

    // On génère le layout horizonthal contenant la matrice et le bouton de validation
    d_horizonthalLayout = new QHBoxLayout{};
    d_mainLayout->addLayout(d_horizonthalLayout);

    // On génère la matrice
    d_matrice = new QTableWidget{};
    d_horizonthalLayout->addWidget(d_matrice);

    // On génère le layout contenant le bouton de validation
    d_valideLayout = new QVBoxLayout{};
    d_horizonthalLayout->addLayout(d_valideLayout);

    // On génère le bouton de validation
    d_bValide = new QPushButton{"Valider"};
    d_valideLayout->addStretch(1);
    d_valideLayout->addWidget(d_bValide);

    // Connexion du changement de la matrice
    connect(d_inputnbSommets, &QLineEdit::textChanged, [=](const QString &text) {
        bool ok;
        unsigned value = text.toUInt(&ok);

        if (ok) onNbSommets(value);
    });
    // Connexion du bouton valider
    connect(d_bValide, &QPushButton::clicked, this, &matAdjInput::createGraph);
}

void matAdjInput::updateInterface(const vector<vector<int>> &matAdj)
{
    bool oriented = d_radioButtonOriente->isChecked();

    delete d_preSaisieLayout;
    delete d_valideLayout;
    delete d_horizonthalLayout;
    delete d_lNbSommets;
    delete d_lOrientation;
    delete d_inputnbSommets;
    delete d_radioButtonOriente;
    delete d_bValide;
    delete d_matrice;

    d_coutinput = new coutInput{matAdj, oriented};
    d_mainLayout->addWidget(d_coutinput);
    d_bValide = new QPushButton{"Valider"};
    d_mainLayout->addWidget(d_bValide);

    connect(d_coutinput, &coutInput::graphe, this, &matAdjInput::onGraphReceived);
    connect(d_bValide, &QPushButton::clicked, d_coutinput, &coutInput::createGraph);
}

void matAdjInput::createMatrice(unsigned i)
{
    // On alloue les lignes et colonnes
    d_matrice->setColumnCount(i);
    d_matrice->setRowCount(i);

    for(unsigned j = 0; j < i; j++) {
        // On définit la taille des cases
        d_matrice->setColumnWidth(j,30);
        d_matrice->setRowHeight(j, 30);
    }

    // On génère les cases
    for(unsigned lignes = 0; lignes < i; lignes++){
        for(unsigned colonnes = 0; colonnes < i; colonnes++){
            // On génère la case
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText("0");
            // On la rend éditable
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            // On l'ajoute dans la matrice
            d_matrice->setItem(lignes, colonnes, item);
        }
    }
}

void matAdjInput::deleteMatrice()
{
    d_matrice->clear();
}

bool matAdjInput::recupereValide(std::vector<std::vector<int>> &matAdj) const
{
    // On déclare les variables tampons
    bool valide = true;
    int nbLignes = d_matrice->rowCount();

    // On alloue et prépare la matrice
    matAdj.clear();
    matAdj.resize(nbLignes + 1, std::vector<int>(nbLignes + 1, 0));
    matAdj[0][0] = nbLignes;

    // On initialise un compteur
    int compt = 0;

    // On fait défiler les lignes
    int i = 0;
    while(valide && i < nbLignes){
        // On fait défiler les colonnes
        int j = 0;
        while(valide && j < nbLignes){
            // On récupère la valeur de la case
            bool isNumber;
            int number = d_matrice->item(i, j)->text().toUInt(&isNumber);

            // On vérifie que la case est valide
            if (!isNumber){
                valide = false;
            } else if(number != 0) {
                // On compte les arrêtes
                compt++;
                // on implémente les données
                matAdj[i +1 ][j + 1] = number;
            }
            j++;
        }
        i++;
    }
    // On implémente le nombre d'arrêtes
    matAdj[0][1] = compt;
    // On retourne la validation
    return valide;
}

void matAdjInput::createGraph()
{
    // générér le graph selon un tableau 2D d'entier
    std::vector<std::vector<int>> matAdj;
    if(recupereValide(matAdj)){
        // Graph avec coûts
        if(d_checkCouts->isChecked()){
            updateInterface(matAdj);

        // Graph sans coûts / orienté
        } else if(d_radioButtonOriente->isChecked()) {
            // On construit le graph orienté à partie de la matrice d'adjacence
            graphalgo::graph g{matAdj, true};
            emit graphe(g);

        // Graph sans coûts / non-orienté
        } else {
            // On construit le graph non-orienté à partie de la matrice d'adjacence
            graphalgo::graph g{matAdj, false};
            emit graphe(g);
        }
    } else {
        QMessageBox::critical(nullptr, "Erreur de saisie", "La matrice d'adjacence est invalide.");
    }
}

// PRIVATE SLOTS

void matAdjInput::onNbSommets(unsigned i)
{
    deleteMatrice();
    createMatrice(i);
}

void matAdjInput::onGraphReceived(graphalgo::graph &g)
{
    emit graphe(g);
}

