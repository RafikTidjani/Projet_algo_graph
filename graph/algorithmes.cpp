#include "algorithmes.h"
#include "graph.h"
#include <sstream>

void graphalgo::empiler(int x, vector<int>& pilch) {
    pilch[x] = pilch[0];
    pilch[0] = x;
}

int graphalgo::depiler(vector<int>& pilch) {
    int x = pilch[0];
    pilch[0] = pilch[x];
    return x;
}

vector<int> graphalgo::distance(int sommet, const vector<int>& fs, const vector<int>& aps){
    // On déclare une variable contenant le nombre de sommets du graph
    int nbSommets = aps[0];
    // Déclaration des variables de début et fin du bloc en cours de traitement
    int debutBloc = 0, finBloc = 1;
    int d = 0, finFile, sommetCourant, successeur, indiceSuccesseur;

    // Déclaration d'un tableau contenant la file d'attente
    vector<int>file (nbSommets + 1);
    file[0] = nbSommets;
    file[1] = sommet;

    // On alloue le tableau de distance
    vector<int>distance (nbSommets + 1, -1);
    distance[0] = nbSommets;
    distance[sommet] = 0;

    // On déroule la file d'atente
    while(debutBloc < finBloc){
        d++;
        finFile = finBloc;
        // On parcourt le bloc courant

        while(debutBloc < finFile){
            // On décale le début du bloc
            debutBloc++;

            // On récupère le prochain successeur
            sommetCourant = file[debutBloc];
            indiceSuccesseur = aps[sommetCourant];
            successeur = fs[indiceSuccesseur];

            // Tant qu'il reste des successeurs
            while(successeur > 0){
                // Si le successeur courant n'est pas encore marqué
                if(distance[successeur] == -1){
                    // On décale la fin du bloc de travail
                    finBloc++;
                    // On place dans le blco suivant et on lui affecte sa distance
                    file[finBloc] = successeur;
                    distance[successeur] = d;
                }
                successeur = fs[++indiceSuccesseur];
            }
        }
    }
    return distance;
}

vector<int> graphalgo::rang(const vector<int>& fs, const vector<int>& aps) {
    // On déclare les variables tampons
    int nbSommets = aps[0], tailleFS = fs[0], sommetCourant, r, indiceSuccesseur, successeur;
    // On initialise le tableau de rangs
    vector<int> rang (nbSommets + 1, -1);

    // On initialise les tableaux tampons
    vector<int> ddi (nbSommets + 1, 0); // Degré entrant de chaque sommet
    vector<int> pilch (nbSommets + 1); // Pile des sommets sans prédécesseurs
    vector<int> prem (nbSommets + 1); // Tableau des premiers sommets de chaque rang

    // AJOUT
    for(int i = 0; i < tailleFS; i++) {
        int sommet = fs[i];
        if(sommet > 0)
            ddi[sommet]++;
    }

    // On calcule le rang
    pilch[0] = 0;
    for(sommetCourant = 1; sommetCourant <= nbSommets; sommetCourant++)
        if(ddi[sommetCourant] == 0)
            empiler(sommetCourant, pilch);

    r = -1;
    sommetCourant = pilch[0];
    prem[0] = sommetCourant;

    while(pilch[0] > 0) {
        // On incrémente le rang courant
        r++;
        // On réinitialise la pile des sommets sans prédécesseurs
        pilch[0] = 0;

        // On parcourt les sommets sans prédécesseurs dans la pile pilch
        while(sommetCourant > 0) {
            // On affecte le rang au sommet courant
            rang[sommetCourant] = r;

            // On récupère le premier successeur du sommet courant
            indiceSuccesseur = aps[sommetCourant];
            successeur = fs[indiceSuccesseur];

            // On parcourt les successeurs du sommet courant
            while(successeur > 0) {
                // On décrémente le degré entrant du successeur
                ddi[successeur]--;
                // On empile le successeur s'il n'a plus de prédécesseurs
                if(ddi[successeur] == 0)
                    empiler(successeur,pilch);

                // On passe au successeur suivant
                indiceSuccesseur++;
                successeur = fs[indiceSuccesseur];
            }

            // On passe au sommet suivant sans prédécesseurs
            sommetCourant = pilch[sommetCourant];
        }

        // On met-à-jour le sommet de début du prochain rang
        sommetCourant = pilch[0];
        prem[r + 1] = sommetCourant;
    }
    rang[0] = aps[0];
    // On retourne le vecteur de rangs calculé
    return rang;
}

