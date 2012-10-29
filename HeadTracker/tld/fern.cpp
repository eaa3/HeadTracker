#include "fern.h"


namespace tld {

Fern::Fern(int nNodes, float minScale, float maxScale)
{
    this->nLeafs = powf(4.0f, float(nNodes) );

    this->p.resize(nLeafs,0);
    this->n.resize(nLeafs,0);

    this->posterior.resize(nLeafs, 0);

    this->node.resize(nNodes);

    for(int i = 0; i < nNodes; i++) this->node[i] = new TwoBitBP(minScale,maxScale);




}

Fern::~Fern()
{
    for(int i = 0; i < this->node.size(); i++) delete this->node[i];
}


int Fern::leafIndex(Mat& integralImage, BoundingBox patch)
{

    int width = integralImage.cols;
    int height = integralImage.rows;


    //The patch must be inside the image
    patch.x = max(min(patch.x, width - 1), 1);
    patch.y = max(min(patch.y, height - 1), 1);

    patch.w = min(patch.w, width - patch.x);
    patch.h = min(patch.h, height - patch.y);


    int leaf = 0;
    int pattern = 0;
    for(int i = 0; i < this->node.size(); i++)
    {
        pattern = this->node[i]->evaluate(integralImage, patch);
        leaf = leaf | (pattern << i*2);
    }


    return leaf;
}


void Fern::train( Mat& integralImage,  BoundingBox& patch, int patchLabel)
{
    int leaf = this->leafIndex(integralImage, patch);


    if( patchLabel == 0 )
    {
        this->n[leaf]++;
    }
    else
    {
        this->p[leaf]++;
    }

    if( p[leaf] > 0 ){
         this->posterior[leaf] = float(p[leaf])/(float(this->p[leaf] + this->n[leaf]));
    }
}

float Fern::classify( Mat& integralImage,  BoundingBox& patch)
{
    int leaf = this->leafIndex(integralImage, patch);

    return this->posterior[ leaf ];
}

}
