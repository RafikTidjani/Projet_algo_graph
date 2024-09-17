#include "vertexinput.h"
#include <QPushButton>
#include <QBoxLayout>
#include <QMessageBox>

vertexInput::vertexInput(QWidget *parent)
    : QWidget(parent)
{
    createInterface();
}

vertexInput::~vertexInput() {

}

void vertexInput::createInterface()
{
    // Création du layout principal
    auto mainLayout {new QVBoxLayout{this}};

    // Titre de la fenêtre
    auto title = new QLabel("Saisie d’un graph selon une liste de sommets : ");
    mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine {new QFrame{}};
    separationLine->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    mainLayout->addWidget(separationLine);

    // Nombre de sommets
    auto layoutNbVertex {new QHBoxLayout()};
    auto labelNbVertex {new QLabel{"Nombre de sommets :"}};
    d_editNbVertex = new QLineEdit{};
    connect(d_editNbVertex, &QLineEdit::textChanged, this, &vertexInput::onNbChange);
    layoutNbVertex->addWidget(labelNbVertex);
    layoutNbVertex->addWidget(d_editNbVertex);
    mainLayout->addLayout(layoutNbVertex);

    // Création du layout pour les successeurs et son ajout au layout principal
    d_layoutSuccessors = new QVBoxLayout{};
    mainLayout->addLayout(d_layoutSuccessors);

    // Ajout d'un espacement vertical
    mainLayout->addSpacing(10);

    // On ajoute le bouton Valider
    auto validateButton = new QPushButton("Valider");
    auto buttonLayout = new QHBoxLayout{};
    buttonLayout->addStretch();
    buttonLayout->addWidget(validateButton);
    mainLayout->addLayout(buttonLayout);
    // On le connecte
    connect(validateButton, &QPushButton::clicked, this, &vertexInput::onValidate);
}

void vertexInput::createLines(int nbLines) {
    // Supprimer les lignes de saisie existantes
    for(QLineEdit *edit : d_linesSuccessors) {
        d_layoutSuccessors->removeWidget(edit);
        delete edit;
    }
    for(QLabel *label : d_labelsSuccessors) {
        d_layoutSuccessors->removeWidget(label);
        delete label;
    }
    d_linesSuccessors.clear();
    d_labelsSuccessors.clear();

    // Créer de nouvelles lignes de saisie en fonction du nombre de sommets
    for (int i = 0; i < nbLines; ++i) {
        QHBoxLayout *layoutLigne = new QHBoxLayout{};
        QLabel *labelSommet = new QLabel{QString("Sommet %1 :").arg(i + 1)};
        QLineEdit *editSuccesseurs = new QLineEdit{};
        d_labelsSuccessors.append(labelSommet);
        d_linesSuccessors.append(editSuccesseurs);
        layoutLigne->addWidget(labelSommet);
        layoutLigne->addWidget(editSuccesseurs);
        d_layoutSuccessors->addLayout(layoutLigne);
    }
}

void vertexInput::onNbChange()
{
    int nbLines = d_editNbVertex->text().toInt();
    createLines(nbLines);
}

bool vertexInput::validationInput(std::vector<int>& fs, std::vector<int>& aps) {
    for(int i = 0; i < d_linesSuccessors.size(); i++)
    {
        QStringList successeursStr = d_linesSuccessors[i]->text().split(" ", Qt::SkipEmptyParts);
        aps[i + 1] = fs.size() + 1; // Mettre à jour l'aps avec la taille actuelle de fs
        for(const QString& successeur : successeursStr)
        {
            bool ok;
            int successeurInt = successeur.toInt(&ok);
            if(!ok || successeurInt < 1 || successeurInt > d_editNbVertex->text().toInt()) {
                QMessageBox::critical(nullptr, "Erreur de saisie", QString("Le successeur '%1' du sommet %2 est invalide.").arg(successeur).arg(i+1));
                return false;
            }
            fs.push_back(successeurInt);
        }
        fs.push_back(0); // Ajouter un 0 pour indiquer la fin des successeurs de ce sommet
    }

    fs[0] = fs.size();
    aps[0] = d_editNbVertex->text().toInt();

    return true;
}

void vertexInput::onValidate()
{
    std::vector<int> fs;
    std::vector<int> aps(d_editNbVertex->text().toInt() + 1, 0);

    // Vérifier la saisie
    if (!validationInput(fs, aps)) {
        return; // Sortir de la méthode si la saisie est invalide
    }

    // Construire le graphe à partir de FS et APS
    graphalgo::graph g(fs, aps);
    emit graphe(g);
}