vector<int> graphalgo::prufer(vector<vector<int>> a) {
    int nbSommets = a[0][0];
    // Création du vecteur pour stocker le codage de Prufer
    vector<int> prf (nbSommets - 1);

    prf[0] = nbSommets - 2;
    int k = 1;

    while(k <= nbSommets - 2) {
        // On cherche le premier sommet feuille
        int i = 1;
        for(; a[i][0] != 1; i++);

        // On cherche le voisin du sommet feuille
        int j=1;
        for(; a[i][j] != 1; j++);

        // On stocke du voisin dans le codage de Prufer
        prf[k++] = j;

        // On supprime le sommet feuille et ses arêtes
        a[i][j] = 0;
        // On supprime  l'arête entre le sommet feuille et son voisin
        a[j][i] = 0;
        a[i][0] = 0; // On marque le sommet feuille comme supprimé
        a[j][0]--;
    }
    return prf;
}

inline vector<int> retireIndex(const std::vector<int> &tab, int index)
{
    index++;
    vector<int> res;
    for(unsigned i = 0; i < tab.size(); i++){
        if(tab[i] != index)
            res.push_back(tab[i]);
    }
    return res;
}

graphalgo::graph graphalgo::dijkstra(int s_depart, graphalgo::graph &g)
{
    const int nb_sommets = g.n();

    // On récupère fs-aps et la matrice des coûts
    std::vector<int> fs, aps;
    g.fs_aps(fs, aps);
    std::vector<vector<int>> matCout = g.cost_matrice();

    // On déclare et initialise les variables tampons
    vector<int> d(nb_sommets, graphalgo::MAXPOIDS), pred(nb_sommets, -1);
    d[s_depart - 1] = 0;
    pred[s_depart - 1] = 0;

    // On déclare un tableau indiquant les sommets non-parcourus
    vector<int> sommets_non_traites(nb_sommets);
    for(int i = 0; i < nb_sommets; i++)
        sommets_non_traites[i] = i + 1;


    // On traite le sommet dont la distance est la plus petite
    for(int i = 0; i < nb_sommets; i++){
        // On déclare des variables tampons pour chercher le sommet de travail (plus courte distance)
        int s_courant = graphalgo::MAXPOIDS, distSommet = graphalgo::MAXPOIDS;
        bool inacessible = true;

        // On cherche le sommet de travail
        for(unsigned j = 0; j < sommets_non_traites.size(); j++){
            int s = sommets_non_traites[j] - 1;
            if(d[s] < distSommet){
                s_courant = s;
                distSommet = d[s];
                // Il reste au moins un sommet accessible
                inacessible = false;
            }
        }

        // Je vérifie que les sommets restant sont accessibles
        if(!inacessible){
            // On traite les successeurs du sommet de travail
            int indice = aps[s_courant + 1];
            while(fs[indice] != 0){
                // On récupère les sonnées
                int sommet_successeur = fs[indice] - 1;
                int c_sommet = d[s_courant];
                int c_chemin = matCout[s_courant][sommet_successeur];

                // On test si le nouveau chemin est plus court
                if(c_sommet + c_chemin < d[sommet_successeur]){
                    // On implémente
                    d[sommet_successeur] = c_sommet + c_chemin;
                    pred[sommet_successeur] = s_courant + 1;
                }
                indice++;
            }
        }

        // sommets_non_traites.erase(sommets_non_traites.begin() + s_courant);
        sommets_non_traites = retireIndex(sommets_non_traites, s_courant);
    }


    // On déclare le graph réduit
    graphalgo::graph newG = graphalgo::graph(nb_sommets, g.oriented());
    // On remplit newG
    for(int i = 0; static_cast<unsigned>(i) < pred.size(); i++){
        if(pred[i] != i + 1)
            newG.add_successor(pred[i], i + 1, d[i]);
    }
    // On retourne le graph réduit
    return newG;
}

