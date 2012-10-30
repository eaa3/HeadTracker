#include "nnclassifier.h"


namespace tld {

NNClassifier::NNClassifier(float thetaTP, float thetaFP)
{
    this->thetaTP = thetaTP;
    this->thetaFP = thetaFP;
}

NNClassifier::~NNClassifier()
{
    //dtor
}

void NNClassifier::release()
{
    this->truePositives.clear();
    this->falsePositives.clear();
}


float NNClassifier::classify( NormalizedPatch& patch)
{
    if( this->truePositives.empty() )
    {
        return 0.0f;
    }

    if( this->falsePositives.empty() )
    {
        return 1.0f;
    }

    float max_ncc_p = 0.0f;
    float max_ncc_n = 0.0f;
    float current_ncc = 0.0f;

    for(int i = 0; i < this->truePositives.size(); i++)
    {
        current_ncc = patch.ncc(this->truePositives[i]);

        max_ncc_p = max(max_ncc_p, current_ncc);
    }

    for(int i = 0; i < this->falsePositives.size(); i++)
    {
        current_ncc = patch.ncc(this->falsePositives[i]);

        max_ncc_n = max(max_ncc_n, current_ncc);
    }



    float dP = 1.0f - max_ncc_p;
    float dN = 1.0f - max_ncc_n;

    float dist = dN/(dN+dP);

    return dist;
}

bool NNClassifier::filter( NormalizedPatch& patch)
{
    float conf = classify(patch);

    return conf >= thetaTP;
}



void NNClassifier::learn(  vector<NormalizedPatch>& patches)
{
    float conf = 0.0f;
    bool learntP = false, learntN = false;
    for(int i = 0; i < patches.size(); i++)
    {
        conf = this->classify(patches[i]);

        if(patches[i].positive && conf < thetaTP)
        {

            this->truePositives.push_back(patches[i]);
            learntP = true;
        }

        if(!patches[i].positive && conf > thetaFP)
        {
            falsePositives.push_back(patches[i]);
            learntN = true;
        }
    }

    //if( learntP ) printf("WE'VE LEARNT POSITIVES! Model_P_Size %d\n", this->truePositives.size());
   // if( learntN ) printf("WE'VE LEARNT NEGATIVES! Model_N_Size %d\n", this->falsePositives.size());


}


}
