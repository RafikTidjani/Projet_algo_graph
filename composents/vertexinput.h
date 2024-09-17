#ifndef VERTEXINPUT_H
#define VERTEXINPUT_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QBoxLayout>
#include "graph/graph.h"

class vertexInput : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructeur de la classe vertexInput
     * @param parent Le widget parent
     */
    explicit vertexInput(QWidget *parent = nullptr);
    /**
     * @brief Destructeur de la classe vertexInput
     */
    ~vertexInput();

private slots:
    /**
     * @brief Slot appelé lorsque l'utilisateur clique sur le bouton "Valider"
     */
    void onValidate();
    /**
     * @brief Slot appelé lorsque le nombre de sommet change
     */
    void onNbChange();

signals:
    /**
     * @brief Signal émis lorsque le graphe a été construit à partir des tableaux FS et APS
     * @param g Le graphe construit
     */
    void graphe(const graphalgo::graph& g);

private:
    /**
     * @brief Layout contenant les widgets pour la saisie des successeurs
     */
    QVBoxLayout* d_layoutSuccessors;
    /**
     * @brief Champ de saisie pour le nombre de sommets
     */
    QLineEdit* d_editNbVertex;
    /**
     * @brief Tableau contenant les champs de saisie des successeurs de sommets
     */
    QVector<QLineEdit*> d_linesSuccessors;
    /**
     * @brief Tableau contenant les labels des sommets
     */
    QVector<QLabel*> d_labelsSuccessors;
    /**
     * @brief Méthode privée pour créer l'interface graphique
     */
    void createInterface();
    /**
     * @brief Méthode privée permettant de créer les lignes de saisie
     * @param nbLines - Le nombre de ligne à créer
     */
    void createLines(int nbLines);
    /**
     * @brief Méthode privée pour valider la saisie
     * @param fs Le vecteur qui contiendra les valeurs du tableau FS
     * @param aps Le vecteur qui contiendra les valeurs du tableau APS
     * @return true si la saisie est valide, false sinon
     */
    bool validationInput(std::vector<int>& fs, std::vector<int>& aps);

};

#endif // VERTEXINPUT_H
