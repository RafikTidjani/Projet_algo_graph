#include "graph.h"
#include <algorithm>
#include <string>
#include <sstream>

namespace graphalgo
{
    bool compare_vtx(const vtx &a, const vtx &b)
    {
        // Si les valeurs de p sont égales, comparer s
        if (a.p == b.p)
            return a.s < b.s;
        // Sinon, comparer les valeurs de p
        else
            return a.p < b.p;
    }

    node::node(int n):d_n{n}, d_next_m{nullptr}, d_next_s{nullptr}
    {}

    int node::n() const { return d_n; }
    node& node::next_m() const { return *d_next_m; }
    node& node::next_s() const { return *d_next_s; }

    graph::graph(): d_n{0}, d_oriented{true}, d_tete{nullptr}
    {}

    graph::graph(unsigned int n, bool oriented): d_n{static_cast<int>(n)}, d_oriented{oriented}
    {
        if(d_n > 0)
        {
            d_tete = new node(1);
            node *crt = d_tete;
            for(int i = 2; i <= d_n; ++i)
            {
                crt->d_next_m = new node(i);
                crt = crt->d_next_m;
            }
        }
    }

    graph::graph(const vector<int>& fs, const vector<int>& aps): d_n{aps[0]}, d_oriented{true}
    {
        d_tete = new node(1);
        node *crt = d_tete;
        for(int i = 2; i <= d_n; ++i)
        {
            crt->d_next_m = new node(i);
            crt = crt->d_next_m;
        }

        int s = 1;
        for(int i = 1; i <= fs[0]; ++i)
        {
            node* ct;
            if(fs[i] != 0)
                add_successor(s, fs[i]);
            else
                ++s;
        }
    }

    graph::graph(const vector<vector<int>>& mat_adj, bool oriented): d_n{mat_adj[0][0]}, d_oriented{oriented}
    {
        d_tete = new node(1);
        node *crt = d_tete;
        for(int i = 2; i <= d_n; ++i)
        {
            crt->d_next_m = new node(i);
            crt = crt->d_next_m;
        }

        for(int i = 1; i <= d_n; ++i)
        {
            for(int j = 1; j <= d_n; ++j)
            {
                node* ct;
                if(mat_adj[i][j] == 1 && (!vertex(i, j, ct) || d_oriented))
                    add_successor(i, j);
            }
        }
    }

    graph::graph(const graph& g): d_n{g.d_n}, d_oriented{g.d_oriented}
    {
        if(d_n > 0)
        {
            d_tete = new node(1);
            node *crt = d_tete;
            for(int i = 2; i <= d_n; ++i)
            {
                crt->d_next_m = new node(i);
                crt = crt->d_next_m;
            }

            crt = g.d_tete;
            while(crt)
            {

                node* crt_ct = crt->d_next_s;
                while(crt_ct)
                {
                    add_successor(crt->d_n, crt_ct->d_next_m->d_n, crt_ct->d_n);
                    crt_ct = crt_ct->d_next_s;
                }
                crt = crt->d_next_m;
            }
        }
    }

    graph::~graph()
    {
        node* crt = d_tete;
        node* as;

        while(crt)
        {
            node* crt_ct = crt->d_next_s;
            // detroying all the cost of the current node
            while(crt_ct)
            {
                as = crt_ct;
                crt_ct = crt_ct->d_next_s;
                delete as;
            }

            // delete the node itself and move to the next node;
            as = crt;
            crt = crt->d_next_m;
            delete as;
        }
    }

    int graph::n() const { return d_n; }

    bool graph::oriented() const { return d_oriented; }

    void graph::oriented(bool oriented) { d_oriented = oriented; }

    bool graph::graphaveccout() const
    {
        vector<vector<int>> matCout = this->cost_matrice();

        bool res = false;
        unsigned i = 0;
        while(!res && i < matCout.size())
        {
            unsigned j = 0;
            while(!res && j < matCout[i].size())
            {
                if(matCout[i][j] != 0 && matCout[i][j] != 1)
                    res = true;
            }
        }

        return res;
    }

    node* graph::find(int s) const
    {
        if(!d_tete)
            return nullptr;

        node* crt = d_tete;
        while(crt && crt->d_n != s)
            crt = crt->d_next_m;

        return crt;
    }

    void graph::add_node()
    {
        if(!d_tete)
            d_tete = new node(++d_n);
        else {
            node* crt = d_tete;

            while(crt->d_next_m != nullptr)
                crt = crt->d_next_m;

            crt->d_next_m = new node(++d_n);
        }
    }

    void graph::add_successor(int s, int ss, int cost)
    {
        node* ns = find(s);
        node* nss = find(ss);

        // return if s or ss could not be found break execution
        if(!ns || !nss)
            return;

        // create the node with the cost @cost and connect it to @ss
        node* new_c = new node(cost);
        new_c->d_next_m = nss;

        // if the list of cost of the node is null then add the top else add to the bottom
        if(!ns->d_next_s)
            ns->d_next_s = new_c;
        else
        {
            node* crt_c = ns->d_next_s;
            while(crt_c->d_next_s && crt_c->d_next_m != nss)
                crt_c = crt_c->d_next_s;
            
            if(crt_c->d_next_m == nss)
            {
                delete new_c;
                crt_c->d_n = cost;
            }
            else
                crt_c->d_next_s = new_c;
        }
    }

