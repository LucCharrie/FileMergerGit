#include "wmanager.h"
#include <QApplication>

#include <QDir>
#include <QFileDialog>
#include <iostream>
#include <QTextStream>
#include <QString>
#include <QIODevice>

using namespace std;

WManager::WManager(QObject *parent) : QObject(parent)
{
    qDebug() << "WManager::instance construite ";

    // 1/2 Initialisation des valeurs pour le QML avant chargement du fichier
    // propriétés simples
    setPropString("Bienvenue");//.... in .cpp 1/3  : Rajout propriété du Context QML
    setLargeurBouton(140);
}

void WManager::load()
{
    QList<QString> fichiersQML;
    // Chargement d'une ou plusieurs fenêtres avec pour fichiers sources :
    fichiersQML << QString("evaluation"); // pour une source "evaluation.qml"

    for( int t=0 ; t<1 ; t++)
    {
        makeQMLtab(fichiersQML[t] );
        std::cout << "tab " << t << " ready." << std::endl;
    }
}

void WManager::makeQMLtab(QString nomFichierQMLsansExtension)
{
    QQuickView* view = new QQuickView();
    quickViews.push_back(view);

    view->setResizeMode( QQuickView::SizeViewToRootObject);
    view->setGeometry(QRect(120, 120, 350, 200));

    // Mise en mémoire des objets pour communiquer avec le QML (avant le chargement de la page)
    m_QMLcontexts << view->engine()->rootContext();

    m_qmlContext = m_QMLcontexts.at(0);
    m_qmlContext->setContextProperty("Context", this->getInstance());
    //

    // 2/2 Initialisation des valeurs pour le QML avant chargement du fichier
    // Initialisation des modèles (même vides)
    updateQML_model("lvModel", listeVide);

    QString repertoireProjet = getRepertoireProjet();

    QString fichierQML = repertoireProjet + QString("/qml/") + nomFichierQMLsansExtension + QString(".qml");
    std::cout  << "charge le fichier QML : " << fichierQML.toLatin1().constData() << std::endl;

    // Chargement du fichier QML
    view->setSource( QUrl::fromLocalFile(fichierQML) ) ;
    view->show();
}

// Permet d'afficher des informations sur les répertoires du projet en cours
// Dans le paramétrage du projet "Project", necessite que "shadow build" soit décoché
QString WManager::getRepertoireProjet(bool trace)//false
{
    QString repertoireDuFichierApplication( qApp->applicationFilePath() );
    if( trace)
        std::cout << "qApp->applicationFilePath() = "<< repertoireDuFichierApplication.toLatin1().constData() << std::endl;

    QDir fichier(repertoireDuFichierApplication);
    fichier.cdUp() ;
    fichier.cdUp() ;
    if( trace)
        std::cout << "qApp->applicationName() = "<< qApp->applicationName().toLatin1().constData() << std::endl;

    fichier.cd(qApp->applicationName());
    QString repertoireProjet  = fichier.absolutePath() ;
    if( trace)
        std::cout  << "repertoireProjet : " << repertoireProjet.toLatin1().constData() << std::endl;

    return repertoireProjet;
}

void WManager::displayInitialInformations()
{
    getRepertoireProjet(true);
}

//.............................................................
// Mise à jour du modele de xxxxView du QML (du type Context.nomModele)
void WManager::updateQML_model(QString nomModele, QStringList listeVide)
{
    m_qmlContext = m_QMLcontexts.at(0);
    m_qmlContext->setContextProperty(nomModele, QVariant::fromValue(listeVide) );
}

void WManager::testActionQML(int i)
{
    qDebug() << "WManager::testActionQML : " << i;
}

