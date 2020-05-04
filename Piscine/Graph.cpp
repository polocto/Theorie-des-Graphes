#include "Graph.h"
#include "Arete.h"
#include "Sommet.h"
#include "myComparator.h"
#include "svgfile.h"

/**DEBUT CHARGMENT INFOS LIEE AU GRAPH*/
///Constructeur de graph:
///Prend en paramettre le nom du fichier utilisé
/// Crée et initialise les valeur des arretes et sommet en utilisant leur constructeur avec les information du fichier
Graph::Graph(std::ifstream&ifs)
    :m_indice{0},m_coeff_aff(0)
{
    std::string line;
    double x_aff=0,y_aff=0;
    size_t ot=0;
    if(!std::getline(ifs,line) || !is_int(line))
        throw(1);
    m_oriente=(bool)std::stoi(line);//orientation du graphe
    if(!std::getline(ifs,line) || !is_int(line))
        throw(2);
    ot=(size_t)std::stoi(line);//ordre du graph
    if(ifs.fail())
        throw(3);
    ///lecture Sommets
    for(size_t i=0; i<ot; i++)
    {
        std::string nom;
        int verrif=0;
        double x=0,y=0;
        if(!std::getline(ifs,line))
            throw(4);//recupération ligne du fichier
        std::stringstream iss(line);
        iss>>verrif;
        if(iss.fail()|| verrif!=(int)i)
            throw(5);
        iss>>nom;
        if(iss.fail())
            throw(6);
        iss>>x;
        if(iss.fail())
            throw(7);
        if(x>x_aff)
            x_aff=x;
        iss>>y;
        if(iss.fail())
            throw(8);
        if(y>y_aff)
            y_aff=y;
        if (y<1)
            y++;
        m_sommets.push_back(new Sommet(nom,x,y));// Creation d'une sommet avec les parametre du fichier nom, x, y
    }
    if(!std::getline(ifs,line) || !is_int(line))
        throw(9);
    ot=(size_t)std::stoi(line);//taille du graph
    ///lecture Arete
    for(size_t i=0; i<ot; i++)
    {
        std::string line;
        size_t verrif=0, s1=0,s2=0;
        if(!std::getline(ifs,line))
            throw(10);
        std::stringstream iss(line);
        iss>>verrif;
        if(iss.fail()||verrif!=i)
            throw(11);
        iss>>s1;
        if(ifs.fail() || s1>m_sommets.size())
            throw(12);
        iss>>s2;
        if(ifs.fail() || s2>m_sommets.size())
            throw(13);
        m_aretes.push_back(new Arete(m_sommets[s1],m_sommets[s2],m_oriente));// Creation d'une Arete a partir des informations du fichier
    }
    x_aff=950/x_aff;
    y_aff=750/y_aff;
    m_coeff_aff=x_aff;
    if(y_aff<m_coeff_aff)
        m_coeff_aff=y_aff;
}
///Construction d'un graph partiel
///Reccupération du graph à copié et suppression du changment reccupéré en string
Graph::Graph(Graph* Gmodel,std::string changement)
    :m_oriente(Gmodel->m_oriente),m_indice(Gmodel->m_indice),m_coeff_aff(Gmodel->m_coeff_aff)//MODIF
{
    std::string ext1="",ext2="";
    unsigned int i=0;
    while (i<changement.size() && changement[i]!='/')
        ext1+=changement[i++];

    if (i<changement.size())
        for (unsigned int j=i+1;j<changement.size();j++)
            ext2+=changement[j];
   else       ext2=" ";
    std::cout<<ext1<<std::endl<<ext2<<std::endl;
    std::map<const Sommet*,Sommet*>traducteur;//MODIF
    if (ext2==" ")//retrait d'un sommet
    {
        for (auto s: Gmodel->m_sommets)
            if (s->getnom()!=ext1)
            {
                m_sommets.push_back(new Sommet(s));//MODIF
                traducteur[s]=m_sommets.back();//MODIF
            }

        for (auto s: Gmodel->m_aretes)
        {
            try
            {
                Arete* temp=new Arete(s,traducteur,m_oriente);//MODIF
                m_aretes.push_back(temp);//MODIF
            }
            catch(int a)
            {
                a = 2;
            }
        }
    }
    else//retrait d'une arete
    {
        for (auto s: Gmodel->m_sommets)//récuppération des valeur d'un sommet et copie dans un nouveau sommet
        {
            m_sommets.push_back(new Sommet(s));//MODIF
            traducteur[s]=m_sommets.back();//MODIF
        }

        for (auto s: Gmodel->m_aretes)
        {
            if (!s->verrif(ext1,ext2) && (m_oriente || (!m_oriente && !s->verrif(ext2,ext1)  )))//selection des aretes à copier
            {
                m_aretes.push_back(new Arete(s,traducteur,m_oriente));//copie des aretes//MODIF
            }
        }
    }
}

