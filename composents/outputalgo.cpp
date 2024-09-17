#include "outputalgo.h"
#include "graph/algorithmes.h"
#include "graphview.h"
#include <QPushButton>

outputAlgo::outputAlgo(int indiceOutput, graphalgo::graph &g, QWidget *parent)
    : QWidget(parent)
{
    switch(indiceOutput) {
        case 0:
            distance(g);
            break;

        case 1:
            rang(g);
            break;

        case 2:
            tarjan(g);
            break;

        case 4:
            dijkstra(g);
            break;

        case 5:
            dantzig(g);
            break;

        case 6:
            kruskal(g);
            break;

        case 7:
            prufer(g);
            break;

        default: break;
    }
}

outputAlgo::~outputAlgo()
{
    delete d_comboBoxDistance;
    delete d_comboBoxDijkstra;
    delete d_resultatsDistance;
    delete d_graphViewDijkstra;
}

void outputAlgo::calculerDistances(graphalgo::graph &g)
{
    // Récupérer le sommet choisi dans la combobox
    int sommet = d_comboBoxDistance->currentIndex() + 1;

    // Récupérer les tableaux FS et APS du graphe
    std::vector<int> fs, aps;
    g.fs_aps(fs, aps);

    // Calculer les distances par rapport au sommet choisi
    std::vector<int> dist = graphalgo::distance(sommet, fs, aps);

    // Mettre à jour l'affichage des résultats
    QString resultatStr;
    for (int i = 1; i <= dist[0]; i++) {
        resultatStr += "Sommet " + QString::number(i) + " : " + QString::number(dist[i]) + "\n";
    }
    d_resultatsDistance->setText(resultatStr);
}

void outputAlgo::distance(graphalgo::graph &g)
{
    // Création du layout
    auto mainLayout {new QVBoxLayout{this}};

    // Titre de la fenêtre
    auto title = new QLabel("Affichage des distances par rapport à un sommet");
    mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine = new QFrame;
    separationLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    mainLayout->addWidget(separationLine);

    // Création de la combobox pour choisir le sommet et du layout
    auto layoutSommet {new QHBoxLayout{}};
    auto sommetLabel = new QLabel("Choisissez le sommet de référence :");
    d_comboBoxDistance = new QComboBox;

    // On remplit la combobox
    d_comboBoxDistance = new QComboBox;
    for(int i = 1; i <= g.n(); i++) {
        d_comboBoxDistance->addItem(QString::number(i));
    }
    // On fait les ajouts aux différents layouts
    layoutSommet->addWidget(sommetLabel);
    layoutSommet->addWidget(d_comboBoxDistance);
    mainLayout->addLayout(layoutSommet);

    // Création du label pour afficher les résultats
    d_resultatsDistance = new QLabel;
    mainLayout->addWidget(d_resultatsDistance);

    // Connexion de la méthode pour le changement de valeur au calcul des distances
    connect(d_comboBoxDistance, &QComboBox::currentIndexChanged, [this, &g] {
        calculerDistances(g);
    });
}

void outputAlgo::rang(graphalgo::graph &g)
{
    // Création du layout
    auto mainLayout {new QVBoxLayout{this}};

    // Titre de la fenêtre
    auto title = new QLabel("Affichage du rang des sommets");
    mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine = new QFrame;
    separationLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    mainLayout->addWidget(separationLine);

    // Création du label pour afficher les résultats
    auto resultatsRang {new QLabel};
    mainLayout->addWidget(resultatsRang);

    // Calculer les rangs
    std::vector<int> fs, aps;
    g.fs_aps(fs, aps);
    std::vector<int> rang = graphalgo::rang(fs, aps);

    // Mettre à jour l'affichage des résultats
    QString resultatStr;
    for (int i = 1; i <= rang[0]; i++) {
        resultatStr += "Sommet " + QString::number(i) + " : " + QString::number(rang[i]) + "\n";
    }
    resultatsRang->setText(resultatStr);
}

void outputAlgo::tarjan(graphalgo::graph &g)
{
    // Création du layout
    auto mainLayout = new QVBoxLayout{this};

    // Titre de la fenêtre
    auto title = new QLabel("Affichage du graphe réduit selon Tarjan");
    mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine = new QFrame;
    separationLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    mainLayout->addWidget(separationLine);

    // Récupérer le résultat de Kruskal
    /*graphalgo::graph p{8, true};
    p.add_successor(1, 2);
    p.add_successor(1, 3);
    p.add_successor(1, 4);
    p.add_successor(3, 6);
    p.add_successor(4, 7);
    p.add_successor(5, 2);
    p.add_successor(5, 6);
    p.add_successor(5, 1);
    p.add_successor(6, 3);
    p.add_successor(6, 7);
    p.add_successor(6, 8);
    p.add_successor(7, 4);
    p.add_successor(8, 7);*/

    /*graphalgo::graph p{10, true};
    p.add_successor(1, 2);
    p.add_successor(1, 3);
    p.add_successor(2, 4);
    p.add_successor(2, 5);
    p.add_successor(4, 6);
    p.add_successor(5, 6);
    p.add_successor(6, 7);
    p.add_successor(7, 3);
    p.add_successor(7, 5);
    p.add_successor(7, 2);
    p.add_successor(8, 1);
    p.add_successor(8, 9);
    p.add_successor(9, 10);
    p.add_successor(10, 4);
    p.add_successor(10, 9);
    p.add_successor(10, 8);*/

    vector<int> fs, aps, fsr, apsr, prem, pilch, cfc, pred, br;
    // p.fs_aps(fs, aps);
    g.fs_aps(fs, aps);
    // On calcule le graph réduit
    graphalgo::fortconnexe(fs, aps, prem, pilch, cfc, pred);
    graphalgo::graph gr = graphalgo::graph_reduit(prem, pilch, cfc, fs, aps);

    // On récupère les bases
    gr.fs_aps(fsr, apsr);
    graphalgo::base_Greduit(apsr, fsr, br);
    QString bases = QString::fromStdString(graphalgo::edition_bases(prem, pilch, br));

    auto graphview {new graphView{gr}};
    mainLayout->addWidget(graphview);

    auto results {new QLabel{}};
    results->setText(bases);
    mainLayout->addWidget(results);
}

