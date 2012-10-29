#ifndef CLUSTERER_H
#define CLUSTERER_H

#include "boundingbox.h"
#include "disjointset.h"


namespace tld {



struct Distance
{
    int i, j;
    float dst;

    Distance()
    {
        i = j = -1;
        dst = 0.0f;
    }

};

bool comp(Distance a, Distance b);


class Clusterer
{
    private:

        void calcDistances( vector<BoundingBox*>& confidentWindows, vector<Distance>& distances);
        BoundingBox calcMeanBB( vector<BoundingBox*>& confidentWindows);
        void cluster(vector<Distance>& distances, DisjointSet& djSet);

        void release();

    public:
        float cutoff;

        Clusterer(float cutoff = CUTOFF);
        virtual ~Clusterer();

        BoundingBox cluster( vector<BoundingBox*>& confidentWindows);


};

}

#endif // CLUSTERER_H
