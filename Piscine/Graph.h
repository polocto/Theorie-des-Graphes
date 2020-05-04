#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "biblio.h"
#include "svgfile.h"
class Sommet;
class Arete;

class Graph
{
public:
    Graph(std::ifstream&ifs);
    Graph(Graph* Gmodel,std::string changement);
    ~Graph();
    /**Chargement des poids des aretes*/
    void chargementPonderation(std::string nomfichier);
    /**affichages des infos liées au graph*/
    void affichage_poly()const;
    void affichage(Svgfile& svgout)const;
    void affichageconsole()const;
    void affichage_suppression();

    /**Saisie*/
    Sommet* saisie()const;
    /**Calcule des différents indice*/
    void calc_indice_total();
    void calcule_indices();
    void calc_icd();//indice de centralité de degré
    void calc_vect_propre();//vecteur propre
    void Brand();//intermediatirité
    void calc_icp();//indice de proximité
    void calc_ici_naif();//intermediatirité naïve
    std::map<const Sommet*,std::pair<const Sommet*,double>> Dijkstra(Sommet* depart)const;
    /**Test la k-arete-connexité d'un graphe*/
    void connexite()const;
    int k_connexe()const;
    //k_co
    double k_ko()const;
    /**Sauvegarde des différents indices du graphe*/
    void sauvegarde(std::ofstream&fichier)const;
///vulnerabilité
    Graph* Supression_element();//je sais pas si ça va là plutot dans la classe menu
    void comparaison_graph(Graph* ancien);

    void parcours()const;
    double recherche_de_flot(const Sommet* s,const Sommet* p , const bool& connexe)const;
    void flot_entre_deux_point()const;
    void chemin_le_plus_court()const;
private:
    std::vector<Sommet*> m_sommets;//tableau de tous les sommet du graph
    std::vector<Arete*> m_aretes;// tableau de toutes les aretes du graph
    bool m_oriente;
    double m_indice;
    double m_coeff_aff;
};

#endif