///Destructeur de Graph
///Supression de tout les sommets et de toutes les aretes qui le compose
Graph::~Graph()
{
    for(Sommet* s: m_sommets)
        delete s;//Suppression de l'ensemble des sommets
    for(Arete* a : m_aretes)
        delete a;//suppression de l'ensemble des aretes
}

///Chargement de la ponderation
///mise en place d'une ponderation au arete du graph a partir d'un fichier
void Graph::chargementPonderation(std::string nomfichier)
{
    std::ifstream ifs(nomfichier);
    std::string line;
    int taille;

    try
    {
        if(!ifs.is_open())
            throw("probleme a l'ouverture du fichier:"+nomfichier);

        //recuperation de la taille du graph
        if(std::getline(ifs, line))
        {
            std::stringstream iss(line);
            iss>>taille;
        }
        else
            throw("probleme dans la lecture de la ligne du "+nomfichier+" : fichier corompu");

        if(taille>(int)m_aretes.size())
            throw(nomfichier+" non comforme : trops d'arrete pour la taille du graph // fichier peu etre corompu");

        for (int i=0; i<taille; i++)
        {
            std::getline(ifs, line);
            std::stringstream iss(line);
            int indice;
            iss >> indice;//recuperation de la position de l'arrete dans le vecteur d'arete de graph

            if (iss.fail())
                throw("probleme dans la lecture de la ligne du "+nomfichier+" : fichier corompu");

            if (indice>taille)
                throw("fichier non comforme : trops d'arrete pour la taille du graph // fichier peu etre corompu");

            m_aretes[indice]->ponderation(iss);//apele de la fonction de arete pour modifier la valeur du poid
        }
    }
    catch (std::string probleme)
    {
        std::cout<<probleme;
    }
}
/**FIN DES INFOS LIEE AU CHARGEMENT*/

/**DEBUT DE L'AFFICHAGE D'UN GRAPH*/
void Graph::affichage_poly()const
{
    Svgfile svgout;
    affichage(svgout);
    affichageconsole();
}

/// Affichage du graph au format svg
/// Appel des fonction affichage pour chaque sommets et chaque arretes
void Graph::affichage(Svgfile& svgout)const
{
    int som_max=(int)m_sommets[0]->getY();
    for (auto s: m_sommets)
        if (som_max<s->getY())
            som_max=(int)s->getY();
    som_max*=(int)m_coeff_aff;
    for (int i=0;i<155;i++)
        svgout.addRect((double)(100+ (double)i), (double)((double)som_max + 20),1,30,makeRGB(0,100,155+i),makeRGB(0,100,155+i));

    for (int i=0;i<155;i++)
        svgout.addRect((double)(255+ (double)i), (double)((double)som_max+20),1,30,makeRGB(0,155+ i,0),makeRGB(0,100,155+i));

    for (int i=0;i<155;i++)
        svgout.addRect((double)(410+ (double)i), (double)((double)som_max+20),1,30,makeRGB(155+i,0,0),makeRGB(155+i,0,0));

    for (Arete* A:m_aretes)
        A->affichage(svgout,m_oriente,m_coeff_aff);//affiche l'ensemble des aretes
    for (Sommet* S:m_sommets)
        S->affichage(svgout,m_coeff_aff);//affiche l'ensemble des sommets
}

void Graph::affichage_suppression()
{
    std::cout<<"Voici la liste des sommet du graph :\n";
    for (auto s:m_sommets)
        std::cout<< s->getnom()<<"  -  ";

    std::cout<<std::endl;
    for (auto a : m_aretes)
    {
        a->afficher_extremites();
        std::cout << std::endl;
    }

}

///Affichage des parametre du graphe en console
///utiliser pour verification du chargement du graphe
void Graph::affichageconsole()const
{
    std::cout<<"l'indice de centralite global du graph est : "<<m_indice<<std::endl;
    std::cout<<" Sommet composant le graphe :"<<std::endl;

    for( auto s:m_sommets)
    {
        s->affichageconsole();//Appel de l'affichage des paramettre des Sommets
        std::cout<<std::endl;
    }
}



