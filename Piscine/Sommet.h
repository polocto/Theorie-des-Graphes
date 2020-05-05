#ifndef SOMMET_H_INCLUDED
#define SOMMET_H_INCLUDED

#include "biblio.h"

class Svgfile;
class myComparator;
class Arete;

class Sommet
{
public:
    Sommet(const std::string&nom, const double& pos_x, const double& pos_y);//constructeur
    Sommet(const Sommet* copie);
    virtual ~Sommet() = default;//destructeur par default ne fait rien
    /**Ajout d'une arete au sommet*/
    virtual void ajout(Arete*suivant);//ajout d'une arete au sommet
    void ajoutP(Arete*precedent);

    /**AFFICHAGE*/
    void affichageconsole()const;//affichage en console
    void affichage(Svgfile& svgout,const double&coeff)const;//affichage svg
    void affichage_comparaison(Sommet* ancien)const;
    /**Getter*/
    std::string getnom()const;//reccupere le nom du sommet
    double getX()const;//reccupere la position x du sommet
    double getY()const;//reccupere la position y du sommet
    double get_vp()const;//Reccupere l'indice de vecteur propre
    double get_cp()const;

    /**CALCULE INDICES*/
    ///indice de centralité de dergré
    void calc_icd(const size_t&ordre);
    ///indice de vecteur propre
    void calc_vp(std::map<Sommet*,double>&somme);
    void indice_vp(std::map<Sommet*,double>&somme,const double& lambda);
    ///indice de proximité
    void calc_icp(double distance,double total);
    ///intermediarite///
    ///naïf
    void calc_ici_naif(double total,double a);
    ///Brand
    void Brand(const std::map<const Sommet*,double>&Cb,const double&n,const double &Cb_max);
    void Brand(std::map<const Sommet*,double>&distance,std::priority_queue<std::pair<const Sommet*,std::pair<const Sommet*,double>>,std::vector<std::pair<const Sommet*,std::pair<const Sommet*,double>>>,myComparator>&q,std::map<const Sommet*,double>&sigma,std::map<const Sommet*,std::list<const Sommet*>>&predecesseur)const;
    void Dijkstra(std::priority_queue<std::pair<const Sommet*, std::pair<const Sommet*, double>>, std::vector<std::pair<const Sommet*, std::pair<const Sommet*, double>>>, myComparator>& pq, std::map<const Sommet*, std::pair<const Sommet*, double>>& poids) const;
    /**Conexité*/
    int k_connexe(const Sommet* arrive)const;
    void k_connexe(int& nombre_chemin,std::set<const Arete*>& arete,std::set<const Sommet*>&sommet,const Sommet*arrive)const;

    /**Sauvegarde*/
    void sauvegarde(std::ofstream&fichier)const;//sauvegarde des indices

    void flot(std::map<const Sommet*,std::pair<std::pair<const Sommet*,const Arete*>, std::pair<bool, double>>>&carte, std::list<const Sommet*>&file,std::map<const Arete*,double> &flot, const bool& connexe)const;
    void flot_reccursif(std::map<const Sommet*,std::pair<std::pair<const Sommet*,const Arete*>, std::pair<bool, double>>>&carte, std::map<const Arete*,double> &flot)const;
    void flot_reccursif(double &n_max ,std::map<const Sommet*,std::pair<std::pair<const Sommet*,const Arete*>, std::pair<bool, double>>>&carte, std::map<const Arete*,double> &flot)const;
    double flot_sortant(const std::map<const Arete*,double> &flot)const;
private:
    std::string m_nom;
    std::vector<Arete*> m_suivants,m_precedents;//arete suivante du graphe
    double m_i_d,m_i_vp,m_i_p,m_i_i,m_i_is;//indice normalisé
    double m_i_vp_nn,m_i_p_nn,m_i_i_nn,m_i_d_nn,m_i_is_nn;//indice non normalisé
    double m_i_i_max;
    double m_x,m_y;//position du sommet
};

#endif
