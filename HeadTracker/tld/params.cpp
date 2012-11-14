#include "params.h"

namespace tld
{

Params::Params(string paramsFile)
{
    if( paramsFile != "none")
    {
        FILE* in = fopen(paramsFile.c_str(),"r");
        char comment[500];


        fgets(comment,500, in);
        fscanf(in, "#FRAME_W=%d\n",&this->frame_w);
        fscanf(in, "#FRAME_H=%d\n",&this->frame_h);
        fscanf(in, "#FPS=%d\n",&this->fps);

        fgets(comment,500, in);
        fscanf(in, "#N_FERNS=%d\n", &this->n_ferns);
        fscanf(in, "#N_FEATURES=%d\n", &this->n_features);
        fscanf(in, "#MIN_SCALE=%f\n", &this->min_scale);
        fscanf(in, "#DECISION_THRESHOLD=%f\n", &this->decision_threshold);

        fgets(comment,500, in);
        fscanf(in, "#MIN_POSITIVE_OVERLAP=%f\n", &this->min_positive_overlap);
        fscanf(in, "#MAX_NEGATIVE_OVERLAP=%f\n", &this->max_negative_overlap);

        fgets(comment,500, in);
        fscanf(in, "#CUTOFF=%f\n", &this->cutoff);

        fgets(comment,500, in);
        fscanf(in, "#NORMALIZED_PATCH_DIM=%d\n", &this->normalized_patch_dim);

        fgets(comment,500, in);
        fscanf(in, "#MIN_BB=%d\n", &this->min_bb);
        fscanf(in, "#MIN_WINDOW_SCALE=%d\n", &this->min_window_scale);
        fscanf(in, "#MAX_WINDOW_SCALE=%d\n", &this->max_window_scale);
        fscanf(in, "#BASE_WINDOW_SCALE=%f\n", &this->base_window_scale);
        fscanf(in, "#SHIFT=%f\n", &this->shift);

        fgets(comment,500, in);
        fscanf(in, "#THETA_TP=%f\n", &this->theta_tp);
        fscanf(in, "#THETA_FP=%f", &this->theta_fp);


        fclose(in);







    }
    else
    {
        this->frame_w = FRAME_W;
        this->frame_h = FRAME_H;

        this->n_ferns = N_FERNS;
        this->n_features = N_FEATURES;
        this->min_scale = MIN_SCALE;
        this->max_scale = MAX_SCALE;
        this->decision_threshold = DECISION_THRESHOLD;

        this->min_positive_overlap = MIN_POSITIVE_OVERLAP;
        this->max_negative_overlap = MAX_NEGATIVE_OVERLAP;

        this->cutoff = CUTOFF;

        this->normalized_patch_dim = NORMALIZED_PATCH_DIM;

        this->min_bb = MIN_BB;
        this->min_window_scale = MIN_WINDOW_SCALE;
        this->max_window_scale = MAX_WINDOW_SCALE;
        this->base_window_scale = BASE_WINDOW_SCALE;
        this->shift = SHIFT;

        this->theta_tp = THETA_TP;
        this->theta_fp = THETA_FP;



    }

}

Params::~Params()
{
    //dtor
}

void Params::save(string paramsFile)
{
    FILE* out = fopen(paramsFile.c_str(), "w");

    fprintf(out, "--------------SAVED PARAMS!---------------------\n\n");
	fprintf(out, "#FRAME_W=%d\n",this->frame_w);
    fprintf(out, "#FRAME_H=%d\n",this->frame_h);

    fprintf(out, "#N_FERNS=%d\n", this->n_ferns);
    fprintf(out, "#N_FEATURES=%d\n", this->n_features);
    fprintf(out, "#MIN_SCALE=%f\n", this->min_scale);
    fprintf(out, "#DECISION_THRESHOLD=%f\n", this->decision_threshold);

    fprintf(out, "#MIN_POSITIVE_OVERLAP=%f\n", this->min_positive_overlap);
    fprintf(out, "#MAX_NEGATIVE_OVERLAP=%f\n", this->max_negative_overlap);

    fprintf(out, "#CUTOFF=%f\n", this->cutoff);

    fprintf(out, "#NORMALIZED_PATCH_DIM=%d\n", this->normalized_patch_dim);

    fprintf(out, "#MIN_BB=%d\n", this->min_bb);
    fprintf(out, "#MIN_WINDOW_SCALE=%d\n", this->min_window_scale);
    fprintf(out, "#MAX_WINDOW_SCALE=%d\n", this->max_window_scale);
    fprintf(out, "#BASE_WINDOW_SCALE=%d\n", this->base_window_scale);
    fprintf(out, "#SHIFT=%f\n", this->shift);

    fprintf(out, "#THETA_TP=%f\n", this->theta_tp);
    fprintf(out, "#THETA_FP=%f\n", this->theta_fp);

    fclose(out);
}

}
