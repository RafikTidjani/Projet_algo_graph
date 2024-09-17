#ifndef COUTINPUT_H
#define COUTINPUT_H

#include "../graph/graph.h"
#include <QWidget>

class QVBoxLayout;
class QTableWidget;

class coutInput : public QWidget
{
    Q_OBJECT

    friend class fsapsInput;
    friend class matAdjInput;

public:
    /**
     * @brief Constructeur de la class coutInput selon FS APS
     * @param fs Le tableau FS
     * @param aps Le tableau APS
     * @param parent
     */
    explicit coutInput(const vector<int> &fs, const vector<int> &aps, QWidget *parent = nullptr);
    /**
     * @brief Constructeur de la class coutInput selon une matrice d'adjacence
     * @param matAdj La matrice d'adjacence
     * @param oriented Booléen indiquant si le graph est orienté
     * @param parent
     */
    explicit coutInput(const vector<vector<int>> &matAdj, bool oriented, QWidget *parent = nullptr);
    ~coutInput();

private :
    /**
     * @brief Attributs privés représentant un graph sous forme de tableaux FS APS
     */
    vector<int> fs, aps;
    /**
     * @brief Attribut privé représentant un graph sous forme d"e tableaux FS APS'une matrice d'adjacence
     */
    vector<vector<int>> matAdj;
    /**
     * @brief Booléen indiquant si le graph est orienté
     */
    bool d_oriented;

    /**
     * @brief Attribut privé contenant le layout principal du QWidget
     */
    QVBoxLayout *d_mainLayout;
    /**
     * @brief Attribut privé contenant le QWidget de saisie des coûts
     */
    QTableWidget *d_matrice;


    /**
     * @brief Méthode privée chargée de générer les différents composants de base de l'interface
     */
    void createInterface();
    /**
     * @brief Méthode privée chargée de générer les composants de saisi à l'aide des tabelaux FS APS
     */
    void createInterfaceFSAPS();
    /**
     * @brief Méthode privée chargée de générer les composants de saisi à l'aide de la matrice d'adjacence
     */
    void createInterfaceMATADJ();

    /**
     * @brief Méthode privéee générant le graph selon les informations saisies
     */
    void createGraph();

signals:
    /**
     * @brief Signal émis lorsque le graphe a été construit à partir des tableaux FS et APS
     * @param g Le graphe construit
     */
    void graphe(graphalgo::graph& g);
};

#endif // COUTINPUT_H
