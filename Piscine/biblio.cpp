#include "biblio.h"
///Methode de verification si un string est un entier positif
bool is_int(const std::string&s)
{
    size_t i=0;
    while(i<s.size() && std::isdigit(s[i]))
        i++;//parcours l'ensemble des caractere du string est vérifi si c'est un digit

    return i && i==s.size();
}