bool graphalgo::dantzig(vector<vector<int>>& c)
{
    // Initialisation
    int n = c.size();
    int k, i, j;
    double x;

    for(k = 0; k < n - 1; k++)
    {
        for(i = 0; i < k; i++)
        {
            for(j = 0; j < k; j++)
            {
                if((x = c[i][j] + c[j][k]) < c[i][k])
                    c[i][k] = x;

                if((x = c[k][j] + c[j][i]) < c[k][i])
                    c[k+1][i] = x;
            }
            if(c[i][k] + c[k][i] < 0)
            {
                // cout<< "Circuit absorbant passant par " << i << " et " << k+1 << "." << endl;
                return false;
            }
        }
        for(i = 0; i < k; i++)
            for(j = 0; j < k; j++)
            {
                if((x = c[i][k] + c[k][j]) < c[i][j])
                    c[i][j] = x;
            }
    }
    return true;
}

void graphalgo::fusion(int s, int t, vector<int>& prem, vector<int>& pilch, vector<int>& cfc) {
    int cs = cfc[s];
    int ct = cfc[t];
    int x = prem[ct];
    while(x != 0) {
        cfc[x] = cs;
        x = pilch[x];
    }
    x = prem[cs];
    while(pilch[x] != 0) {
        x = pilch[x];
    }
    pilch[x] = prem[ct];
}

graphalgo::graph graphalgo::kruskal(const graphalgo::graph& G) {
    int nbSommets = G.n();
    // Initialiser T avec le même nombre de noeuds et orientation que G
    graphalgo::graph T = graphalgo::graph(nbSommets, G.oriented());

    // On initialise les tableaux tampons
    vector<int> prem(nbSommets + 1);
    vector<int> pilch(nbSommets + 1);
    vector<int> cfc(nbSommets + 1);

    // Initialisation des tableaux tampons
    for (int i = 1; i <= nbSommets; i++) {
        prem[i] = i;
        pilch[i] = 0;
        cfc[i] = i;
    }
    pilch[0] = 0;

    // Stockage des arêtes avec leur poids
    vector<graphalgo::vtx> aretes = G.vertexes();

    int k = 0;
    for(const auto& a : aretes) {
        // On récupère les extrémités de l'arête
        int s = a.s;
        int t = a.t;
        if(cfc[s] != cfc[t]) {
            // Ajouter le successeur si on n'a pas de circuit (même CFC)
            T.add_successor(s, t, a.p);
            fusion(s, t, prem, pilch, cfc);
            k++;
            // On s'arrête quand l'arbre couvrant est complet
            if(k == nbSommets - 1) break;
        }
    }

    return T;
}



void graphalgo::traversee(int s, int &k, int &p, const std::vector<int> &fs, const std::vector<int> &aps, std::vector<int>& prem, std::vector<int>& pilch, std::vector<int>& cfc, std::vector<int>& pred, std::vector<int>& tarj, std::vector<bool>& entarj, std::vector<int>& num, std::vector<int>& ro)
{
    int t, u;
    // numérote s et initialise ro[s]
    p++;
    num[s] = p;
    ro[s] = p;
    empiler(s, tarj);
    entarj[s] = true;
    for (int k = aps[s]; (t=fs[k]) != 0 ; k++) {
        if (num[t] == 0) { // si t n'est pas encore numéroté
            pred[t] = s;
            traversee(t, k, p, fs, aps, prem, pilch, cfc, pred, tarj, entarj, num, ro);
            if (ro[t] < ro[s]) ro[s] = ro[t]; // Z1
        } else {
            if ((num[t] < ro[s]) && entarj[t]) ro[s] = num[t]; // Z2
        }
    }

    if (ro[s] == num[s]) {
        k++;
        do {
            u = depiler(tarj);
            entarj[u] = false;
            empiler(u, pilch);
            cfc[u] = k;
        } while (u != s);
        prem[k] = pilch[0];
        pilch[0] = 0;
    }
}

