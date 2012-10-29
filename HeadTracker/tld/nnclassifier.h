#ifndef NNCLASSIFIER_H
#define NNCLASSIFIER_H

#include "includes.h"

#include "normalizedpatch.h"


namespace tld {

class NNClassifier
{
    protected:
    private:


        vector<NormalizedPatch> truePositives;
        vector<NormalizedPatch> falsePositives;


    public:

        float thetaTP, thetaFP;
        NNClassifier(float thetaTP = THETA_TP, float thetaFP = THETA_FP);
        virtual ~NNClassifier();


        float classify( NormalizedPatch& patch);


        void learn( vector<NormalizedPatch>& patches);

        bool filter( NormalizedPatch& patch);


        void release();

};

}

#endif // NNCLASSIFIER_H
