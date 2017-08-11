#ifndef WMANAGER_H
#define WMANAGER_H

#include <QObject>
// #include <QQuickWidget>
#include <QQuickView>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QQmlEngine>
#include <QQmlContext>

#include <QDebug>
#include <QStringList>
#include <vector>

using namespace std;

class WManager : public QObject
{
    Q_OBJECT

    //.... in .h 1/5  : Rajout propriété du Context QML
    Q_PROPERTY(QString propString READ propString_r WRITE setPropString NOTIFY propStringChanged)
    Q_PROPERTY(int largeurBouton READ largeurBouton_r WRITE setLargeurBouton NOTIFY largeurBoutonChanged)

public:
    explicit WManager(QObject *parent = 0);

    // Permet d'avoir une seule instance de cette classe
    static WManager *getInstance() {
        if(nullptr == _singleton) {
            _singleton = new WManager ;
        }
        return _singleton ;
    }
//    void displayEntiers();
    void load();
    void displayInitialInformations();

    //...........................................................................
    // Suite des fonctions servant aux échanges C++ <--> QML

    // Mise à jour du modele de xxxxView du QML (du type Context.nomModele)
    void updateQML_model(QString nomModele, QStringList listeVide);
    Q_INVOKABLE void sendActionToCpp(QString, QString parametre = QString(""));
    Q_INVOKABLE void sendUrlToCpp(QString urlFolder);
    Q_INVOKABLE void sortList(QString urlFolder, QString ext);
    Q_INVOKABLE void write(QString ligne, QString outputName);

    //...........................................................................
    // Personnalisation pour ce projet

    Q_INVOKABLE void testActionQML(int nom);
    Q_INVOKABLE void quit();


    //vector<int> listeEntiers; // une variable persistante en mémoire


protected:
    std::vector<QQuickView*> quickViews;

    QString getRepertoireProjet(bool trace = false);

    QVector<QQmlContext *>          m_QMLcontexts;
    QQmlContext*                    m_qmlContext;

signals:
    void propStringChanged();               //.... in .h 2/5  : Rajout propriété du Context QML
    void largeurBoutonChanged();

public slots:
    void setPropString(const QString &a);   //.... in .h 3/5  : Rajout propriété du Context QML
    QString propString_r() const;           //.... in .h 4/5  : Rajout propriété du Context QML

    void setLargeurBouton(const int &b);
    int largeurBouton_r() const;

private :
    static WManager *_singleton ;
    void makeQMLtab(QString nomFichierQMLsansExtension);

    QString m_propString;                   //.... in .h 5/5  : Rajout propriété du Context QML
    int m_largeurBouton;
    QStringList sl;
    QStringList listeVide;
};

#endif // WMANAGER_H


