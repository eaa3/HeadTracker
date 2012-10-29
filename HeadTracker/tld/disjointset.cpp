#include "disjointset.h"


namespace tld {

DisjointSet::DisjointSet(int maxSize)
{
    //at first nobody is in a set
    this->elements.resize(maxSize, NOT_IN_A_SET);
    this->rank.resize(maxSize, 0);

    this->setCount = 0;
    this->elemCount = 0;
}

DisjointSet::~DisjointSet()
{
    //dtor
}


void DisjointSet::makeSet(int i)
{
    if( this->elements[i] == NOT_IN_A_SET && this->elemCount < this->elements.size() )
    {
        this->elements[i] = i;
        this->setCount++;
        this->elemCount++;
    }

}
void DisjointSet::unionSets(int i, int j)
{
    this->linkSets(this->findSet(i), this->findSet(j));
}

void DisjointSet::linkSets(int i, int j)
{
    if( i != j && i != NOT_IN_A_SET && j != NOT_IN_A_SET )
    {
        if( rank[i] > rank[j] )
        {
            elements[j] = i;
        }
        else if( rank[i] < rank[j] )
        {
            elements[i] = j;
        }
        else
        {
            elements[j] = i;
            rank[i]++;
        }

        this->setCount--;
    }

}
int DisjointSet::findSet(int i)
{
    if( this->elements[i] == i || this->elements[i] == NOT_IN_A_SET )
    {
        return this->elements[i];
    }
    else
    {
        int rep = findSet(this->elements[i]);

        this->elements[i] = rep;

        return rep;
    }
}

}