void Graph::connexite()const
{
    if (m_oriente)
        std::cout<<"le graphe oriente est " << k_ko()<<" fortement connexe"<<std::endl;
    else
        std::cout<<"le graphe est "<<k_connexe()<<" conexe(s)"<<std::endl;
}
/**CALCULE DES INDICES DU GRAPH*/


void Graph::calcule_indices()
{
    calc_icd();
    calc_vect_propre();
    calc_icp();
    Brand();
    calc_ici_naif();
    calc_indice_total();
}

///Calcule de la centralité de proximiter du graph entier
void Graph::calc_indice_total()
{
    double max1=0;
    double total=0;
    for (auto s:m_sommets)
        if(max1<s->get_cp())
            max1=s->get_cp();

    for (auto s:m_sommets)
    {
        total+=max1-s->get_cp();
    }
    m_indice=total/(((double)m_sommets.size()* (double)m_sommets.size())+3* (double)m_sommets.size()+2)/(2* (double)m_sommets.size()-3);
}


///Calcule de l'indice de centralité de degrée
void Graph::calc_icd()
{
    for (auto s:m_sommets)
        s->calc_icd(m_sommets.size());
}

///Calucle de l'indice de vecteur propre
void Graph::calc_vect_propre()
{
    double lambda=-1,l=0;

    while(abs(lambda-l)>=0.001)//tant que variance de lambda superieur à 10^-3
    {
        std::map<Sommet*,double>somme;
        lambda = l;
        l=0;
        for(Sommet* s : m_sommets)
            s->calc_vp(somme);//somme de vecteur propre de sommet voisins pour chaque sommets
        for(Sommet* s : m_sommets)
            l+=somme.at(s)*somme.at(s);//somme des carré des sommes ci-dessus
        l=sqrt(l);//racine de la somme
        for(Sommet* s : m_sommets)
            s->indice_vp(somme,l);//actualisation des indices
    }
}
///Calcule de l'indice de centralité d'intermédiarité avec l'algorithme de Brand
void Graph::Brand()
{
    std::map<const Sommet*,double>Cb;
    double Cb_max=0;
    for(const Sommet* s : m_sommets)
        Cb[s]=0;
    for(Sommet* d : m_sommets)
    {
        //Déclarations
        std::stack<const Sommet*>p;
        std::set<const Sommet*> verrif;
        std::map<const Sommet*,double>delta;
        std::map<const Sommet*,std::list<const Sommet*>>predecesseur;
        std::map<const Sommet*,double>sigma;
        std::map<const Sommet*,double>distance;
        std::priority_queue<std::pair<const Sommet*,std::pair<const Sommet*,double>>,std::vector<std::pair<const Sommet*,std::pair<const Sommet*,double>>>,myComparator>q;
        //utiliser Dijkstra
        q.push(std::pair<const Sommet*,std::pair<const Sommet*,double>> {d,std::pair<const Sommet*,double>{nullptr,0}});
        sigma[d]=1;
        while(!q.empty())
        {
            if(!verrif.count(q.top().first))
            {
                p.push(q.top().first);
                verrif.insert(q.top().first);
            }
            q.top().first->Brand(distance,q,sigma,predecesseur);
            q.pop();
        }
        for(const Sommet* v : m_sommets)
            delta[v]=0;
        //Méthode de BRAND
        while(!p.empty())
        {
            if(p.top()!=d)
            {
                for(const Sommet* pre : predecesseur.at(p.top()))
                    delta.at(pre)+=(sigma.at(pre)/sigma.at(p.top()))*(1+delta.at(p.top()));
                Cb.at(p.top())+=delta.at(p.top());
                if(Cb.at(p.top())>Cb_max)
                    Cb_max=Cb.at(p.top());
            }
            p.pop();
        }
    }
    //affiliation des valeurs calculé au différentssommets du graphe
    for(Sommet* s : m_sommets)
        s->Brand(Cb,(double)m_sommets.size(),Cb_max);
}

