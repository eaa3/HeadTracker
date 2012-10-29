#ifndef NORMALIZEDPATCH_H
#define NORMALIZEDPATCH_H

#include "boundingbox.h"

namespace tld {

class BoundingBox;

class NormalizedPatch
{
    public:

        float values[NORMALIZED_PATCH_DIM*NORMALIZED_PATCH_DIM];
        bool positive;



        NormalizedPatch( const NormalizedPatch& other);
        NormalizedPatch( Mat& image,  BoundingBox& bb, bool positive = false);
        NormalizedPatch();

        void release();
        virtual ~NormalizedPatch();

        float ncc( NormalizedPatch& other);

        static void extractNormalizedValues( Mat& image,  BoundingBox& bb, float* out_values);


    protected:

};

}

#endif // NORMALIZEDPATCH_H
