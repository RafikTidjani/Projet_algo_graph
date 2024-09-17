#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <utility>
using std::vector, std::string;
#include <iostream>

namespace graphalgo
{
    /**
     * @brief Structure représentant une arête dans le graphe.
     */
    struct vtx
    {
        /// Le sommet de départ de l'arête
        int s;
        /// Le sommet de destination de l'arête
        int t;
        /// Le poids ou le coût de l'arête
        int p;
    };

    /**
     * @brief Compare deux structures vtx en fonction de leur poids.
     * @param a Première structure vtx.
     * @param b Deuxième structure vtx.
     * @return true si le poids de a est inférieur à celui de b, sinon false.
     */
    bool compare_vtx(const vtx &a, const vtx &b);

    /**
     * @brief Structure représentant une tâche dans le contexte de l'ordonnancement.
     */
    struct Tache {
        /// Le nom de la tâche.
        string nom;
        /// La durée nécessaire pour compléter la tâche.
        int duree;
        /// Les indices des tâches qui la précèdent.
        std::vector<int> predecesseurs;
        /// Les dates au plus tôt et au plus tard d'exécution.
        int dateTot, dateTard;

        /**
         * @brief Constructeur de la structure Tache.
         * @param nom Le nom de la tâche.
         * @param duree La durée nécessaire pour compléter la tâche.
         * @param predecesseurs Les indices des tâches précédentes.
         */
        Tache(const string& nom, int duree, const std::vector<int>& predecesseurs)
            : nom(nom), duree(duree), predecesseurs(predecesseurs)
        {}
    };

    class node
    {
        public:
            /**
             * @brief Constructor for the node class.
             * @param n The value of the node or the cost.
             */
            node(int n);

            /**
             * @brief Getter method for retrieving the value of the node or the cost.
             * @return The value of the node.
             */
            int n() const;
            /**
             * @brief Getter method for retrieving the pointer to the next node in the main list.
             * This can represent either a consecutive successor (1 -> 2) or its successor in the graph (1 <---> 3).
             * @return A reference to the next node in the main list.
             */
            node& next_m() const;
            /**
             * @brief Getter method for retrieving the pointer to the next node in the secondary list.
             * This can be either a cost or the next cost of the current cost.
             * @return A reference to the next node in the secondary list.
             */
            node& next_s() const;
        private:
            /**
             * @brief d_n: The value of the node or the cost
             */
            int d_n;
            /**
             * @brief d_next_m: The pointer to a node in the main list.
             * This is either a consecutive successor (1 -> 2)
             * or its successor in the graph (1 <---> 3).
             */
            node* d_next_m;
            /**
             * @brief d_next_s: The pointer to a node in the secondary list.
             * It's either a cost or the next cost of the current cost.
             */
            node* d_next_s;

        friend class graph;
    };

    /**
     * @brief The graph class a dynamique structur of graph node
     */
    class graph
    {
        public:
            /**
             * @brief graph default constructor
             */
            graph();
            /**
             * @brief construct a list of n node
             * @param n: the number of node to create
             * @param oriented: the graph is oriented or not
             * @pre n > 1
             */
            graph(unsigned int n, bool oriented = true);
            /**
             * @brief graph create a graph using fs & aps;
             * @param fs
             * @param aps
             */
            graph(const vector<int>& fs, const vector<int>& aps);
            /**
             * @brief create a graph using an adjacency matrix;
             * @param mat_adj: the adjacency matrix
             */
            graph(const vector<vector<int>>& mat_adj, bool oriented = true);
            /**
             * @brief graph copie constructor
             */
            graph(const graph& g);
            /**
             * @brief destructor;
             */
            ~graph();

            // Getteurs
            /**
             * @brief Méthode publique retournant le contenu de d_n
             * @return
             */
            int n() const;

            // methods

            /**
             * @brief return the orientation of a graph
             * @return the orientation
             */
            bool oriented() const;
            /**
             * @brief set an orientation to a graph
             * @param oriented -  the orientation
             */
            void oriented(bool oriented);
            /**
             * @brief Méthode publique retournant si le graph possède des coûts
             * @return
             */
            bool graphaveccout() const;

            /**
             * @brief find a node
             * @param s: the node to find
             * @return the pointer to the node
             * @pre s > 1
             */
            node* find(int s) const;
            /**
             * @brief add_node; add a node to the graph
             */
            void add_node();
            /**
             * @brief add a successor to a node
             * @param s: the node itself
             * @param ss: the node that should be connected to the node s
             * @param cost: the cost of node
             * @pre cost in ]-INT_MAX, INT_MAX[
             * @pre s > 0
             * @pre ss > 0
             */
            void add_successor(int s, int ss, int cost = __INT_MAX__);
            /**
             * @brief checks if there is a vertex between two node of a not oriented graph
             * @param s
             * @param ss
             * @param ct: a pointer to the cost of the vertex
             * @return the vertex head if there is, nullptr if there isn't
             */
            node* vertex(int s, int ss, node*& ct) const;
            /**
             * @brief checks if there is a vertex between two node of a oriented graph
             * @param s
             * @param ss
             * @return the cost of the vertex if there is, nullptr if there isn't
             */
            node* vertex(int s, int ss) const;
            /**
             * @brief get the cost of a vertex
             * @param s
             * @param ss
             * @return INT_MAX if there a cost was not set
             */
            int cost(int s, int ss) const;
            /**
             * @brief set the cost of a vertex
             * @param s
             * @param ss
             * @param cost
             */
            void cost(int s, int ss, int cost);
            /**
             * @brief transform the graph to a queue of successors
             * @param fs
             * @param aps
             */
            void fs_aps(vector<int>& fs, vector<int>& aps);
            /**
             * @brief retrun the adjacency matrix of the graph
             * @retrun vector<vector<int>>
             */
            vector<vector<int>> mat_adj();

            /**
             * @brief get all the vertex of a graph
             * @return a vector of vt
             */
            vector<vtx> vertexes() const;

            /**
             * @brief Returns the adjacency matrix of cost of the graph.
             * @return A 2D vector representing the adjacency matrix of cost.
             */
            vector<vector<int>> cost_matrice() const;
            /**
             * @brief Saves the graph to an output stream.
             * @param ost The output stream to which the graph will be saved.
             */
            void save(std::ostream& ost) const;
            /**
             * @brief Loads the graph from an input stream.
             * @param ist The input stream from which the graph will be loaded.
             */
            void load(std::istream& ist);
            /**
             * @brief Assigns the contents of another graph to this graph.
             * @param g The graph to copy from.
             * @return A reference to this graph after assignment.
             */
            graph& operator=(const graph &g);
            /**
             * @brief Checks if this graph is equal to another graph.
             * @param g The graph to compare with.
             * @return True if the graphs are equal, false otherwise.
             */
            bool operator==(const graph& g);

        private:
            /// nb of node
            int d_n;
            /// the graph is oriented or not
            bool d_oriented;
            /// entry point if the list
            node* d_tete;

            /// @brief create an adjacency matrix for a not oriented graph
            void mat_adj_not_oriented(vector<vector<int>>& mat_adj);
            /// @brief create an adjacency matrix for an oriented graph
            void mat_adj_oriented(vector<vector<int>>& mat_adj);
    };
}

#endif // GRAPH_H
