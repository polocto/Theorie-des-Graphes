#include "Sommet.h"
#include "Arete.h"
#include "myComparator.h"
#include "svgfile.h"


/**DEBUT CONSTRUCTION SOMMET*/
///constructeur
///initialisation de donn�es non pass� en param�tre � 0
Sommet::Sommet(const std::string&nom, const double& pos_x, const double& pos_y)
    :m_nom(nom),m_i_d(0),m_i_vp(1),m_i_p(0),m_i_i(0),m_i_is(0),m_i_vp_nn(0),m_i_p_nn(0),m_i_i_nn(0),m_i_d_nn(0),m_i_is_nn(0),m_i_i_max(0),m_x(pos_x),m_y(pos_y)
{
}


///Nouvelle Proposition
Sommet::Sommet(const Sommet* copie)
    :m_nom(copie->m_nom),m_i_d(0),m_i_vp(1),m_i_p(0),m_i_i(0),m_i_is(0),m_i_vp_nn(0),m_i_p_nn(0),m_i_i_nn(0),m_i_d_nn(0),m_i_is_nn(0),m_i_i_max(0),m_x(copie->m_x),m_y(copie->m_y)
{

}
///fin


/**ajout d'une arete suivante*/
void Sommet::ajout(Arete*suivant)
{
    m_suivants.push_back(suivant);
}

void Sommet::ajoutP(Arete*precedent)
{
    m_precedents.push_back(precedent);
}
/**FIN CONSTRUCTION SOMMET*/

/**DEBUT AFFICHAGE*/
///Affichage console
void Sommet::affichageconsole()const
{
    std::cout<<std::setprecision(3)
            <<m_nom<<"\ticd: ("<<std::fixed<<m_i_d<<","<<m_i_d_nn<<") "
            <<"\ticp: ("<<std::fixed<<m_i_p<<","<<m_i_p_nn<<") "
            <<"\tivp: ("<<std::fixed<<m_i_vp<<","<<m_i_vp_nn<<") "
            << "\ticis: ("<<std::fixed<<m_i_is<<","<<m_i_is_nn<<") "
            <<"\tinter: ("<<std::fixed<<m_i_i<<", "<<std::fixed<<m_i_i_nn<<")";

}
///Affichage SVG
void Sommet::affichage(Svgfile& svgout,const double&coeff)const
{
    std::string couleur;
    double coef=1-((m_i_i_max-m_i_i_nn)/m_i_i_max);
    if (coef>=0.5)
        couleur=makeRGB(100+(int)(150*coef),0,0);
    else if (coef>=0.25)
        couleur=makeRGB(0,100+(int)(155*coef*3),0);
    else
        couleur=makeRGB(0,100,(int)(155+100*(coef*7)));


    svgout.addDisk(m_x*coeff,m_y*coeff,3,couleur);//Affichage sommet
    svgout.addText(m_x*coeff-((double)m_nom.size()/2.0)*1/coeff,(m_y)*coeff-(30)*1/coeff-3,m_nom,"BLUE",coeff);//Affichage nom sommet
}

void Sommet::affichage_comparaison(Sommet* ancien)const
{

        std::cout<<std::setprecision(3)
            <<m_nom<<" :\ticd: ("<<std::fixed<<ancien->m_i_d-m_i_d<<", "<<ancien->m_i_d_nn-m_i_d_nn<<")"
            <<"\ticp: ("<<std::fixed<<ancien->m_i_p-m_i_p<<", "<<ancien->m_i_p_nn-m_i_p_nn<<")"
            <<"\tivp: ("<<std::fixed<<ancien->m_i_vp-m_i_vp<<", "<<ancien->m_i_vp_nn-m_i_vp_nn<<")"
            << "\ticis: ("<<std::fixed<<ancien->m_i_is-m_i_is<<", "<<ancien->m_i_is_nn-m_i_is_nn<<")"
            <<"\tinter: ("<<std::fixed<<ancien->m_i_i-m_i_i<<", "<<std::fixed<<ancien->m_i_i_nn-m_i_i_nn<<")"
            <<std::endl;

}
/**FIN AFFICHAGE*/

