#include "Arete.h"
#include "Sommet.h"
#include "myComparator.h"
#include "svgfile.h"
/**CONSTUCTION DU ARETE DEBUT*/
///Constructeur de la class Arete
///Prend en parametre les adresses des deux extremit�e pour initialiser arrete et pour ajouter l'arrete a leur liste de suivant
///initialisation du poids a 1 cas d'un graph non ponder�
Arete::Arete(Sommet* s1, Sommet* s2,bool oriente)
    :m_ext1(s1),m_ext2(s2),m_poids(1)
{
    m_ext1->ajout(this);
    if (!oriente)
        m_ext2->ajout(this);
    else
        m_ext2->ajoutP(this);
}
///constructeur copie arete
///Nouvelle proposition
Arete::Arete(const Arete* copie,const std::map<const Sommet*,Sommet*>&traducteur,bool oriente)
    :m_poids(copie->m_poids)
{
    if(!traducteur.count(copie->m_ext1) || !traducteur.count(copie->m_ext2))
        throw (0);

    m_ext1=traducteur.at(copie->m_ext1);
    m_ext2=traducteur.at(copie->m_ext2);
    m_ext1->ajout(this);
    if (!oriente)
        m_ext2->ajout(this);
    else
        m_ext2->ajoutP(this);
}
///fin

///initialisation des ponderation apartir d'un flot de fichier
void Arete::ponderation(std::stringstream& iss)
{
    iss>>m_poids;

    if (iss.fail())
        throw("probleme dans la lecture de la ligne du fichier: fichier corompu");
}
/**CONSTUCTION DE ARETE FIN*/
/**AFFICHAGE DE ARETE DEBUT*/
///Affichage des informations lier a Arete dans la console
void Arete::affichageconsole()const
{
    std::cout<<m_ext1->getnom()<<" "<<m_ext2->getnom()<<" "<<m_poids;//utilisation de getteur pour acceder au nom des sommets
}

void Arete::afficher_extremites()const
{
    std::cout << m_ext1->getnom() << " / " << m_ext2->getnom();
}

/// Affichage au format svg d'une Arete du graph
void Arete::affichage(Svgfile& svgout,const bool&oriente,const double& coeff_t)const
{

    svgout.addLine(m_ext1->getX()*coeff_t,m_ext1->getY()*coeff_t,m_ext2->getX()*coeff_t,m_ext2->getY()*coeff_t,"BLACK");
    if(oriente)
    {
        double d_x=m_ext2->getX()*coeff_t,d_y=m_ext2->getY()*coeff_t;
        double x=m_ext2->getX()-m_ext1->getX(),y=m_ext2->getY()-m_ext1->getY();
        double module=sqrt(pow(x,2)+pow(y,2));
        double a=(-y)/module,b=x/module;
        double coeff=2;
        x/=module;
        y/=module;
        double pointe_x = d_x-3*x, pointe_y=d_y-3*y;
        double ext1_x=d_x-(3+coeff)*x-a*coeff,ext1_y=d_y-(3+coeff)*y-coeff*b;
        double ext2_x=d_x-(3+coeff)*x+a*coeff,ext2_y=d_y-(3+coeff)*y+coeff*b;
        svgout.addTriangle(pointe_x,pointe_y,ext1_x,ext1_y,ext2_x,ext2_y,"black");
    }
}
/**AFFICHAGE DE ARETE FIN*/

/**GETTER DEBUT*/
///recupere l'indice de vecteur propre du voisin
double Arete::get_vp(Sommet*precedent)const
{
    if(precedent==m_ext1)
        return m_ext2->get_vp();
    return m_ext1->get_vp();
}

///getteur qui retourn le poids
double Arete::get_poid()const
{
    return m_poids;
}

/**GETTER FIN*/


bool Arete::verrif(const std::string&ext1,const std::string&ext2)const
{
    //return ext1==m_ext1->getnom() && m_ext2->getnom()==ext2;
    return true;
}

