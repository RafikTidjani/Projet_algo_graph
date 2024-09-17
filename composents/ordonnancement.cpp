#include "ordonnancement.h"
#include "graph/algorithmes.h"
#include "composents/graphview.h"
#include <QPushButton>
#include <QBoxLayout>
#include <QMessageBox>
#include <QRegularExpressionValidator>



ordonnancement::ordonnancement(QWidget *parent)
    : QWidget(parent)
{
    createInterface();
}

ordonnancement::~ordonnancement()
{
    // On supprime les pointeurs
    delete d_title;
    delete d_layoutNbTaches;;
    delete d_editNbTaches;

    delete d_layoutGraph;

    delete d_layoutTaches;
    delete d_resultsLabel;
    delete d_validateButton;

    effacerLignes();
}

void ordonnancement::createInterface()
{
    // Création du layout principal
    auto mainLayout = new QVBoxLayout(this);

    // Titre de la fenêtre
    d_title = new QLabel("Saisie des tâches :");
    mainLayout->addWidget(d_title);

    // Ajout d'une ligne de séparation
    auto separationLine = new QFrame;
    separationLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);
    mainLayout->addWidget(separationLine);

    // Création du layout pour le futur graph
    d_layoutGraph = new QHBoxLayout{};
    // graphalgo::graph p {};
    // graph = new graphView{p};
    d_layoutGraph->addWidget(graph);
    mainLayout->addLayout(d_layoutGraph);

    // Nombre de tâches
    d_layoutNbTaches = new QHBoxLayout();
    auto labelNbTasks = new QLabel("Nombre de tâches :");
    QRegularExpression intRegx("[0-9]+");
    d_editNbTaches = new QLineEdit;
    d_editNbTaches->setValidator(new QRegularExpressionValidator(intRegx));
    connect(d_editNbTaches, &QLineEdit::textChanged, this, &ordonnancement::onNbChange);
    d_layoutNbTaches->addWidget(labelNbTasks);
    d_layoutNbTaches->addWidget(d_editNbTaches);
    mainLayout->addLayout(d_layoutNbTaches);

    // Création du layout pour les tâches et son ajout au layout principal
    d_layoutTaches = new QVBoxLayout;
    mainLayout->addLayout(d_layoutTaches);

    // Ajout d'un espacement vertical
    mainLayout->addSpacing(10);

    // Création du label pour afficher les résultats
    d_resultsLabel = new QLabel;
    mainLayout->addWidget(d_resultsLabel);

    // On ajoute le bouton Valider
    auto validateButton = new QPushButton("Valider");
    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(validateButton);
    mainLayout->addLayout(buttonLayout);

    // On le connecte
    connect(validateButton, &QPushButton::clicked, this, &ordonnancement::onValidate);
}

void ordonnancement::effacerLignes() {
    // On supprime les labels et edits de chaque ligne, en retirant le widget et le supprimant
    for (QLineEdit *edit : d_lignesNom) {
        d_layoutTaches->removeWidget(edit);
        delete edit;
    }
    for (QLineEdit *edit : d_lignesDuree) {
        d_layoutTaches->removeWidget(edit);
        delete edit;
    }
    for (QLineEdit *edit : d_lignesPredecesseurs) {
        d_layoutTaches->removeWidget(edit);
        delete edit;
    }
    for (QLabel *label : d_labelsTaches) {
        d_layoutTaches->removeWidget(label);
        delete label;
    }
    // On vide les tableaux
    d_lignesNom.clear();
    d_lignesDuree.clear();
    d_lignesPredecesseurs.clear();
    d_labelsTaches.clear();
}

void ordonnancement::createLines(int nbLines)
{
    // Supprimer les lignes de saisie existantes
    effacerLignes();

    // Créer de nouvelles lignes de saisie en fonction du nombre de tâches
    for (int i = 0; i < nbLines; ++i) {
        QHBoxLayout *layoutLigne = new QHBoxLayout;

        // Ajouter le label de la tâche
        QLabel *labelTache = new QLabel(QString("Tâche %1 :").arg(i + 1));
        d_labelsTaches.append(labelTache);
        layoutLigne->addWidget(labelTache);

        // Ajouter le champ de saisie du nom de la tâche
        QLineEdit *editTacheNom = new QLineEdit;
        editTacheNom->setPlaceholderText("Nom de la tâche");
        d_lignesNom.append(editTacheNom);
        layoutLigne->addWidget(editTacheNom);

        // Ajouter le champ de saisie de la durée de la tâche
        QRegularExpression intRegx("[0-9]+");
        QLineEdit *editTacheDuree = new QLineEdit;
        editTacheDuree->setValidator(new QRegularExpressionValidator(intRegx));
        editTacheDuree->setPlaceholderText("Durée de la tâche");
        d_lignesDuree.append(editTacheDuree);
        layoutLigne->addWidget(editTacheDuree);

        // Ajouter le champ de saisie des prédécesseurs de la tâche
        QLineEdit *editTachePredecesseurs = new QLineEdit;
        editTachePredecesseurs->setPlaceholderText("Prédécesseurs (séparés par des espaces)");
        d_lignesPredecesseurs.append(editTachePredecesseurs);
        layoutLigne->addWidget(editTachePredecesseurs);

        d_layoutTaches->addLayout(layoutLigne);
    }
}