/**DEBUT GETTER*/
///retourn le nom du sommet
std::string Sommet:: getnom()const
{
    return m_nom;
}

///retourn la position x du sommet
double Sommet::getX()const
{
    return m_x;
}

///retourn la position y du sommet
double Sommet::getY()const
{
    return m_y;
}
///retourne l'indice de vecteur propre
double Sommet::get_vp()const
{
    return m_i_vp;
}

double Sommet::get_cp()const
{
    return m_i_p;
}
/**FIN GETTER*/

/**DEBUT CALCULE INDICES*/
///Calcule icd indice de centralit� de degr�
void Sommet::calc_icd(const size_t&ordre)
{
    m_i_d_nn=m_suivants.size();
    m_i_d=m_i_d_nn/(double)(ordre-1);
}
///Calcul somme des indice de vecteur propre des voisins
void Sommet::calc_vp(std::map<Sommet*,double>&somme)
{
    somme[this]=0;//initialisation � 0
    for(Arete* a : m_suivants)
        somme.at(this)+=a->get_vp(this);//somme des vecteur propre des sommet voisins

}
///mise a jour du de l'indice de vecteur propre
///normalise l'indice
void Sommet::indice_vp(std::map<Sommet*,double>&somme,const double& lambda)
{
    m_i_vp_nn=somme.at(this);
    m_i_vp=somme.at(this)/lambda;//somme des vecteur propre des sommet voisins divise par lambda
}

///mise a jour de l'indice de centralité de proximité
///normalise l'indice
void Sommet::calc_icp(double distance,double total)
{
    m_i_p_nn=1.0/distance;
    m_i_p=total/distance;
}

///mise a jour de l'indice de centralité d'intermediarité simplifier
///normalise l'indice
void Sommet::calc_ici_naif(double total,double a)
{
    m_i_is_nn=total;
    m_i_is=total/a;
}

///Calcule des indice de centralité d'intermédiarité suivant l'alorithme de Brand
///normalisé et non
void Sommet::Brand(const std::map<const Sommet*,double>&Cb,const double&n,const double &Cb_max)
{
    m_i_i_nn=0;
    if(Cb.count(this))
        m_i_i_nn=Cb.at(this);
    m_i_i=m_i_i_nn/(n*n-3*n+2);
    m_i_i_max=Cb_max;
}
///Algortihme de Brand
void Sommet::Brand(std::map<const Sommet*,double>&distance,std::priority_queue<std::pair<const Sommet*,std::pair<const Sommet*,double>>,std::vector<std::pair<const Sommet*,std::pair<const Sommet*,double>>>,myComparator>&q,std::map<const Sommet*,double>&sigma,std::map<const Sommet*,std::list<const Sommet*>>&predecesseur)const
{
    if(!distance.count(this))//Si le sommet n'a pas encore atteint son plus court chemin
    {
        distance[this]=q.top().second.second;//definir son plus court chemin (suit le principe de Dijkstra)
        for(const Arete* a : m_suivants)//parcourir l'ensemble de ses aretes adjacentes
            a->Brand(this,distance,distance.at(this),q,sigma,predecesseur);//parcourir les aretes
    }
    if(distance.at(this)==q.top().second.second && q.top().second.first != nullptr)//si la distance la plus courte du sommet est celle proposé par la file
    {
        sigma.at(this)+=sigma.at(q.top().second.first);//ajouter le nombre de chemin le plus court de son prédecesseur à lui même
        predecesseur.at(this).push_back(q.top().second.first);//ajouter son prédécesseur à l'ensemble de ses prédécesseur sur les plus courts chemins
    }
    else if(distance.at(this)>q.top().second.second)//si la distance minimal de ce sommet est supérieur à celle proposé dans la file erreur dans l'algo
        std::cout<<"Erreur"<<std::endl;
}

