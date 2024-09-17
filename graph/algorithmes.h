#ifndef PROJETGRAPHE_ALGORITHMES_H
#define PROJETGRAPHE_ALGORITHMES_H

#include "graph.h"
#include <vector>

using std::vector, std::pair;

namespace graphalgo{
    /// Constante représentant le poids maximal
    const int MAXPOIDS = __INT_MAX__;

    /**
     * @brief Fonction permettant d'empiler un sommet dans une pile chainée
     * @param x - Le sommet à empiler
     * @param pilch - La pile chainée
     */
    void empiler (int x, vector<int>& pilch);
    /**
     * @brief Fonction permettant de dépiler un sommet dans une pile chainée
     * @param pilch - La pile chainée
     * @return La valeur dépilée
     */
    int depiler(vector<int>& pilch);
    /**
     * @brief Fusionne deux sommets dans une structure de données.
     * @param s Le premier sommet.
     * @param t Le deuxième sommet.
     * @param prem Tableau des premiers sommets de chaque rang.
     * @param pilch Tableau de pile chainée.
     * @param cfc Tableau des composantes fortement connexes.
     */
    void fusion(int s, int t, vector<int>& prem, vector<int>& pilch, vector<int>& cfc);

    /**
     * @brief Fonction retournant le tableau de distances
     * @param sommet Le sommet de départ
     * @param fs
     * @param aps
     * @return Le tableau résultat contenant les distances
     */
    vector<int> distance(int sommet, const vector<int>& fs, const vector<int>& aps);
    /**
     * @brief Fonction retournant le tableau des rangs des sommets d'un graphe orienté.
     * @param fs Le tableau des successeurs de chaque sommet.
     * @param aps Le tableau des adresses des premiers successeurs de chaque sommet.
     * @return Le tableau des rangs des sommets.
     */
    vector<int> rang(const vector<int>& fs, const vector<int>& aps);
    /**
     * @brief Applique l'algorithme de Dijkstra pour calculer le plus court chemin.
     * @param s Le sommet source à partir duquel calculer les plus courts chemins.
     * @param g Le graphe.
     * @return Un graphe contenant les plus courts chemins.
     */
    graphalgo::graph dijkstra(int s, graphalgo::graph &g);
    /**
     * @brief Fonction appliquant l'algorithme de dantzig.
     * @param matriceAdj La matrice
     */
    bool dantzig(vector<vector<int>>& c);
    /**
     * @brief Applique l'algorithme de Kruskal pour trouver l'arbre couvrant de poids minimal.
     * @param G Le graphe.
     * @return Un graphe contenant l'arbre couvrant de poids minimal.
     */
    graphalgo::graph kruskal(const graphalgo::graph& G);
    /**
     * @brief Fonction retournant le codage de Prufer d'un arbre donné par sa matrice d'adjacence.
     * @param a La matrice d'adjacence représentant l'arbre.
     * @return Le codage de Prufer de l'arbre.
     */
    vector<int> prufer(vector<vector<int>> a);
    /**
     * @brief Effectue une traversée dans un graphe pour trouver les composantes fortement connexes.
     * @param s Le sommet de départ.
     * @param k Variable utilisée pour l'algorithme.
     * @param p Variable utilisée pour l'algorithme.
     * @param fs Le tableau des successeurs de chaque sommet.
     * @param aps Le tableau des adresses des premiers successeurs de chaque sommet.
     * @param prem Tableau des premiers sommets de chaque rang.
     * @param pilch Tableau de pile chainée.
     * @param cfc Tableau des composantes fortement connexes.
     * @param pred Tableau des prédécesseurs.
     * @param tarj Tableau utilisé pour tarjan.
     * @param entarj Tableau utilisé pour l'algorithme.
     * @param num Tableau utilisé pour la nouvelle numérotation.
     * @param ro Tableau utilisé pour stocker le ro de chaque sommet..
     */
    void traversee(int s, int &k, int &p, const std::vector<int> &fs, const std::vector<int> &aps, std::vector<int>& prem, std::vector<int>& pilch, std::vector<int>& cfc, std::vector<int>& pred, std::vector<int>& tarj, std::vector<bool>& entarj, std::vector<int>& num, std::vector<int>& ro);
    /**
     * @brief Applique l'algorithme de recherche de composantes fortement connexes.
     * @param fs Le tableau des successeurs de chaque sommet.
     * @param aps Le tableau des adresses des premiers successeurs de chaque sommet.
     * @param prem Tableau des premiers sommets de chaque rang.
     * @param pilch Tableau de pile chainée.
     * @param cfc Tableau des composantes fortement connexes.
     * @param pred Tableau des prédécesseurs.
     */
    void fortconnexe(const std::vector<int>& fs, const std::vector<int>& aps, std::vector<int>& prem, std::vector<int>& pilch, std::vector<int>& cfc, std::vector<int>& pred);
    /**
     * @brief Construit un graphe réduit à partir d'un graphe orienté et de ses composantes fortement connexes.
     * @param prem Tableau des premiers sommets de chaque rang.
     * @param pilch Tableau de pile chainée.
     * @param cfc Tableau des composantes fortement connexes.
     * @param fs Le tableau des successeurs de chaque sommet.
     * @param aps Le tableau des adresses des premiers successeurs de chaque sommet.
     * @return Le graphe réduit.
     */
    graphalgo::graph graph_reduit(const std::vector<int>& prem, const std::vector<int>& pilch, const std::vector<int>& cfc, const std::vector<int>& fs, const std::vector<int>& aps);
    /**
     * @brief Calcule les bases du graphe réduit.
     * @param apsr Vecteur des débuts de liste de successeurs du graphe réduit.
     * @param fsr Vecteur des listes de successeurs du graphe réduit.
     * @param br Vecteur qui contiendra les bases du graphe réduit après le calcul.
     */
    void base_Greduit(const vector<int>& apsr, const vector<int>& fsr, vector<int>& br);
    /**
     * @brief Permet l'affichage des bases
     */
    std::string afficher(const std::vector<int>& base);
    /**
     * @brief Réalise l'édition des bases du graphe initial.
     * @param prem Vecteur des sommets de chaque base dans le graphe initial.
     * @param pilch Pile chainée
     * @param br Vecteur contenant les bases du graphe réduit.
     * @return Les bases
     */
    std::string edition_bases(const vector<int>& prem, const vector<int>& pilch, const vector<int>& br);
    /**
     * @brief Calcule les dates au plus tôt pour les tâches.
     * @param taches Les tâches à traiter.
     */
    void calculerDateTot(std::vector<Tache>& taches);
    /**
     * @brief Obtient les successeurs d'une tâche donnée.
     * @param taches Les tâches.
     * @param index L'index de la tâche dont on veut obtenir les successeurs.
     * @return Un vecteur contenant les index des successeurs.
     */
    std::vector<int> getSuccesseurs(const std::vector<Tache>& taches, int index);
    /**
     * @brief Calcule les dates au plus tard pour les tâches.
     * @param taches Les tâches à traiter.
     */
    void calculerDateTard(std::vector<Tache>& taches);
    /**
     * @brief Trouve les chemins critiques dans le diagramme de Gantt.
     * @param taches Les tâches à traiter.
     * @return Un vecteur contenant les tâches qui sont critiques.
     */
    std::vector<Tache> cheminsCritiques(std::vector<Tache>& taches);
    /**
     * @brief Convertit les tableaux FP APP en FS APS.
     * @param fp Tableau des précédences.
     * @param app Tableau des adresses des premiers précédents de chaque sommet.
     * @param fs Tableau des successeurs.
     * @param aps Tableau des adresses des premiers successeurs de chaque sommet.
     */
    void FPAPPtoFSAPS(const vector<int>& fp, const vector<int>& app, vector<int>& fs, vector<int>& aps);
    /**
     * @brief Convertit les tableaux FS APS en FP APP..
     * @param fs Tableau des flux.
     * @param aps Tableau des adresses des premiers successeurs de chaque sommet.
     * @param fp Tableau des précédences.
     * @param app Tableau des adresses des premiers précédents de chaque sommet.
     */
    void FSAPStoFPAPP(const vector<int>& fs, const vector<int>& aps, vector<int>& fp, vector<int>& app);
    /**
     * @brief Calcule la longueur critique d'un projet.
     * @param file_pred Le tableau des précédences.
     * @param adr_prem_pred Le tableau des adresses des premiers précédents de chaque sommet.
     * @param duree_taches Le tableau des durées des tâches.
     * @param file_pred_critique Le tableau des précédences des tâches critiques.
     * @param adr_prem_pred_critique Le tableau des adresses des premiers précédents des tâches critiques.
     * @param longueur_critique Le tableau des durées des tâches critiques.
     */
    void longueurCritique(const vector<int> file_pred, const vector<int> adr_prem_pred, const vector<int> duree_taches, vector<int>& file_pred_critique, vector<int>& adr_prem_pred_critique, vector<int>& longueur_critique);
    /**
     * @brief Effectue l'ordonnancement des tâches.
     * @param fs Tableau FS.
     * @param aps Tableau des adresses des premiers successeurs de chaque sommet.
     * @param duree_taches Le tableau des durées des tâches.
     * @param new_fs Le nouveau FS.
     * @param new_aps Le nouveau tableau des adresses des premiers successeurs de chaque sommet.
     * @return Un vecteur contenant les longueurs critiques.
     */
    vector<int> ordonnancement(const vector<int>& fs, const vector<int>& aps, const vector<int>& duree_taches, vector<int>& new_fs, vector<int>& new_aps);

}

#endif //PROJETGRAPHE_ALGORITHMES_H
