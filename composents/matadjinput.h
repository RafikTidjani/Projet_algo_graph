#ifndef MATADJINPUT_H
#define MATADJINPUT_H

#include "../graph/graph.h"
#include <vector>
#include <QObject>
#include <QDialog>

class QTableWidget;
class QLabel;
class QLineEdit;
class QCheckBox;
class QPushButton;
class QBoxLayout;
class coutInput;

class matAdjInput : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructeur de la classe matAdjInput
     * @param parent Le widget parent
     */
    explicit matAdjInput(QWidget *parent = nullptr);
    ~matAdjInput();

private :
    QBoxLayout *d_mainLayout, *d_preSaisieLayout, *d_valideLayout, *d_horizonthalLayout, *d_coutLayout;
    /**
     * @brief Attributs privés contenant les textes de l'affichage
     */
    QLabel *d_lNbSommets, *d_lOrientation, *d_labelCout;
    /**
     * @brief Attributs privés contenant la saisie du nombre de sommets
     */
    QLineEdit *d_inputnbSommets;
    /**
     * @brief Attribut privé indiquant si le graph est orienté ou non
     */
    QCheckBox *d_radioButtonOriente ,*d_checkCouts;
    /**
     * @brief Attribut privé contenant la matrice à remplir
     */
    QTableWidget *d_matrice;
    /**
     * @brief Attribut privé contenant le bouton de validation
     */
    QPushButton *d_bValide;
    coutInput *d_coutinput;

    /**
     * @brief Méthode privée générant les composants de l'interface
     */
    void createInterface();
    /**
     * @brief Méthode privée mettant à jour l'affichage pour permettre la saisie des coûts
     */
    void updateInterface(const vector<vector<int>> &matAdj);

    /**
     * @brief Méthode privée générant les case de la matrice
     * @param i Le nombre de sommets compris dans la matrice
     */
    void createMatrice(unsigned i);
    /**
     * @brief méthode privée vidant la matrice
     */
    void deleteMatrice();

    /**
     * @brief matrticeValide
     * @return TRUE - la matrice est valide | FALSE - la matrice n'est pas valide
     */
    bool recupereValide(std::vector<std::vector<int>> &matAdj) const;

    /**
     * @brief Méthode privé générant le graph
     */
    void createGraph();

private slots :
    /**
     * @brief Méthode privée générant la matrice d'adjacence à compléter
     */
    void onNbSommets(unsigned i);
    /**
     * @brief Méthode privée réagissant au signal graphe d'un coutInput et renvoygant le graph généré à la MainWinodw
     * @param g Le graph généré
     */
    void onGraphReceived(graphalgo::graph &g);

signals:
    /**
     * @brief Signal privé émettant le nouveau graph
     * @param g Le graph saisi
     */
    void graphe(const graphalgo::graph &g);
};

#endif // MATADJINPUT_H