void graphalgo::fortconnexe(const std::vector<int>& fs, const std::vector<int>& aps, std::vector<int>& prem, std::vector<int>& pilch, std::vector<int>& cfc, std::vector<int>& pred)
{
    int n = aps[0];
    prem.resize(n + 1);
    pilch.resize(n + 1);
    cfc.resize(n + 1);
    pred.resize(n + 1, 0);

    std::vector<int> tarj(n + 1);
    std::vector<bool> entarj(n + 1, false);
    std::vector<int> num(n + 1, 0);
    std::vector<int> ro(n + 1, 0);

    int p = 0;
    int k = 0;

    pilch[0] = 0;
    tarj[0] = 0;

    for (int s = 1; s <= n; s++)
        if (num[s] == 0)
            traversee(s, k, p, fs, aps, prem, pilch, cfc, pred, tarj, entarj, num, ro);

    prem[0] = k;
}

graphalgo::graph graphalgo::graph_reduit(const std::vector<int>& prem, const std::vector<int>& pilch, const std::vector<int>& cfc, const std::vector<int>& fs, const std::vector<int>& aps) {
    // Initialisation des variables
    int s, kr = 1, k, t, nbc = prem[0];
    // Vecteur pour vérifier si un sommet a déjà été ajouté
    std::vector<bool> deja_mis(nbc + 1);
    // Vecteurs pour stocker les nouveaux FS ET APS
    vector<int> fsr(fs[0] + 1);
    vector<int> apsr(nbc + 1);
    apsr[0] = nbc;

    // Parcours des composantes fortement connexes
    for (int i = 1; i <= nbc; i++) {
        // Initialisation du nouvel indice pour le prochain successeur
        apsr[i] = kr;
        // Réinitialisation du vecteur de vérification
        for(int i = 1; i <= nbc; i++)
            deja_mis[i]=false;
        // Marquage de la composante fortement connexe actuelle
        deja_mis[i] = true;
        // Récupération du premier sommet de la composante fortement connexe
        s = prem[i];
        // Parcours des successeurs du sommet actuel
        while (s != 0) {
            for (int k = aps[s]; (t = fs[k]) != 0; k++) {
                // Si la composante fortement connexe du successeur n'a pas encore été ajoutée
                if (deja_mis[cfc[t]] == false) {
                    // Ajout de la composante fortement connexe comme nouveau successeur
                    fsr[kr] = cfc[t];
                    kr++;
                    // Marquage de la composante fortement connexe comme déjà ajoutée
                    deja_mis[cfc[t]] = true;
                }
            }
            // Passage au sommet suivant dans la composante fortement connexe
            s = pilch[s];
        }
        // Ajout d'un marqueur de fin de liste de successeurs pour la composante fortement connexe actuelle
        fsr[kr] = 0;
        kr++;
    }
    // Mise à jour du nombre total de successeurs
    fsr[0] = kr - 1;

    // Création du graphe réduit à partir des nouveaux FS et APS
    graphalgo::graph gr{fsr, apsr};
    return gr;
}