/**Indice intermediarite*/
void Arete::Brand(const Sommet*precednent,std::map<const Sommet*,double>&distance,const double&d_a,std::priority_queue<std::pair<const Sommet*,std::pair<const Sommet*,double>>,std::vector<std::pair<const Sommet*,std::pair<const Sommet*,double>>>,myComparator>&q,std::map<const Sommet*,double>&sigma,std::map<const Sommet*,std::list<const Sommet*>>&predecesseur)const
{
    Sommet* suivant=nullptr;
    if(precednent==m_ext1)//definir qui est le sommet suivant
        suivant=m_ext2;
    else
        suivant=m_ext1;

    if(!distance.count(suivant) || distance.at(suivant)>d_a+m_poids)//si mon sommet n'a pas de distance ou que sa distance min est sup à nouvelle distance(ce sui ne devrai pas arriver)
    {
        q.push(std::pair<const Sommet*,std::pair<const Sommet*,double>> {suivant,std::pair<const Sommet*,double>{precednent,d_a+m_poids}}); //ajouter à la file
        sigma[suivant]=0;//initialiser son nombre de chemin dont il fait parti à zero
        predecesseur[suivant].clear();//vider sa liste de précédent
    }
    else if(distance.at(suivant)==d_a+m_poids)//si son nombre de sommet est égale
    {
        sigma.at(suivant)+=sigma.at(precednent);//actualiser son nombre de chemin
        predecesseur.at(suivant).push_back(precednent);//ajouter predecesseu à l'ensemble des predecesseur
    }
}

//Dijkstra
void Arete::Dijkstra(std::priority_queue< std::pair<const Sommet*, std::pair<const Sommet*, double>>, std::vector< std::pair<const Sommet*, std::pair<const Sommet*, double>>>, myComparator>& pq, std::map<const Sommet*, std::pair<const Sommet*, double>>& poids)const
{
    if (!poids.count(m_ext1))
        pq.push(std::pair<const Sommet*, std::pair<const Sommet*, double>>{m_ext1, std::pair<const Sommet*, double>{pq.top().first, pq.top().second.second + m_poids}});
    else if (!poids.count(m_ext2))
        pq.push(std::pair<const Sommet*, std::pair<const Sommet*, double>>{m_ext2, std::pair<const Sommet*, double>{pq.top().first, pq.top().second.second + m_poids}});
}

/**k-arete connexité*/
void Arete::k_connexe(int& nombre_chemin,std::set<const Arete*>& arete,std::set<const Sommet*>&sommet,const Sommet*arrive)const
{
    arete.insert(this);//saturer l'arete
    if(!sommet.count(m_ext1))//si mon sommet ext1 n'a pas était emprunter
        m_ext1->k_connexe(nombre_chemin,arete,sommet,arrive);
    else if(!sommet.count(m_ext2))//si mon sommet ext2 n'a pas était emprunter
        m_ext2->k_connexe(nombre_chemin,arete,sommet,arrive);
}


void Arete::flot(std::map<const Sommet*,std::pair<std::pair<const Sommet*,const Arete*>, std::pair<bool, double>>>&carte, std::list<const Sommet*>&file,std::map<const Arete*,double> &flot, const bool& connexe)const
{
    double poids=1;
    if(!connexe)
        poids=m_poids;
    if(!carte.count(m_ext2) && ( flot.at(this)<poids ))
    {
        file.push_back(m_ext2);
        carte[m_ext2]=std::pair<std::pair<const Sommet*,const Arete*>, std::pair<bool, double>>{{m_ext1,this},{true,poids-flot.at(this)}};
    }
    else if(!carte.count(m_ext1) && ( flot.at(this) ))
    {
        file.push_back(m_ext1);
        carte[m_ext1]=std::pair<std::pair<const Sommet*,const Arete*>, std::pair<bool, double>>{{m_ext2,this},{false,flot.at(this)}};
    }
}


void Arete::flot_reccursif(const Sommet*suivant,std::map<const Sommet*,std::pair<std::pair<const Sommet*,const Arete*>, std::pair<bool, double>>>&carte, std::map<const Arete*,double> &flot,double&n_max)const
{
    Sommet* temp = nullptr;
	if ( (suivant == m_ext1  )|| suivant == m_ext2)
		suivant->flot_reccursif(n_max,carte,flot);
	else
		return;
	if (m_ext1 == suivant)
		temp = m_ext2;
	else
		temp = m_ext1;
	if (carte.at(temp).second.first)
		flot.at(this) += n_max;
	else
		flot.at(this) -= n_max;
}
