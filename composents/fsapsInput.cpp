#include "fsapsInput.h"
#include "coutinput.h"
#include <QStringList>
#include <QBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QCheckBox>

// PUBLIC

fsapsInput::fsapsInput(QWidget *parent)
    : QWidget(parent)
{
    createInterface();
}

fsapsInput::~fsapsInput() {
    delete d_mainLayout;
    delete d_inputFS;
    delete d_inputAPS;
    delete d_labelFS;
    delete d_labelAPS;
    delete d_labelCout;
    delete d_checkCouts;
    delete d_coutinput;
    delete d_validateButton;
}

// PRIVATE

void fsapsInput::createInterface()
{
    // Création du layout principal
    d_mainLayout = new QVBoxLayout(this);

    // Titre de la fenêtre
    auto title = new QLabel("Saisie d’un graph selon des tableaux fs-aps");
    d_mainLayout->addWidget(title);

    // Ajout d'une ligne de séparation
    auto separationLine {new QFrame{}};
    separationLine->setFrameStyle(QFrame::HLine|QFrame::Sunken);
    d_mainLayout->addWidget(separationLine);

    // Création du checkbox pour l'ajout par la suite de côuts
    auto coutLayout {new QHBoxLayout{}};
    d_mainLayout->addLayout(coutLayout);
    d_labelCout = new QLabel{"Avec coûts :"};
    coutLayout->addWidget(d_labelCout);
    d_checkCouts = new QCheckBox{};
    coutLayout->addWidget(d_checkCouts);
    coutLayout->addStretch(1);

    // On crée les inputs
    d_inputFS = new QLineEdit{};
    d_inputAPS = new QLineEdit{};
    // On crée les labels
    d_labelFS = new QLabel{"Saisissez le tableau FS :"};
    d_labelAPS = new QLabel{"Saisissez le tableau APS :"};

    // On les ajoute au layout
    d_mainLayout->addWidget(d_labelFS);
    d_mainLayout->addWidget(d_inputFS);
    d_mainLayout->addWidget(d_labelAPS);
    d_mainLayout->addWidget(d_inputAPS);

    // Ajout d'un espacement vertical
    d_mainLayout->addSpacing(10);

    // On ajoute le bouton Valider
    auto buttonLayout = new QHBoxLayout{};
    buttonLayout->addStretch();
    d_validateButton = new QPushButton("Valider");
    buttonLayout->addWidget(d_validateButton);
    d_mainLayout->addLayout(buttonLayout);
    // On le connecte
    connect(d_validateButton, &QPushButton::clicked, this, &fsapsInput::onValidate);
}

void fsapsInput::updateInterface(const std::vector<int> &fs, const std::vector<int> &aps)
{
    // On supprime les composants inutiles
    delete d_labelAPS;
    delete d_labelFS;
    delete d_inputAPS;
    delete d_inputFS;
    delete d_checkCouts;
    delete d_labelCout;
    delete d_validateButton;

    d_coutinput = new coutInput{fs, aps};
    d_mainLayout->addWidget(d_coutinput);
    d_validateButton = new QPushButton{"Valider"};
    d_mainLayout->addWidget(d_validateButton);

    connect(d_coutinput, &coutInput::graphe, this, &fsapsInput::onGraphReceived);
    connect(d_validateButton, &QPushButton::clicked, d_coutinput, &coutInput::createGraph);
}

// PRIVATE SLOTS

void fsapsInput::onValidate()
{
    std::vector<int> fs;
    std::vector<int> aps;

    // Vérifier la saisie des tableaux FS et APS
    if (!validationFsAps(d_inputFS->text(), d_inputAPS->text(), fs, aps)) {
        return; // Sortir de la méthode si la saisie est invalide
    }

    // Si le graph nécessite une saisie des coûts
    if(d_checkCouts->isChecked()){
        updateInterface(fs, aps);

    } else {
        // Construire le graphe à partir de FS et APS
        graphalgo::graph g{fs, aps};

        emit graphe(g);
    }
}

void fsapsInput::onGraphReceived(graphalgo::graph &g)
{
    emit graphe(g);
}

// FIN FSAPSINPUT

bool validationFsAps(const QString& saisieFS, const QString& saisieAPS, std::vector<int>& fs, std::vector<int>& aps) {
    // Vérifier la saisie du tableau FS
    QStringList nombresFS = saisieFS.split(" ", Qt::SkipEmptyParts);
    if (nombresFS.isEmpty()) {
        QMessageBox::critical(nullptr, "Erreur de saisie", "Le tableau FS est vide.");
        return false;
    }

    for(const QString& valeur : nombresFS) {
        bool ok;
        int nombre = valeur.toInt(&ok);
        if(!ok || nombre < 0 || nombre > INT_MAX) {
            QMessageBox::critical(nullptr, "Erreur de saisie", "Le tableau FS contient une valeur invalide.");
            return false;
        }
        fs.push_back(nombre);
    }

    // Vérifier la saisie du tableau APS
    QStringList nombresAPS = saisieAPS.split(" ", Qt::SkipEmptyParts);
    if (nombresAPS.isEmpty()) {
        QMessageBox::critical(nullptr, "Erreur de saisie", "Le tableau APS est vide.");
        return false;
    }

    bool ok;
    int premierAPS = nombresAPS[0].toInt(&ok);
    if(!ok) {
        QMessageBox::critical(nullptr, "Erreur de saisie", "Le tableau APS contient une valeur invalide.");
        return false;
    }
    aps.push_back(premierAPS);

    int dernierAPS = 0;
    for(int i = 1; i < nombresAPS.size(); i++) {
        int nombre = nombresAPS[i].toInt(&ok);
        if(!ok || nombre < 1 || nombre > INT_MAX || nombre <= dernierAPS || nombre > fs.size()) {
            QMessageBox::critical(nullptr, "Erreur de saisie", "Le tableau APS contient une valeur invalide.");
            return false;
        }
        aps.push_back(nombre);
        dernierAPS = nombre;
    }

    return true;
}