    node* graph::vertex(int s, int ss, node*& ct) const
    {
        node* ns = find(s);
        node* nss = find(ss);

        if(!ns || !nss)
            return nullptr;

        node* crt_vertex_1 = ns->d_next_s;
        node* crt_vertex_2 = nss->d_next_s;

        // d_n in crt_ct is the cost and d_next_m->d_n is the successor
        while((crt_vertex_1 && crt_vertex_2) &&
            (crt_vertex_1->d_next_m->d_n != ss && crt_vertex_2->d_next_m->d_n != s))
        {
            crt_vertex_1 = crt_vertex_1->d_next_s;
            crt_vertex_2 = crt_vertex_2->d_next_s;
        }

        if(crt_vertex_1 && crt_vertex_2)
        {
            if (crt_vertex_1->d_next_m->d_n == ss)
            {
                ct = crt_vertex_1;
                return ns;
            }
            else
            {
                ct = crt_vertex_2;
                return nss;
            }
        }
        else{
            if(crt_vertex_1)
            {
                while(crt_vertex_1 && crt_vertex_1->d_next_m->d_n != ss)
                    crt_vertex_1 = crt_vertex_1->d_next_s;

                if(crt_vertex_1 && crt_vertex_1->d_next_m->d_n == ss)
                {
                    ct = crt_vertex_1;
                    return ns;
                }
            }
            else
            {
                while(crt_vertex_2 && crt_vertex_2->d_next_m->d_n != s)
                    crt_vertex_2 = crt_vertex_2->d_next_s;

                if(crt_vertex_2 && crt_vertex_2->d_next_m->d_n == s)
                {
                    ct = crt_vertex_2;
                    return nss;
                }
            }
        }
        return nullptr;
    }

    node* graph::vertex(int s, int ss) const
    {
        node* ns = find(s);

        if(!ns)
            return nullptr;

        node* crt_ct = ns->d_next_s;
        while(crt_ct && crt_ct->d_next_m->d_n != ss)
            crt_ct = crt_ct->d_next_s;
        // at this point crt_ct would be nullptr or equal to the cost of the vertex
        return crt_ct;
    }

    int graph::cost(int s, int ss) const
    {
        node* crt_ct = nullptr;

        if(d_oriented)
            crt_ct = vertex(s, ss);
        else
            vertex(s, ss, crt_ct);

        if(crt_ct != nullptr)
            return crt_ct->d_n;
        else
            return -__INT_MAX__;
    }

    void graph::cost(int s, int ss, int cost)
    {
        node* crt_ct = nullptr;

        if(d_oriented)
            crt_ct = vertex(s, ss);
        else
            vertex(s, ss, crt_ct);

        if(crt_ct != nullptr)
            crt_ct->d_n = cost;
    }

    void graph::fs_aps(vector<int> &fs, vector<int> &aps)
    {
        if(!d_tete)
            return;

        //putting a default size in fs[0]
        fs.clear();
        fs.push_back(-1);
        aps.resize(d_n + 1);
        aps[0] = d_n;
        //current node id;
        int i = 1;

        node *crt = d_tete;
        while(crt)
        {
            // add the fs index of the current node
            aps[i] = fs.size();

            node* crt_ct = crt->d_next_s;
            // going to all the successors of the current node
            while(crt_ct)
            {
                // d_n in crt_ct is the cost and d_next_m->d_n is the successor
                fs.push_back(crt_ct->d_next_m->d_n);
                crt_ct = crt_ct->d_next_s;
            }
            fs.push_back(0);
            crt = crt->d_next_m;
            ++i;
        }
        fs[0] = fs.size() - 1;
    }

    void graph::mat_adj_not_oriented(vector<vector<int>>& mat_adj)
    {
        // nombre of arrete
        int m = 0;
        //current node id;
        int i = 1;

        node *crt = d_tete;
        while(crt)
        {
            node* crt_ct = crt->d_next_s;
            // going to all the successors of the current node
            while(crt_ct)
            {
                // d_n in crt_ct is the cost and d_next_m->d_n is the successor
                mat_adj[i][crt_ct->d_next_m->d_n] = 1;
                mat_adj[crt_ct->d_next_m->d_n][i] = 1;
                crt_ct = crt_ct->d_next_s;
                ++m;
            }
            crt = crt->d_next_m;
            ++i;
        }
        mat_adj[0][1] = m;
    }

    void graph::mat_adj_oriented(vector<vector<int>>& mat_adj)
    {
        // nombre of arrete
        int m = 0;
        //current node id;
        int i = 1;

        node *crt = d_tete;
        while(crt)
        {
            node* crt_ct = crt->d_next_s;
            // going to all the successors of the current node
            while(crt_ct)
            {
                // d_n in crt_ct is the cost and d_next_m->d_n is the successor
                mat_adj[i][crt_ct->d_next_m->d_n] = 1;
                crt_ct = crt_ct->d_next_s;
                ++m;
            }
            crt = crt->d_next_m;
            ++i;
        }
        mat_adj[0][1] = m;
    }