void graphalgo::base_Greduit(const vector<int>& apsr, const vector<int>& fsr, vector<int>& br)
{
    int nr = apsr[0];
    br.resize(nr + 1);
    vector<int> ddir(nr + 1);

    for(int i = 0; i <= nr; i++)
        ddir[i] = 0;

    for(int kr = 1; kr <= fsr[kr]; kr++)
        ddir[fsr[kr]]++;

    int nb = 0;
    for(int c = 1; c <= nr; c++)
        if (ddir[c] == 0)
            br[++nb] = c;

    br[0] = nb;
}

std::string graphalgo::afficher(const std::vector<int>& base)
{
    std::stringstream ss;
    ss << "Bases : ";

    for (unsigned i = 0; i < base.size(); i++)
        ss << base[i] << " ";

    ss << "\n";
    return ss.str();
}

std::string graphalgo::edition_bases(const vector<int>& prem, const vector<int>& pilch, const vector<int>& br)
{
    int nb = br[0]; // Nombre de CFC de l’unique base du graphe réduit
    vector<int> Base(nb + 1); // pile qui mémorise les sommets des bases du graphe initial
    Base[0] = nb;
    int p = 1;
    int som = prem[br[1]]; // premier sommet de la première CFC

    std::stringstream ss;
    while (p >= 1) {
        if ((p <= nb) && (som != 0)) {
            Base[p] = som;
            p++;
            if (p <= nb)
                som = prem[br[p]];
            else
                ss << afficher(Base); // Affiche le contenu du tableau Base contenant les sommets d’une base du graphe initial
        } else {
            p--;
            som = pilch[Base[p]];
        }
    }
    return ss.str();
}

void graphalgo::calculerDateTot(std::vector<Tache>& taches) {
    // Initialisation de la date au plus tôt à 0 pour la première tâche
    taches[0].dateTot = 0;

    // Calcul des dates au plus tôt pour les autres tâches
    for (unsigned i = 1; i < taches.size(); i++) {
        int maxPredDate = 0;
        for (int pred : taches[i].predecesseurs) {
            maxPredDate = std::max(maxPredDate, taches[pred - 1].dateTot);
        }
        taches[i].dateTot = maxPredDate + taches[i].duree;
    }
}

std::vector<int> graphalgo::getSuccesseurs(const std::vector<Tache>& taches, int index) {
    // Vecteur pour stocker les successeurs de la tâche à l'index donné
    std::vector<int> successeurs;

    // Parcours de toutes les tâches
    for (int i = 0; i < taches.size(); i++) {
        // Parcours des prédécesseurs de la tâche à l'index i
        for (int pred : taches[i].predecesseurs) {
            // Si un prédécesseur correspond à l'index donné, la tâche à l'index i est un successeur
            if (pred == index + 1) {
                // Ajout de l'index de la tâche en tant que successeur
                successeurs.push_back(i);
                // Arrêt de la recherche des prédécesseurs pour cette tâche
                break;
            }
        }
    }

    return successeurs;
}

void graphalgo::calculerDateTard(std::vector<Tache>& taches) {
    // Initialisation de la date au plus tard à la date au plus tôt de la dernière tâche
    taches[taches.size() - 1].dateTard = taches[taches.size() - 1].dateTot;

    // Calcul des dates au plus tard pour les autres tâches
    for (int i = taches.size() - 2; i >= 0; i--) {
        int minSuccDate = taches[taches.size() - 1].dateTard;
        for (int succ : getSuccesseurs(taches, i + 1)) {
            minSuccDate = std::min(minSuccDate, taches[succ].dateTard);
        }
        taches[i].dateTard = minSuccDate - taches[i].duree;
    }
}

std::vector<graphalgo::Tache> graphalgo::cheminsCritiques(std::vector<Tache>& taches) {
    std::vector<Tache> cheminsCritiques;

    for (Tache& tache : taches) {
        if (tache.dateTot == tache.dateTard) {
            cheminsCritiques.push_back(tache);
        }
    }

    return cheminsCritiques;
}

