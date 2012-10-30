#include "clusterer.h"


namespace tld
{



bool comp(Distance a, Distance b)
{
    return a.dst < b.dst;
}

Clusterer::Clusterer(float cutoff)
{
    this->cutoff = cutoff;
}

Clusterer::~Clusterer()
{
    this->release();
}

void Clusterer::release()
{

}

void Clusterer::calcDistances( vector<BoundingBox*>& confidentWindows, vector<Distance>& distances)
{
    int dstIndex = 0;
    for(int i = 0; i < confidentWindows.size(); i++ )
    {
        for(int j = i+1; j < confidentWindows.size(); j++ )
        {
            distances[dstIndex].i = i;
            distances[dstIndex].j = j;
            distances[dstIndex].dst = confidentWindows[i]->overlap(*confidentWindows[j]);

            dstIndex++;
        }
    }

    for(int i = 0; i < distances.size(); i++ ) distances[i].dst = 1.0f - distances[i].dst;

    sort(distances.begin(), distances.end(), comp);

}
BoundingBox Clusterer::calcMeanBB( vector<BoundingBox*>& confidentWindows)
{

    float x, y, w, h, conf;
    x = y = w = h = 0;
    int size = confidentWindows.size();
    for(int i = 0; i < size ; i++)
    {
        x += confidentWindows[i]->x;
        y += confidentWindows[i]->y;
        w += confidentWindows[i]->w;
        h += confidentWindows[i]->h;

        conf += confidentWindows[i]->conf;
    }

    x /= size;
    y /= size;
    w /= size;
    h /= size;

    conf /= size;

    return BoundingBox( floor(x + 0.5), floor(y + 0.5), floor(w + 0.5), floor(h + 0.5), conf );
}

void Clusterer::cluster(vector<Distance>& distances, DisjointSet& djSet)
{
    //printf("distancesSize %d\n", distances.size());
    for(int k = 0; k < distances.size(); k++)
    {
        //printf("lol2\n");
        if( djSet.findSet(distances[k].i) == NOT_IN_A_SET && djSet.findSet(distances[k].j) == NOT_IN_A_SET )
        {
            djSet.makeSet(distances[k].i);
            djSet.makeSet(distances[k].j);

            if( distances[k].dst <= cutoff )
            {
                djSet.unionSets(distances[k].i, distances[k].j );
            }
        }
        else if( djSet.findSet(distances[k].i) != NOT_IN_A_SET && djSet.findSet(distances[k].j) == NOT_IN_A_SET )
        {
            djSet.makeSet(distances[k].j);

            if( distances[k].dst <= cutoff )
            {
                djSet.unionSets(distances[k].i, distances[k].j );
            }
        }
        else if( djSet.findSet(distances[k].i) == NOT_IN_A_SET && djSet.findSet(distances[k].j) != NOT_IN_A_SET )
        {
            djSet.makeSet(distances[k].i);

            if( distances[k].dst <= cutoff )
            {
                djSet.unionSets(distances[k].i, distances[k].j );
            }
        }
        else
        {
            if( distances[k].dst <= cutoff )
            {
                djSet.unionSets(distances[k].i, distances[k].j );
            }
        }
        //printf("lol\n");
        if( djSet.elemCount >= djSet.elements.size() && djSet.setCount == 1 ) break;
    }

    //printf("ClusterResult elemCount %d setCount %d\n", djSet.elemCount, djSet.setCount);
}

BoundingBox Clusterer::cluster( vector<BoundingBox*>& confidentWindows)
{
    BoundingBox detectionBB(0,0,0,0);
    detectionBB.detected = false;
    //this->release();

    //printf("ConfidentWindows to cluster %d\n", confidentWindows.size());
    vector<Distance> distances((confidentWindows.size()*(confidentWindows.size() - 1))/2);

    DisjointSet djSet(confidentWindows.size());

    calcDistances(confidentWindows, distances);

    cluster(distances, djSet);

    if( djSet.setCount == 1 )
    {
        detectionBB = calcMeanBB(confidentWindows);
        detectionBB.detected = true;
        //printf("DETECTED!!! DETECTED!!! DETECTED!!! DETECTED!!!\n");
    }
    else
    {
        //detectionBB = calcMeanBB(confidentWindows);
        //printf("setCount %d ElemCount %d\n", djSet.setCount, djSet.elemCount);
    }

    return detectionBB;
}

}