    vector<vector<int>> graph::mat_adj()
    {
        if(!d_tete)
            return {};

        vector<vector<int>> adj_mat(d_n + 1, vector<int>(d_n + 1, 0));

        adj_mat[0][0] = d_n;

        if(d_oriented)
            mat_adj_oriented(adj_mat);
        else
            mat_adj_not_oriented(adj_mat);

        return adj_mat;
    }

    vector<vtx> graph::vertexes() const
    {
        vector<vtx> vtxs;
        vtxs.reserve(d_n * d_n);

        node *crt = d_tete;
        while(crt)
        {
            node* crt_ct = crt->d_next_s;
            // going to all the successors of the current node and get the cost
            while(crt_ct)
            {
                // crt_ct->d_n is the cost and crt_ct->d_next_m->d_n is the successor of crt->d_n
                vtx vt;
                vt.s = crt->d_n;
                vt.t = crt_ct->d_next_m->d_n;
                vt.p = crt_ct->d_n;

                vtxs.push_back(vt);
                crt_ct = crt_ct->d_next_s;
            }
            crt = crt->d_next_m;
        }

        sort(vtxs.begin(), vtxs.end(), compare_vtx);
        return vtxs;
    }

    vector<vector<int>> graph::cost_matrice() const
    {
        vector<vector<int>> ct_mat(d_n, vector<int>(d_n, __INT_MAX__));

        node *crt = d_tete;
        while(crt)
        {
            int s = crt->d_n;
            node* crt_ct = crt->d_next_s;
            // going to all the successors of the current node and get the cost
            while(crt_ct)
            {
                // crt_ct->d_n is the cost and crt_ct->d_next_m->d_n is the successor of crt->d_n
                int t = crt_ct->d_next_m->d_n;
                ct_mat[s-1][t-1] = crt_ct->d_n;

                crt_ct = crt_ct->d_next_s;
            }
            crt = crt->d_next_m;
        }

        return ct_mat;
    }

    void graph::save(std::ostream& ost) const
    {
        ost << d_n << " " << d_oriented << std::endl;
        
        node* crt = d_tete;

        while(crt)
        {

            node* crt_ct = crt->d_next_s;
            while(crt_ct)
            {
                ost << crt->d_n << " " << crt_ct->d_next_m->d_n << "-" << crt_ct->d_n;
                crt_ct = crt_ct->d_next_s;
                ost << std::endl;
            }
            crt = crt->d_next_m;
        }
    }

    void graph::load(std::istream& ist)
    {
        ist >> d_n >> d_oriented;
        if(d_n > 0)
        {
            d_tete = new node(1);
            node *crt = d_tete;
            for(int i = 2; i <= d_n; ++i)
            {
                crt->d_next_m = new node(i);
                crt = crt->d_next_m;
            }

            std::string line;
            while ( std::getline(ist, line ) ) {
                std::istringstream li( line);
                int s, ss, cost;
                char c;
                
                li >> s >> ss >> c >> cost;
                add_successor(s, ss, cost);
            }
        }
    }

    graph& graph::operator=(const graph &g)
    {
        if (this != &g) { // Check for self-assignment
            // Create a copy of the graph 'g'
            graph new_graph{g};
            d_n = g.d_n;
            d_oriented = g.d_oriented;

            // Swap the contents of 'this' and 'new_graph'
            std::swap(d_tete, new_graph.d_tete);

            // No need to delete 'tmp' as it's just a pointer swap
        }
        return *this;
    }

    bool graph::operator==(const graph& g) {
        // Vérifier si les deux graphes ont le même nombre de nœuds
        if (this->n() != g.n()) {
            return false;
        }

        // Vérifier si les deux graphes ont la même orientation
        if (this->oriented() != g.oriented()) {
            return false;
        }

        // Parcourir tous les nœuds du premier graphe
        node* n1 = this->d_tete;
        node* n2 = g.d_tete;
        while (n1 != nullptr && n2 != nullptr) {
            // Vérifier si les valeurs des nœuds sont les mêmes
            if (n1->d_n != n2->d_n) {
                return false;
            }

            // Parcourir tous les successeurs du nœud courant
            node* s1 = n1->d_next_s;
            node* s2 = n2->d_next_s;
            while (s1 != nullptr && s2 != nullptr) {
                // Vérifier si les successeurs ont les mêmes valeurs
                if (s1->d_next_m->d_n != s2->d_next_m->d_n || s1->d_n != s2->d_n) {
                    return false;
                }
                s1 = s1->d_next_s;
                s2 = s2->d_next_s;
            }

            // Vérifier si les deux listes de successeurs ont la même longueur
            if (s1 != nullptr || s2 != nullptr) {
                return false;
            }

            n1 = n1->d_next_m;
            n2 = n2->d_next_m;
        }

        // Vérifier si les deux listes de nœuds ont la même longueur
        if (n1 != nullptr || n2 != nullptr) {
            return false;
        }

        return true;
    }
}
