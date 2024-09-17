#include "graphview.h"
#include <random>
#include <QLineF>
#include <QMessageBox>

// CONSTRUCTEURS

graphNode::graphNode(int index, QPoint coordonnees)
    : d_index{index}, d_coordonnee{coordonnees}
{
}

// GETTERS

int graphNode::indice() const
{
    return d_index;
}

QPoint graphNode::coordonnees() const
{
    return d_coordonnee;
}

// CONSTRUCTEURS

graphView::graphView(graphalgo::graph &g, QWidget *parent)
    : QWidget{parent}, d_graph{g}
{
}

graphView::~graphView()
{
}

// CRÉATION DE L'INTERFACE

QSize graphView::sizeHint() const
{
    return {600, 800};
}

void graphView::paintEvent(QPaintEvent *)
{
    QPainter painter{this};
    // On dessine le graphe si il contient des noeuds
    if(d_graph.n() > 0)
        dessineGraph(painter);
}

// MÉTHODE BACK-END

graphNode graphView::find(int index) const
{
    // On initialise les variables tampons
    bool estTrouve = false;
    unsigned i = 0;
    // On déroule la liste
    while(!estTrouve && i < d_listeNoeuds.size()) {
        // On compare
        if(d_listeNoeuds[i].indice() == index)
            estTrouve = true;
        // On incrémente
        else i++;
    }
    // On retourne le Noeud
    return d_listeNoeuds[i];
}

// MÉTHODES DE DESSIN

void graphView::dessineNoeud(QPainter &painter, const graphNode &node)
{
    // Dessiner le cercle en noir
    painter.setBrush(Qt::black);
    painter.drawEllipse(node.coordonnees(), 10, 10);

    // Dessiner le numéro du nœud en blanc
    painter.setPen(Qt::white);
    // On centre le numéro de l'item
    painter.drawText(node.coordonnees() - QPoint(node.indice() > 9 ? 7 : 4, -5), QString::number(node.indice()));
}

void graphView::dessineArete(QPainter &painter, const graphNode &node1,
                             const graphNode &node2, int cout, bool oriented)
{
    // Calcul des vecteurs directeurs des arêtes
    QPoint direction = node2.coordonnees() - node1.coordonnees();
    double distance = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());
    direction /= distance;

    // Calcul des points de départ et d'arrivée sur le bord des cercles
    QPoint depart = node1.coordonnees() + direction * 20;
    QPoint arrivee = node2.coordonnees() - direction * 20;

    // On trace la ligne en noir
    painter.setPen(Qt::black);
    painter.drawLine(depart, arrivee);


    // On teste si le cout est renseigné
    if(cout != __INT_MAX__)
    {
        // On calcule les coordonnées du cout et on le dessine
        QPoint milieu {(depart + arrivee) / 2};
        painter.drawText(milieu, QString::number(cout));
    }

    if(oriented)
    {
        // Calcul de la direction de la flèche
        QPoint direction = arrivee - depart;
        double angle = atan2(direction.y(), direction.x());

        // Dessin de la flèche
        painter.save();
        painter.translate(arrivee);
        painter.rotate(angle * 180.0 / M_PI);
        painter.drawLine(0, 0, -6, -6);
        painter.drawLine(0, 0, -6, 6);
        painter.restore();
    }
}

struct point{
    int x, y;
};

std::vector<graphNode> graphView::calculePositions(std::vector<graphalgo::vtx> vertex)
{
    // On récupère les dimensions de l'écran
    int largeur = width() - 40, hauteur = height() - 40;
    // On initialise notre générateur d'aléatoire
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(40, largeur);
    std::uniform_int_distribution<> distY(40, hauteur);

    // Vector résultat contenant les coordonnées des Noeuds
    std::vector<graphNode> listeNoeuds;
    // On remplit la liste
    for(int i = 0; i < d_graph.n(); i++) {
        int x = distX(gen);
        int y = distY(gen);
        graphNode node(i + 1, {x, y});

        // On vérifie que le nœud ne chevauche pas une liaison existante
        bool estValide = true;
        for (const auto& v : vertex) {
            int s = v.s - 1;
            int t = v.t - 1;
            if(listeNoeuds.size() > 0) {
                QLineF ligne(listeNoeuds[s].coordonnees(), listeNoeuds[t].coordonnees());
                if (ligne.intersects(QLineF(node.coordonnees() - QPoint(20, 20), node.coordonnees() + QPoint(20, 20)), nullptr) == QLineF::BoundedIntersection) {
                    estValide = false;
                    break;
                }
            }
        }

        if (estValide) {
            listeNoeuds.push_back(node);
        } else {
            i--; // On réessaie une nouvelle position pour ce nœud
        }
    }

    // On la retourne
    return listeNoeuds;
}

void graphView::dessineGraph(QPainter &painter)
{
    std::vector<graphalgo::vtx> vtxs = d_graph.vertexes();

    if(d_listeNoeuds.size() == 0){
        // Récupérer les positions des nœuds
        d_listeNoeuds = calculePositions(vtxs);
    }

    // On dessine les nœuds
    for(const auto item : d_listeNoeuds)
        dessineNoeud(painter, item);

    // On récupère les liaisons
    for (const auto& v : vtxs) {
        int s = v.s - 1;
        int t = v.t - 1;
        dessineArete(painter, d_listeNoeuds[s], d_listeNoeuds[t], v.p, d_graph.oriented());
    }
}

void graphView::graphChanged(graphalgo::graph &g)
{
    // On libère la mémoire allouée
    if(!(d_graph == g)){
        d_listeNoeuds.clear();
        d_graph = g;
    }
    // On redessine le QWidget
    update();
}
