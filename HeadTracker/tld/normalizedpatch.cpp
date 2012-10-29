#include "normalizedpatch.h"


namespace tld {

NormalizedPatch::NormalizedPatch()
{
    this->positive = false;
}

NormalizedPatch::NormalizedPatch( const NormalizedPatch& other)
{
    (*this) = other;
}

NormalizedPatch::NormalizedPatch( Mat& image,  BoundingBox& bb, bool positive)
{
    extractNormalizedValues(image, bb, this->values);
    this->positive = positive;
}

void NormalizedPatch::release()
{
    //delete [] this->values;
}

NormalizedPatch::~NormalizedPatch()
{
    this->release();
}

void NormalizedPatch::extractNormalizedValues( Mat& image,  BoundingBox& bb, float* out_values)
{
    int size = NORMALIZED_PATCH_DIM;

    Mat patch = bb.extractPatch(image);

    Mat temp;//(size, size, DataType< unsigned char >::type );
    resize(patch, temp, Size(size, size));

    float mean = 0.0f;

    for(int i = 0; i < size; i++ )
    {
        for(int j = 0; j < size; j++)
        {
            mean += temp.at< unsigned char >(i,j);
        }

    }

    mean /= size*size;

    for(int i = 0; i < size; i++ )
    {
        for(int j = 0; j < size; j++)
        {
            out_values[i*size + j] = (temp.at< unsigned char >(i,j) - mean);
        }
    }

}

float NormalizedPatch::ncc( NormalizedPatch& other)
{
    float *f1 = this->values;
    float *f2 = other.values;

    double corr = 0;
	double norm1 = 0;
	double norm2 = 0;

	int size = NORMALIZED_PATCH_DIM*NORMALIZED_PATCH_DIM;

	for (int i = 0; i<size; i++) {
		corr += f1[i]*f2[i];
		norm1 += f1[i]*f1[i];
		norm2 += f2[i]*f2[i];
	}
	// normalization to <0,1>

	return (corr / sqrt(norm1*norm2) + 1) / 2.0;
}

}