///calcule de l'indicateur de centralité de proximité pour chaque sommet
/// On test ainsi tout les chemins possible entre 2 sommets pour sommer les distances
void Graph::calc_icp()
{
    for (auto i:m_sommets)
    {
        double distance=0;
        std::map<const Sommet*,std::pair<const Sommet*,double>> tampon=Dijkstra(i);
        for(auto s:m_sommets)
        {
            if (s!=i && tampon.count(s))
                distance+=tampon.at(s).second;
        }
        i->calc_icp(distance,m_sommets.size()-1);//envoie les valeur pour modifier la valeur de l'indice icp des sommet
    }
}

///calcul de l'indice de centralité d'intermédiarité avec l'agorithme naïf
///considère qu'il n'y a que un plus court chemin
void Graph::calc_ici_naif()
{
    const Sommet* actuel=nullptr;
    double a=0;
    std::map<const Sommet*,double> total;
    for(auto i:m_sommets)
        total[i]=0;
    for(auto i:m_sommets)
    {
        std::map<const Sommet*,std::pair<const Sommet*,double>> tampon=Dijkstra(i);

        for(auto arrive : m_sommets)
        {
            if (tampon.count(arrive))
            {
                actuel=tampon.at(arrive).first;
                if(actuel != nullptr && actuel != i)
                    a++;
                while(actuel!=nullptr && actuel != i )
                {
                    total.at(actuel)++;
                    actuel = tampon.at(actuel).first;
                }
            }
            
        }
    }
    for(auto i:m_sommets)
        i->calc_ici_naif(total.at(i),a);
}
/**Début Dijkstra*/

/**Algorithme de dijkstra modifier pour donner la longeur du plus cour chemin entre deux Sommets
Prend en paramettre l'adresse de depart et l'adresse d'arriver et renvoie une distance total*/
std::map<const Sommet*,std::pair<const Sommet*,double>> Graph::Dijkstra(Sommet* depart)const
{
    //Declaration de variable
    std::priority_queue< std::pair<const Sommet*, std::pair<const Sommet*, double>>, std::vector< std::pair<const Sommet*, std::pair<const Sommet*, double>>>, myComparator>pq;
    std::map<const Sommet*,std::pair<const Sommet*,double>> poids;


    //ajout du Sommet de depart a la liste
    pq.push(std::pair<const Sommet*, std::pair<const Sommet*, double>>{depart, std::pair<const Sommet*, double>{nullptr,0}});

    while ( !pq.empty())//La boucle tourne tant que la liste est remplie et le Sommet d'arriver n'est pas marquer
    {
        poids[pq.top().first] = pq.top().second;

        pq.top().first->Dijkstra(pq, poids);

        while (!pq.empty() && poids.count(pq.top().first))
            pq.pop();
    }

    return poids;//on retourne le poids du chemin*/
}
/**Fin Dijkstra*/
/**FIN CALCULE DES INDICES DU GRAPH*/

/**TEST LA CONNEXITE DU GRAPH*/

///Test la k-arete-connexité d'un graph avec la méthode des flot
///la capacité de l'ensemble des aretes est équivalente à 1 pour 1 passage par arete
int Graph::k_connexe()const
{
    int k=-1;

    for(Sommet*d : m_sommets)//en partant de chaque sommet
    {
        for(Sommet * a : m_sommets)//pour aller à l'ensemble des autres sommet
        {
            if(a!=d)
            {
                int temp=d->k_connexe(a);//nombre de chemin existant pour aller de d à a
                if(temp<k || k< 0)//prend le plus grand nombre de chemin existant
                    k=temp;
            }
        }
    }
    return k;//retourne le nombre de chemin différent en empruntant qu'une seul fois les arete sois la k-arete-connexité
}

double Graph::k_ko()const
{
    double k = -1;
    for (const Sommet* s : m_sommets)
    {
        for (const Sommet* p : m_sommets)
        {
            if (p != s)
            {
                double tampon = recherche_de_flot(s, p, true);
                if (tampon < k || k < 0)
                    k = tampon;
            }
        }
        if (!k)
            break;
    }
    return k;
}
/**FIN DU TEST DE CONNEXITE D'UN GRAPH*/

/**DEBUT SAUVEGARDE*/
/**Sauvegarde*/
void Graph::sauvegarde(std::ofstream&fichier)const
{
    int i=0;
    for(Sommet* s : m_sommets)
    {
        fichier<<"Sommet : "<<i++;//indice du sommet
        s->sauvegarde(fichier);//écriture des données d'un sommet
        fichier<<std::endl;//retour à la ligne
    }
}
/**FIN SAUVEGARDE*/

