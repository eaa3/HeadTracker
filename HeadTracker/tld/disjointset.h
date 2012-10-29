#ifndef DISJOINTSET_H
#define DISJOINTSET_H

#include <vector>


namespace tld {

using std::vector;

const int NOT_IN_A_SET = -1;

class DisjointSet
{
    public:
        DisjointSet(int maxSize);
        virtual ~DisjointSet();


        vector<int> elements, rank;
        int setCount, elemCount;

        void makeSet(int i);
        void unionSets(int i, int j);
        void linkSets(int i, int j);

        int findSet(int i);


    protected:
    private:
};

}

#endif // DISJOINTSET_H
