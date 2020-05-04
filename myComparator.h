#ifndef MYCOMPARATOR_H_INCLUDED
#define MYCOMPARATOR_H_INCLUDED

class Sommet;
#include "biblio.h"

class myComparator
{
public:
    bool operator () (const std::pair<const Sommet*,std::pair<const Sommet*,double>>&s1,std::pair<const Sommet*,std::pair<const Sommet*,double>>&s2)
    {
        return s1.second.second>s2.second.second;
    }
};

#endif // MYCOMPARATOR_H_INCLUDED
