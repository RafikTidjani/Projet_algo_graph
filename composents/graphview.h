#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include "../graph/graph.h"
#include <vector>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPoint>

class graphNode{
public :
    /**
     * @brief Constructeur d'un noeud du graphe
     * @param index Son index
     * @param coordonnees Son point, avec ses coordonnées
     */
    graphNode(int index, QPoint coordonnees);

    /**
     * @brief Méthode publique retournant l'indice du Noeud
     * @return
     */
    int indice() const;
    /**
     * @brief Méthode publique retournant les coordonnées du Noeud
     * @return
     */
    QPoint coordonnees() const;

private :
    /**
     * @brief L'indice du Noeud
     */
    int d_index;
    /**
     * @brief Les coordonnées du Noeud
     */
    QPoint d_coordonnee;
};

/**
 * @brief QWidget affaichant la représentation graphique d'un graphalgo::graph
 */
class graphView : public QWidget
{
    Q_OBJECT
public:
    explicit graphView(graphalgo::graph &g, QWidget *parent = nullptr);
    ~graphView();

    /**
     * @brief Redéfinition de la méthode sizeHint - On redimensionne le QWidget par défaut
     * @return
     */
    QSize sizeHint() const override;

private :
    /**
     * @brief La liste des Noeuds du graph
     */
    std::vector<graphNode> d_listeNoeuds;
    /**
     * @brief Les tableaux FS et APS du graph à dessiner
     */
    // vector<int>fs, aps;
    graphalgo::graph d_graph;

    /**
     * @brief Méthode privée retournant le Noeud d'indice saisi
     * @param index L'indice en question
     * @return
     */
    graphNode find(int index) const;

private :
    /**
     * @brief Méthode dessinant le composant
     */
    void paintEvent(QPaintEvent *) override;

    /**
     * @brief Méthode privée traçant un noeud
     * @param p Les coordonnées du Noeud
     * @param index Le numéro du Noeud
     */
    void dessineNoeud(QPainter &painter, const graphNode &node);
    /**
     * @brief dessineArete Méthode privée reliant deux noeuds dans l'interface
     * @param noeud1 Le premier Noeud
     * @param noeud2 Le second
     * @param oriented Booléen indiquant si le graph est orienté
     */
    void dessineArete(QPainter &painter, const graphNode &node1, const graphNode &node2,
                      int cout = __INT_MAX__, bool oriented = false);

    /**
     * @brief Méthode calculant les coordonnées des Noeuds du graph à l'aide du tableau de rang de ces denriers
     * @param rang Le tableau des rangs
     * @return
     */
    std::vector<graphNode> calculePositions(std::vector<graphalgo::vtx> vertex);

    /**
     * @brief Méthode générant la représentation graphique d'un graph
     * @param g Le graph à illustrer
     */
    void dessineGraph(QPainter &painter);

public slots :
    /**
     * @brief Méthode privée réagissant à la modification du graph courant de l'application
     * @param g Le graph courant
     */
    void graphChanged(graphalgo::graph &g);
};

#endif // GRAPHVIEW_H