void ordonnancement::onNbChange()
{
    int nbLines = d_editNbTaches->text().toInt();
    createLines(nbLines);
}

bool ordonnancement::validationInput(std::vector<graphalgo::Tache>& taches)
{
    taches.clear();
    for (int i = 0; i < d_lignesNom.size(); i++)
    {
        QString nom = d_lignesNom[i]->text();
        int duree = d_lignesDuree[i]->text().toInt();
        QStringList predecesseursStr = d_lignesPredecesseurs[i]->text().split(" ", Qt::SkipEmptyParts);
        std::vector<int> predecesseurs;
        for (const QString& predecesseur : predecesseursStr)
        {
            bool ok;
            int predecesseurInt = predecesseur.toInt(&ok);
            if (!ok || predecesseurInt < 1 || predecesseurInt > d_lignesNom.size()) {
                QMessageBox::critical(nullptr, "Erreur de saisie", QString("Le prédécesseur '%1' de la tâche %2 est invalide.").arg(predecesseur).arg(i+1));
                return false;
            }
            predecesseurs.push_back(predecesseurInt);
        }
        graphalgo::Tache t{nom.toStdString(), duree, predecesseurs};
        taches.push_back(t);
    }
    return true;
}

graphalgo::graph ordonnancement::buildGraph(const std::vector<graphalgo::Tache>& taches)
{
    // On récupère le nombre de tâches
    int n = taches.size();
    std::vector<int> fp, app;
    app.resize(n + 1);
    app[0] = n;
    fp.push_back(n);

    // On construit FP APP
    for (int i = 0; i < n; i++) {
        app[i + 1] = fp.size();
        for(int predecesseur : taches[i].predecesseurs) {
            fp.push_back(predecesseur);
        }
        fp.push_back(0);
    }

    fp[0] = fp.size() - 1;

    // On le convertit en FS APS
    std::vector<int> fs, aps;
    graphalgo::FPAPPtoFSAPS(fp, app, fs, aps);

    // On retourne le graph correspondant
    return graphalgo::graph(fs, aps);
}

void ordonnancement::effaceAffichage() {
    // On efface les lignes
    effacerLignes();

    // On retire les widgets pour gérer le nombre de tâches
    QLayoutItem* edit = d_layoutNbTaches->takeAt(0);
    delete edit->widget();
    delete edit;
    QLayoutItem* label = d_layoutNbTaches->takeAt(0);
    delete label->widget();
    delete label;
}

void ordonnancement::updateResults(const std::vector<graphalgo::Tache>& taches, graphalgo::graph &g, const std::vector<int>& longueurCritique, const std::vector<graphalgo::Tache>& cheminsCritique)
{
    // On met à jour l'affichage
    effaceAffichage();
    d_title->setText("Affichage des résultats de l'ordonnancement");

    // graph->graphChanged(g);

    // Mettre à jour l'interface avec les résultats
    QString resultsText = "Chemin critique : ";
    for (graphalgo::Tache tache : cheminsCritique) {
        resultsText += tache.nom + " -> ";
    }

    resultsText += "\n\nLongueur critique : ";
    for (unsigned i = 1; i < longueurCritique.size(); i++) {
        resultsText += QString::number(longueurCritique[i]) + " ";
    }

    resultsText += "\n\nDates au plus tôt :";
    for (unsigned i = 0; i < taches.size(); i++) {
        resultsText += "\nTâche " + QString::fromStdString(taches[i].nom) + ": " + QString::number(taches[i].dateTot);
    }

    resultsText += "\n\nDates au plus tard :";
    for (unsigned i = 0; i < taches.size(); i++) {
        resultsText += "\nTâche " + QString::fromStdString(taches[i].nom) + ": " + QString::number(taches[i].dateTard);
    }
    d_resultsLabel->setText(resultsText);
}

void ordonnancement::onValidate()
{
    std::vector<graphalgo::Tache> taches;
    if (!validationInput(taches)) {
        return; // Sortir de la méthode si la saisie est invalide
    }

    // Construire le graphe à partir des tâches
    graphalgo::graph g = buildGraph(taches);

    // Calculer les dates au plus tôt
    graphalgo::calculerDateTot(taches);
    graphalgo::calculerDateTard(taches);

    // Calculer les longueurs critiques
    vector<int> fs, aps, fsn, apsn;
    g.fs_aps(fs, aps);
    // On recupère les durées
    std::vector<int> duree_taches(g.n() + 1);
    duree_taches[0] = g.n();
    for(unsigned i = 1; i < taches.size(); i++) {
        duree_taches[i] = taches[i-1].duree;
    }
    vector<int> longueurCritique = graphalgo::ordonnancement(fs, aps, duree_taches, fsn, apsn);

    // Récupérer les chemins critiques
    std::vector<graphalgo::Tache> cheminsCritique = cheminsCritiques(taches);

    // Calculer les résultats et mettre à jour l'interface
    updateResults(taches, g, longueurCritique, cheminsCritique);

    // On cache le bouton
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    clickedButton->hide();
}
