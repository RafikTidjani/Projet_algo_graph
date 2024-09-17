#ifndef FSAPSINPUT_H
#define FSAPSINPUT_H

#include <QWidget>

#include "graph/graph.h"

class QCheckBox;
class QLineEdit;
class QLabel;
class coutInput;
class QPushButton;
class QBoxLayout;

class fsapsInput : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe fsapsInput
     * @param parent Le widget parent
     */
    explicit fsapsInput(QWidget *parent = nullptr);
    /**
     * @brief Destructeur de la classe fsapsInput
     */
    ~fsapsInput();

private:
    QBoxLayout *d_mainLayout;
    /**
     * @brief Attributs privés stockant les label du QWidget
     */
    QLabel *d_labelFS, *d_labelAPS, *d_labelCout;
    /**
     * @brief Attributs privés contenant les champs de saisie des tableaux FS et APS
     */
    QLineEdit *d_inputFS, *d_inputAPS;
    /**
     * @brief Attribut privé contenant le bouton clickable indiquant si le grave psosède des coûts
     */
    QCheckBox *d_checkCouts;
    /**
     * @brief Attribut privé contenant le QPushButton validant la saisie
     */
    QPushButton *d_validateButton;
    /**
     * @brief Attribut privé contenant le coutInput
     */
    coutInput *d_coutinput;


    /**
     * @brief Méthode privée pour créer l'interface graphique
     */
    void createInterface();
    /**
     * @brief Méthode privée mettant à jour l'affichage pour permettre la saisie des coûts
     */
    void updateInterface(const std::vector<int> &fs, const std::vector<int> &aps);

private slots:
    /**
     * @brief Méthode privée appelé lorsque l'utilisateur clique sur le bouton "Valider"
     */
    void onValidate();
    /**
     * @brief Méthode privée réagissant au signal graphe d'un coutInput et renvoygant le graph généré à la MainWinodw
     * @param g Le graph généré
     */
    void onGraphReceived(graphalgo::graph &g);

signals:
    /**
     * @brief Signal émis lorsque le graphe a été construit à partir des tableaux FS et APS
     * @param g Le graphe construit
     */
    void graphe(const graphalgo::graph& g);
};

/**
     * @brief Méthode pour valider la saisie des tableaux FS et APS
     * @param saisieFS La saisie du tableau FS
     * @param saisieAPS La saisie du tableau APS
     * @param fs Le vecteur qui contiendra les valeurs du tableau FS
     * @param aps Le vecteur qui contiendra les valeurs du tableau APS
     * @return true si la saisie est valide, false sinon
     */
bool validationFsAps(const QString& saisieFS, const QString& saisieAPS, std::vector<int>& fs, std::vector<int>& aps);

#endif // FSAPSINPUT_H