//Dijkstra
void Sommet::Dijkstra(std::priority_queue< std::pair<const Sommet*, std::pair<const Sommet*, double>>, std::vector< std::pair<const Sommet*, std::pair<const Sommet*, double>>>, myComparator>& pq, std::map<const Sommet*, std::pair<const Sommet*, double>>& poids)const
{
    for (const Arete* a : m_suivants)
        a->Dijkstra(pq, poids);
}

/**FIN CALCULE INDICES*/

/**DEBUT ETUDE DE CONNEXITE*/
/**etude de la k_arete connexite*/
///Sommet de départ
int Sommet::k_connexe(const Sommet* arrive)const
{
    //declaration des variables nécessaires
    int nombre_chemin=0;
    std::set<const Arete*> arete;//permet de savoir si nous sommes déjà passé par l'arête
    std::set<const Sommet*>sommet;//permet de savoir si nous sommes déjà passé par le sommet

    k_connexe(nombre_chemin,arete,sommet,arrive);

    return nombre_chemin;//retourne le nombre max de chemin sans passer deux fois par une ârete
}

///Sommet quelconque
void Sommet::k_connexe(int& nombre_chemin,std::set<const Arete*>& arete,std::set<const Sommet*>&sommet,const Sommet*arrive)const
{
    if(sommet.count(this))//si nous sommes déjà passer par ce sommet areter la récursivité
        return;
    if(arrive==this)//si nous sommes au sommet d'arrivé
    {
        nombre_chemin++;//ajouter un nouveau chemin
        return;//fin reccursivité
    }
    sommet.insert(this);//si n'existe pas et n'est pas le sommet d'arrivé marquer le sommet
    for(Arete* a : m_suivants)//parcourir l'ensemble des arets pour aller aux sommets suivants
        if (!arete.count(a))
            a->k_connexe(nombre_chemin,arete,sommet,arrive);
}



/**FIN ETUDE DE CONNEXITE*/

///Sauvegarde
void Sommet::sauvegarde(std::ofstream&fichier)const
{
    //ecriture dans le fichier
    fichier<<"\tindice de centrailite de degre : ("<<m_i_d_nn<<", "<<m_i_d<<"); ";
    fichier<<"\tindice de vecteur propre : ("<<m_i_vp_nn<<", "<<m_i_vp<<"); ";
    fichier<<"\tindice de proximite : ("<<m_i_p_nn<<", "<<m_i_p<<"); ";
    fichier<<"\tindice de centralite d'intermediarite : ("<<m_i_i_nn<<", "<<m_i_d<<"); ";
}


void Sommet::flot(std::map<const Sommet*,std::pair<std::pair<const Sommet*,const Arete*>, std::pair<bool, double>>>&carte, std::list<const Sommet*>&file,std::map<const Arete*,double> &flot, const bool& connexe)const
{
    for(Arete* s : m_suivants)
        s->flot(carte,file,flot,connexe);
    for(Arete* p : m_precedents)
        p->flot(carte,file,flot,connexe);
}


void Sommet::flot_reccursif(std::map<const Sommet*,std::pair<std::pair<const Sommet*,const Arete*>, std::pair<bool, double>>>&carte, std::map<const Arete*,double> &flot)const
{
    double n_max = carte.at(this).second.second;
    carte.at(this).first.second->flot_reccursif(carte.at(this).first.first,carte,flot,n_max);
}

void Sommet::flot_reccursif(double &n_max ,std::map<const Sommet*,std::pair<std::pair<const Sommet*,const Arete*>, std::pair<bool, double>>>&carte, std::map<const Arete*,double> &flot)const
{
    if(carte.at(this).first.first == nullptr)
        return;
    if(n_max>carte.at(this).second.second)
        n_max=carte.at(this).second.second;
    carte.at(this).first.second->flot_reccursif(carte.at(this).first.first,carte,flot,n_max);
}

double Sommet::flot_sortant(const std::map<const Arete*,double> &flot)const
{
    double somme=0;
    for(const Arete* a : m_suivants)
        if(flot.count(a))
            somme+=flot.at(a);
    return somme;
}