void outputAlgo::calculerDijkstra(graphalgo::graph& g)
{
    // Récupérer le sommet choisi dans la combobox
    int sommet = d_comboBoxDijkstra->currentIndex() + 1;

    // Calculer les chemins les plus courts par rapport au sommet choisi
    graphalgo::graph T = graphalgo::dijkstra(sommet, g);

    // Mettre à jour le graph
    d_graphViewDijkstra->graphChanged(T);
}

void outputAlgo::dijkstra(graphalgo::graph &g) {
    // Création du layout
    auto mainLayout {new QVBoxLayout{this}};

    // Titre de la fenêtre
    auto title = new QLabel("Affichage des chemins les plus courts selon Dijkstra");
    mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine = new QFrame;
    separationLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    mainLayout->addWidget(separationLine);

    // Création de la combobox pour choisir le sommet et du layout
    auto layoutSommet {new QHBoxLayout{}};
    auto sommetLabel = new QLabel{"Choisissez le sommet de référence :"};
    d_comboBoxDijkstra = new QComboBox;

    // On remplit la combobox
    d_comboBoxDijkstra = new QComboBox;
    for(int i = 1; i <= g.n(); i++) {
        d_comboBoxDijkstra->addItem(QString::number(i));
    }
    // On fait les ajouts aux différents layouts
    layoutSommet->addWidget(sommetLabel);
    layoutSommet->addWidget(d_comboBoxDijkstra);
    mainLayout->addLayout(layoutSommet);

    // Récupération du résultat du sommet 1 et création du graphView
    graphalgo::graph sommet1 = graphalgo::dijkstra(1, g);
    d_graphViewDijkstra = new graphView{sommet1};
    // Création du graphView pour afficher les résultats
    mainLayout->addWidget(d_graphViewDijkstra);

    connect(d_comboBoxDijkstra, &QComboBox::currentIndexChanged, [this, &g] {
        calculerDijkstra(g);
    });
}

void outputAlgo::dantzig(graphalgo::graph &g)
{
    // Création du layout
    auto mainLayout = new QVBoxLayout{this};

    // Titre de la fenêtre
        auto title = new QLabel("Affiche de l'algorithme de Dantzig");
    mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine = new QFrame;
    separationLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    mainLayout->addWidget(separationLine);

    vector<vector<int>> res = g.cost_matrice();
    if(graphalgo::dantzig(res)) {
        graphalgo::graph temp{g.mat_adj(), g.oriented()};

        // Mettre à jour les coûts dans le graphe temp
        for (int i = 1; i <= temp.n(); i++)
        {
            for(int j = 1; j <= temp.n(); j++)
            {
                if(res[i - 1][j - 1] != -__INT_MAX__)
                {
                    temp.cost(i, j, res[i - 1][j - 1]);
                }
            }
        }

        // Afficher le graphe mis à jour
        auto graphV = new graphView{temp};
        mainLayout->addWidget(graphV);

    } else {
        auto results {new QLabel{"Il y a la présence d'un circuit absorbant, impossible d'exécuter Dantzig."}};
        mainLayout->addWidget(results);
    }

}

void outputAlgo::kruskal(graphalgo::graph& g) {
    // Création du layout
    auto mainLayout = new QVBoxLayout{this};

    // Titre de la fenêtre
    auto title = new QLabel("Affichage de l'arbre recouvrant minimal d'un graphe non orienté selon Kruskal");
    mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine = new QFrame;
    separationLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    mainLayout->addWidget(separationLine);

    // Récupérer le résultat de Kruskal
    graphalgo::graph T = graphalgo::kruskal(g);
    auto graphview {new graphView{T}};
    mainLayout->addWidget(graphview);
}

void outputAlgo::prufer(graphalgo::graph& g)
{
    // Création du layout
    auto mainLayout = new QVBoxLayout{this};

    // Titre de la fenêtre
    auto title = new QLabel("Affichage du codage de Prufer");
    mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine = new QFrame;
    separationLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    mainLayout->addWidget(separationLine);

    // Création du label pour afficher les résultats
    auto resultatsLabel = new QLabel;
    mainLayout->addWidget(resultatsLabel);

    // Calculer le codage de Prufer
    std::vector<std::vector<int>> adj = g.mat_adj();
    std::vector<int> prf = graphalgo::prufer(adj);

    // Mettre à jour l'affichage des résultats
    QString resultatStr = "Codage de Prufer : ";
    for (unsigned i = 0; i < prf.size(); i++) {
        resultatStr += QString::number(prf[i] + 1) + " ";
    }
    resultatsLabel->setText(resultatStr);
}
