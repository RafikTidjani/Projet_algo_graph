#ifndef ORDONNANCEMENT_H
#define ORDONNANCEMENT_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include "graph/graph.h"
#include "composents/graphview.h"

class ordonnancement : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe ordonnancement.
     * @param parent Le widget parent.
     */
    explicit ordonnancement(QWidget *parent = nullptr);
    ~ordonnancement();

private:
    /// Titre de la fenêtre.
    QLabel* d_title;
    /// Layout pour le nombre de tâches.
    QHBoxLayout* d_layoutNbTaches;
    /// Champ de saisie pour le nombre de tâches.
    QLineEdit* d_editNbTaches;

    /// Layout pour le graphique.
    QHBoxLayout* d_layoutGraph;

    /// Layout pour les tâches.
    QVBoxLayout* d_layoutTaches;
    /// Vecteur de champs de saisie pour les noms des tâches.
    QVector<QLineEdit*> d_lignesNom;
    /// Vecteur de champs de saisie pour les durées des tâches.
    QVector<QLineEdit*> d_lignesDuree;
    /// Vecteur de champs de saisie pour les prédécesseurs des tâches.
    QVector<QLineEdit*> d_lignesPredecesseurs;
    /// Vecteur de labels pour les tâches.
    QVector<QLabel*> d_labelsTaches;

    /// Label pour afficher les résultats.
    QLabel* d_resultsLabel;

    /// Bouton de validation.
    QPushButton* d_validateButton;

    /// Vue du graphique.
    graphView* graph;

    /**
     * @brief Crée l'interface utilisateur.
     */
    void createInterface();

    /**
     * @brief Crée des lignes pour saisir les informations des tâches.
     * @param nbLines Le nombre de lignes à créer.
     */
    void createLines(int nbLines);

    /**
     * @brief Vérifie la validité de la saisie des tâches.
     * @param taches Vecteur de tâches à remplir avec les données saisies.
     * @return True si la saisie est valide, False sinon.
     */
    bool validationInput(std::vector<graphalgo::Tache>& taches);

    /**
     * @brief Construit le graphe à partir des tâches saisies.
     * @param taches Vecteur de tâches.
     * @return Le graphe construit à partir des tâches.
     */
    graphalgo::graph buildGraph(const std::vector<graphalgo::Tache>& taches);

    /**
     * @brief Efface les lignes de saisie des tâches.
     */
    void effacerLignes();

    /**
     * @brief Efface l'affichage.
     */
    void effaceAffichage();

    /**
     * @brief Met à jour l'interface avec les résultats d'ordonnancement.
     * @param taches Vecteur de tâches.
     * @param g Le graphe associé aux tâches.
     * @param longueurCritique Vecteur des longueurs critiques.
     * @param cheminsCritique Vecteur des tâches constituant les chemins critiques.
     */
    void updateResults(const std::vector<graphalgo::Tache>& taches, graphalgo::graph &g, const std::vector<int>& longueurCritique, const std::vector<graphalgo::Tache>& cheminsCritique);

private slots:
    /**
     * @brief Slot appelé lorsque le nombre de tâches est modifié.
     */
    void onNbChange();

    /**
     * @brief Slot appelé lors de la validation des saisies.
     */
    void onValidate();

};

#endif // ORDONNANCEMENT_H