void graphalgo::FPAPPtoFSAPS(const vector<int>& fp, const vector<int>& app, vector<int>& fs, vector<int>& aps) {
    int n = app[0];

    aps.clear();
    fs.clear();

    aps.reserve(n + 1);
    aps.push_back(n);

    fs.reserve(fp[0] + 1);
    fs.push_back(fp[0]);

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= fp[0]; ++j) {
            if (fp[j] == i) {
                unsigned k = 1;
                while (k < app.size() and app[k] <= j) {
                    ++k;
                }
                fs.push_back(k - 1);
            }
        }
        fs.push_back(0);
    }

    aps.push_back(1);
    for (unsigned i = 1; i < fs.size() - 1; ++i)
        if (fs[i] == 0) aps.push_back(i + 1);
}

void graphalgo::FSAPStoFPAPP(const vector<int>& fs, const vector<int>& aps, vector<int>& fp, vector<int>& app) {
    int n = aps[0] ;
    app.push_back(n);
    fp.push_back(fs[0]);
    for(int i = 1 ; i <= n ; ++i)
    {
        app.push_back(fp.size());
        for(int j = 1 ; j <= fs[0] ; ++j)
        {
            if(fs[j] == i)
            {
                int k = 1;
                while(aps[k] <= j)
                {
                    k++;
                }
                fp.push_back(k-1);
            }
        }
        fp.push_back(0);
    }
}

void graphalgo::longueurCritique(const vector<int> file_pred, const vector<int> adr_prem_pred, const vector<int> duree_taches, vector<int>& file_pred_critique, vector<int>& adr_prem_pred_critique, vector<int>& longueur_critique) {
    int n = adr_prem_pred[0];
    int m = file_pred[0];

    file_pred_critique.resize(m+1);
    adr_prem_pred_critique.resize(n+1);
    adr_prem_pred_critique[0] = n;

    longueur_critique.resize(n+1);
    longueur_critique[0] = n;

    int kc = 1; // Indice de la dernière place remplie dans fpc
    int t, lg; // La longueur lg de la tâche t
    longueur_critique[1] = 0;
    file_pred_critique[1] = 0; //Fin de la liste
    adr_prem_pred_critique[1] = 1;

    for(int s = 2 ; s <= n ; s++)
    {
        // Calcul de lc[s] en fonction des prédecesseurs critiques de s
        longueur_critique[s] = 0;
        adr_prem_pred_critique[s] = kc+1; // Début de la liste des prédecesseurs critiques de s
        for(int k = adr_prem_pred[s] ; (t = file_pred[k]) != 0 ; ++k)
        {
            lg = longueur_critique[t] + duree_taches[t];
            if(lg >= longueur_critique[s])
            {
                if(lg > longueur_critique[s])
                {
                    longueur_critique[s] = lg; // Nouvelle lg candidate à être critique
                    kc = adr_prem_pred_critique[s] ;
                    file_pred_critique[kc] = t;
                }
                else //lg == lc[s]
                {
                    kc++;
                    file_pred_critique[kc] = t;
                }
            }
        }
        kc++;
        file_pred_critique[kc] = 0; // Fin de la liste des prédecesseurs critiques de s
    }
    file_pred_critique[0] = kc;
}

vector<int> graphalgo::ordonnancement(const vector<int>& fs, const vector<int>& aps, const vector<int>& duree_taches, vector<int>& new_fs, vector<int>& new_aps) {
    // Création des vecteurs à utiliser
    vector<int> file_pred;
    vector<int> adr_prem_pred;
    vector<int> file_pred_critique;
    vector<int> adr_prem_pred_critique;
    vector<int> longueur_critique;

    // On convertit le fs/aps en fp/app :
    FSAPStoFPAPP(fs, aps, file_pred, adr_prem_pred);

    // On récupère les longueurs critiques
    longueurCritique(file_pred, adr_prem_pred, duree_taches, file_pred_critique, adr_prem_pred_critique, longueur_critique);

    return longueur_critique;
}
