#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

class Graph;

class Menu
{
public:
    Menu();//constucteur
    ~Menu();//destructeur
    void menu();//menu
    void affichage()const;//affichage menu
    bool choix();//choix

    void calculIndices();//calcul affichage et sauvegarde des indices
    void chargementGraph();//chargement du graph
    void chargementPonderation();//chargement du poids des aretes du graphe
    ///vulnerabilité///
    void vulnerabilite();
    void affichage_vulnerabilite()const;

private:
    Graph* m_etude;
};

#endif // MENU_H_INCLUDED