/**DEBUT TEST LA VULNERABILTE D'UN GRAPH*/
///  Vulnerabilité
Graph* Graph::Supression_element()
{
    Graph* etude_2;
    std::string choix;
    std::cout<<"Quel element voulez vous supprimer?";
    std::cin>>choix;//saisie

    etude_2=new Graph(this,choix);

    if(etude_2->m_aretes.size() == m_aretes.size() && etude_2->m_sommets.size() == m_sommets.size() )
    {
        delete etude_2;
        throw (0);
    }


    return etude_2;
}

void Graph::comparaison_graph(Graph* ancien)
{
    std::cout<< "Evolution des indicateur apres la suppression \n";
    for (auto s:m_sommets)
    {
         bool fin=1;
        unsigned int  i=0;
        fin=1;
        while(fin && i<ancien->m_sommets.size())
        {
            if (ancien->m_sommets[i]->getnom()==s->getnom())
                fin=0;
            else
                i++;
        }
        if(i<ancien->m_sommets.size())
            s->affichage_comparaison(ancien->m_sommets[i]);
    }
}

/**FIN TEST LA VULNERABILTE D'UN GRAPH*/


double Graph::recherche_de_flot(const Sommet* s,const Sommet* p , const bool& connexe)const
{
    std::list<const Sommet*>file;
    bool continuer = true;
    std::map<const Arete*,double> flot;
    for (Arete* a : m_aretes)
        flot[a] = 0;
    while(continuer)
    {
        continuer = false;
        std::map<const Sommet*,std::pair<std::pair<const Sommet*,const Arete*>, std::pair<bool, double>>>carte;
        file.clear();
        file.push_front(s);
        carte[s]=std::pair<std::pair<const Sommet*,const Arete*>, std::pair<bool, double>>{{nullptr,nullptr},{true,1}};
        do
        {
            file.front()->flot(carte,file,flot,connexe);
                file.pop_front();
        }while(!file.empty() && !carte.count(p));
        if(carte.count(p))
        {
            continuer = true;
            p->flot_reccursif(carte,flot);
        }
    }

    return s->flot_sortant(flot);
}

Sommet* Graph::saisie()const
{
    std::string tampon;
    Sommet* sommet = nullptr;
    bool fin=false;
    do
    {
        std::cin>>tampon;
        for(Sommet* s : m_sommets)
        {
            if(s->getnom()==tampon)
            {
                sommet=s;
                fin = true;
            }
        }
    }while(!fin);
    return sommet;
}

void Graph::chemin_le_plus_court()const
{
    std::map<const Sommet*,std::pair<const Sommet*,double>> tampon;
    Sommet*depart=nullptr;
    const Sommet*arrive=nullptr;
    std::cout<<"Saisir depart : ";
    depart=saisie();
    std::cout<<"Saisir arrive : ";
    arrive=saisie();
    tampon=Dijkstra(depart);
    while(arrive!=nullptr)
    {
        std::cout<<arrive->getnom();
        if(tampon[arrive].first!=nullptr)
            std::cout<<"<--";
        arrive=tampon[arrive].first;
    }
    std::cout<<std::endl;
}

void Graph::flot_entre_deux_point()const
{
    Sommet* Sdepart=nullptr;
    Sommet* Sarriver=nullptr;
    double poid=0;
    std::cout<< "saississez le sommet de depart : ";
    Sdepart=saisie();
        std::cout<< "saississez le sommet d'arriver : ";
    Sarriver=saisie();
    //saisir les deux point
    if (Sdepart && Sarriver)
    {
        poid=recherche_de_flot(Sdepart,Sarriver,false);
        std::cout<<"le flot max entre la sation "<<Sdepart->getnom() << " et la station "<<Sarriver->getnom()<<" est egale "<<poid<<std::endl;
    }
    else std::cout<< " saisie incorecte des sommets";

}

void Graph::parcours()const
{
    std::string choix;
    if(m_oriente)
    {
        do
        {
            std::cout<<"1/Parcours entre deux Sommets"<<std::endl;
            std::cout<<"2/Flot entre deux Sommets"<<std::endl;
            std::cin>>choix;
        }while(!is_int(choix));
        switch(std::stoi(choix))
        {
        case 1:
            chemin_le_plus_court();
          break;
        case 2:
            flot_entre_deux_point();
            break;
        }
    }
    else
        chemin_le_plus_court();
}
