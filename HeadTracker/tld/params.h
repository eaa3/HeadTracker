#ifndef PARAMS_H
#define PARAMS_H

#include <cstdio>
#include <string>
#include <cstdlib>
#include "includes.h"

using std::string;


namespace tld {

class Params
{
    public:
        Params(string paramsFile = "none");
        virtual ~Params();


        int frame_w;
        int frame_h;
        int fps;


        //Fern Forest Params
        int n_ferns;
        int n_features;
        float min_scale;
        float max_scale;
        float decision_threshold;

        //P-N Criteria of deciding if a bb is positive or not.
        float min_positive_overlap;
        float max_negative_overlap;


        //Clustering overlapping condition
        float cutoff;


        //Patch dimension
        int normalized_patch_dim;

        /*detector params*/
        int min_bb;
        int min_window_scale;
        int max_window_scale;
        float base_window_scale;
        float shift;

         /*1-NN Classifier Constants*/
         float theta_tp;
         float theta_fp;



        /*<NOT USED YET>

        //Threshold defining the validity of the tracker
        #define THRESHOLD_NN_VALID 0.7f

        //Threshold defining if some patch is the same as the already existing inside the positive/negative set
        #define THE_SAME 0.95f

        </NOT USED YET>*/

        void save(string paramsFile);

    protected:
    private:
};

}

#endif // PARAMS_H