// Fonction appelée à partir du QML pour exécuter une action C++
void WManager::sendActionToCpp(QString nomAction, QString parametre/*=""*/)
{
    qDebug() << "WManager::sendActionfromQML : nomAction = " << nomAction;
    qDebug() << "WManager::sendActionfromQML : parametre = " << parametre;

    if ( nomAction == "ajouterEntier" ) // Détail d'une action appelée à partir du C++
    {
        sl << parametre;

        updateQML_model("lvModel", sl);


    }

    if ( nomAction == "supprimerEntier" ) // Détail d'une autre action appelée à partir du C++
    {
        if (sl.size() > 0)
        {
            sl.pop_back();
            updateQML_model("lvModel", sl);
        }
        else
        {
            setPropString("Il n'y a plus de valeurs à retirer");
            //            qDebug() << m_propString;
            //            updateQML_model("lvModel", sl);
        }
    }

    if (nomAction == "navigateur" )
    {
    }
}

void WManager::sendUrlToCpp(QString urlFolder)
{
    qDebug() << "WManager::sendUrlfromQML : " << urlFolder;

    ////Liste des fichiers en console
    QString url = urlFolder.remove(0, 8);
    QDir fileDir(url);
    //qDebug() << "Cleaned URL" << url;

    QFileInfoList listeFichiers =  fileDir.entryInfoList();
    QStringList listFiles;
    QStringList filtres;
    QStringList ext;

    for (int i(0); i < listeFichiers.size(); ++i)
    {
        listFiles << listeFichiers[i].fileName();
        qDebug() << listeFichiers[i].fileName();
        ext << listeFichiers[i].suffix();
        qDebug() << ext[i];

    }

    ext.removeDuplicates();
    updateQML_model("Filtres", ext);
    updateQML_model("lvModel", listFiles);

}

void WManager::sortList(QString urlFolder, QString ext)
{
    QString url = urlFolder.remove(0, 8);
    QDir fileDir(url);
    QStringList filtres;
    QString extClear = "*." + ext;
    //qDebug() << extClear;
    filtres << extClear;
    fileDir.setNameFilters(filtres);

    QFileInfoList listeFichiers =  fileDir.entryInfoList();
    QStringList listFiles;

    for (int i(0); i < listeFichiers.size(); ++i)
    {
        listFiles << listeFichiers[i].absoluteFilePath();
    }
    updateQML_model("lvModel", listFiles);
}

void WManager::write(QString ligne, QString outputName)
{
    ligne = ligne.mid(0, ligne.size() -1);//enlève le dernier caractère
    qDebug() << ligne;


    QStringList fileToMerge;
    fileToMerge << ligne.split(";");
    qDebug() << fileToMerge;

   // QString fichierLu = outputName;
    QFile writingFile(outputName);

    if (!writingFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        return;
    }

    QTextStream writingFlux(&writingFile);

    for (int i(0); i < fileToMerge.size(); ++i)
    {

        QString buffer = "";
        QFile readingFile (fileToMerge[i]);

        if (readingFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream readingFlux(&readingFile);

            while (!readingFlux.atEnd()) {
                buffer += readingFlux.readAll() + "\n";
                buffer += "/* ************ */";
                buffer += "\n";
            }
            writingFlux << buffer;
            readingFile.close();
        }
        else
        {
            qDebug() << "erreur ouverture fichier";
        }
    }
}

//................ Rajout d'une propriété du Context pour le QML .............
//.... in .cpp 2/3  : Rajout propriété du Context QML
void WManager::setPropString(const QString &a) {
    if (a != m_propString) {
        m_propString = a;
        qDebug() << m_propString;
        emit propStringChanged();
    }
}

void WManager::setLargeurBouton(const int &b) {
    if (b != m_largeurBouton) {
        m_largeurBouton = b;
        emit largeurBoutonChanged();
    }
}

//.... in .cpp 3/3  : Rajout propriété du Context QML
QString WManager::propString_r() const {
    return m_propString;
}

int WManager::largeurBouton_r() const {
    qDebug() << "largeurBouton_r";
    return m_largeurBouton;
}

//Fonction QUIT
void WManager::quit()
{
    exit(0);
}


