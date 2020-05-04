#ifndef ARETE_H_INCLUDED
#define ARETE_H_INCLUDED

#include "biblio.h"
#include "svgfile.h"
#include "myComparator.h"
class Sommet;

class Arete
{
public:
    /**Construction des arete*/
    Arete(Sommet* s1, Sommet* s2,bool oriente);//constructeur arete
    //Arete(Sommet* s1, Sommet* s2,double poids);//copie arete
    Arete(const Arete* copie,const std::map<const Sommet*,Sommet*>&traducteur,bool oriente);
    virtual  ~Arete() = default;
    void ponderation(std::stringstream& ifs);//chargement des poid de l'arete

    /**Affichage*/
    void affichage(Svgfile& svgout,const bool&oriente,const double&coeff_t)const;//affichage svg
    void affichageconsole()const;//affichage console
    /**Getter*/
    double get_vp(Sommet*precedent)const;//reccupere l'indice de vecteur propre du sommet à l'autre extrémité "suivant"
    double get_poid()const;//reccupere le poids de l'arete
    Sommet* getsuivant(Sommet* Som)const;//reccup le sommet suivant
    Sommet* getext1();//reccup sommet ext1
    Sommet* getext2();//recup sommet ext2

    bool verrif(const std::string&ext1,const std::string&ext2,const bool&oriente)const;
    /**Indice intermediarite*/
    void Brand(const Sommet*precednent,std::map<const Sommet*,double>&distance,const double&d_a,std::priority_queue<std::pair<const Sommet*,std::pair<const Sommet*,double>>,std::vector<std::pair<const Sommet*,std::pair<const Sommet*,double>>>,myComparator>&q,std::map<const Sommet*,double>&sigma,std::map<const Sommet*,std::list<const Sommet*>>&predecesseur)const;
    /**k-arete connexité*/
    void k_connexe(int& nombre_chemin,std::map<const Arete*,bool>& arete,std::map<const Sommet*,bool>&sommet,const Sommet*arrive)const;

    void flot(std::map<const Sommet*,std::pair<std::pair<const Sommet*,const Arete*>, std::pair<bool, double>>>&carte, std::list<const Sommet*>&file,std::map<const Arete*,double> &flot, const bool& connexe)const;
    void flot_reccursif(const Sommet*suivant,std::map<const Sommet*,std::pair<std::pair<const Sommet*,const Arete*>, std::pair<bool, double>>>&carte, std::map<const Arete*,double> &flot,double&n_max)const;
private:
    Sommet* m_ext1,* m_ext2;//extemit� des Arete
    double m_poids;
};

#endif
