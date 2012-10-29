#include "feature.h"


namespace tld {

Feature::Feature()
{
    this->xp = this->yp = this->wp = this->hp = 0;
}

Feature::Feature(float minScale, float maxScale)
{
    this->wp = (maxScale - minScale)*randomFloat() + minScale;
    this->hp = (maxScale - minScale)*randomFloat() + minScale;

    this->xp = (1.0f - this->wp)*randomFloat();
    this->yp = (1.0f - this->hp)*randomFloat();
}

Feature::~Feature()
{
    //dtor
}


}
