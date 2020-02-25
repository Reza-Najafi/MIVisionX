
#include <cmath>
#include <VX/vx.h>
#include <VX/vx_compatibility.h>
#include <graph.h>
#include "parameter_crop_resize.h"
#include "commons.h"


void RandomCropResizeParam::set_area_coeff(Parameter<float>* area)
{
    if(!area)
        return;

    ParameterFactory::instance()->destroy_param(area_coeff);
    area_coeff = area;
}
void RandomCropResizeParam::set_x_drift(Parameter<float>* x_drift)
{
    if(!x_drift)
        return;

    ParameterFactory::instance()->destroy_param(x_center_drift);
    x_center_drift = x_drift;
}
void RandomCropResizeParam::set_y_drift(Parameter<float>* y_drift)
{
    if(!y_drift)
        return;

    ParameterFactory::instance()->destroy_param(y_center_drift);
    y_center_drift = y_drift;
}
 void RandomCropResizeParam::set_batch_size(unsigned int bs)
 {
     batch_size = bs;
 }

void RandomCropResizeParam::create_array(std::shared_ptr<Graph> graph,unsigned int bs)
{
    set_batch_size(bs);
    x1_arr_val = (size_t*)malloc(sizeof(size_t)*batch_size);
    y1_arr_val = (size_t*)malloc(sizeof(size_t)*batch_size);
    x2_arr_val = (size_t*)malloc(sizeof(size_t)*batch_size);
    y2_arr_val = (size_t*)malloc(sizeof(size_t)*batch_size);
    x1_arr = vxCreateArray(vxGetContext((vx_reference)graph->get()), VX_TYPE_UINT64,batch_size);
    y1_arr = vxCreateArray(vxGetContext((vx_reference)graph->get()), VX_TYPE_UINT64,batch_size);
    x2_arr = vxCreateArray(vxGetContext((vx_reference)graph->get()), VX_TYPE_UINT64,batch_size);
    y2_arr = vxCreateArray(vxGetContext((vx_reference)graph->get()), VX_TYPE_UINT64,batch_size);
    vxAddArrayItems(x1_arr,batch_size, x1_arr_val, sizeof(size_t));
    vxAddArrayItems(y1_arr,batch_size, y1_arr_val, sizeof(size_t));
    vxAddArrayItems(x2_arr,batch_size, x2_arr_val, sizeof(size_t));
    vxAddArrayItems(y2_arr,batch_size, y2_arr_val, sizeof(size_t));
    update_array();
}

void RandomCropResizeParam::update_array()
{
    vx_status status = VX_SUCCESS;
    for (uint i = 0; i < batch_size; i++)
    {
        area_coeff->renew();
        float area = area_coeff->get();
        x_center_drift->renew();
        float x_center = x_center_drift->get();
        y_center_drift->renew();
        y_center_drift->renew();
        float y_center = y_center_drift->get();
        calculate_area(
            area,
            x_center,
            y_center);
        x1_arr_val[i] = x1;
        y1_arr_val[i] = y1;
        x2_arr_val[i] = x2;
        y2_arr_val[i] = y2;
    }
    status = vxCopyArrayRange((vx_array)x1_arr, 0, batch_size, sizeof(size_t), x1_arr_val, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    if(status != VX_SUCCESS)
        WRN("ERROR: vxCopyArrayRange x1_arr failed " +TOSTR(status));
    status = vxCopyArrayRange((vx_array)y1_arr, 0, batch_size, sizeof(size_t), y1_arr_val, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    if(status != VX_SUCCESS)
        WRN("ERROR: vxCopyArrayRange x1_arr failed " +TOSTR(status));
    status = vxCopyArrayRange((vx_array)x2_arr, 0, batch_size, sizeof(size_t), x2_arr_val, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    if(status != VX_SUCCESS)
        WRN("ERROR: vxCopyArrayRange x1_arr failed " +TOSTR(status));
    status = vxCopyArrayRange((vx_array)y2_arr, 0, batch_size, sizeof(size_t), y2_arr_val, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    if(status != VX_SUCCESS)
        WRN("ERROR: vxCopyArrayRange x1_arr failed " +TOSTR(status));
}

void RandomCropResizeParam::update_array_for_cmn() // For crop mirro normalize
{
    vx_status status = VX_SUCCESS;
    for (uint i = 0; i < batch_size; i++)
    {
        area_coeff->renew();
        float area = area_coeff->get();
        x_center_drift->renew();
        float x_center = x_center_drift->get();
        y_center_drift->renew();
        y_center_drift->renew();
        float y_center = y_center_drift->get();
        calculate_area(
            area,
            x_center,
            y_center);
        x1_arr_val[i] = x1;
        y1_arr_val[i] = y1;
        x2_arr_val[i] = x2 - x1 ;
        y2_arr_val[i] = y2 - y1 ;

    }
    status = vxCopyArrayRange((vx_array)x1_arr, 0, batch_size, sizeof(size_t), x1_arr_val, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    if(status != VX_SUCCESS)
        WRN("ERROR: vxCopyArrayRange x1_arr failed " +TOSTR(status));
    status = vxCopyArrayRange((vx_array)y1_arr, 0, batch_size, sizeof(size_t), y1_arr_val, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    if(status != VX_SUCCESS)
        WRN("ERROR: vxCopyArrayRange x1_arr failed " +TOSTR(status));
    status = vxCopyArrayRange((vx_array)x2_arr, 0, batch_size, sizeof(size_t), x2_arr_val, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    if(status != VX_SUCCESS)
        WRN("ERROR: vxCopyArrayRange x1_arr failed " +TOSTR(status));
    status = vxCopyArrayRange((vx_array)y2_arr, 0, batch_size, sizeof(size_t), y2_arr_val, VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    if(status != VX_SUCCESS)
        WRN("ERROR: vxCopyArrayRange x1_arr failed " +TOSTR(status));
}


void RandomCropResizeParam::calculate_area(float area_coeff_, float x_center_drift_, float y_center_drift_)
{

// +-----------------------------------------> X direction
// |  ___________________________________
// |  |   (x1,y1)      |                |
// |  |    +-----------|-----------+    |
// |  |    |           |           |    |
// |  -----------------o-----------------
// |  |    |           |           |    |
// |  |    +-----------|-----------+    |
// |  |                |        (x2,y2) |
// |  +++++++++++++++++++++++++++++++++++
// |
// V Y directoin

    auto bound = [](float arg, float min , float max)
    {
        if( arg < min)
            return min;
        if( arg > max)
            return max;
        return arg;
    };

    auto y_center = in_height / 2;
    auto x_center = in_width / 2;


    float length_coeff = std::sqrt(bound(area_coeff_, RandomCropResizeParam::MIN_RANDOM_AREA_COEFF, 1.0));

    auto cropped_width = (size_t)(length_coeff  * (float)in_width);
    auto cropped_height= (size_t)(length_coeff * (float)in_height);

    size_t y_max_drift = (in_height - cropped_height) / 2;
    size_t x_max_drift = (in_width - cropped_width ) / 2;


    size_t no_drift_y1 = y_center - cropped_height/2;
    size_t no_drift_x1 = x_center - cropped_width/2;


    float x_drift_coeff = bound(x_center_drift_, -1.0, 1.0);// in [-1 1] range
    float y_drift_coeff = bound(y_center_drift_, -1.0, 1.0);// in [-1 1] range


    x1 = (size_t)((float)no_drift_x1 + x_drift_coeff * (float)x_max_drift);
    y1 = (size_t)((float)no_drift_y1 + y_drift_coeff * (float)y_max_drift);
    x2 = x1 + cropped_width;
    y2 = y1 + cropped_height;

    auto check_bound = [](int arg, int min , int max)
    {
        return arg < min || arg > max;
    };

    if(check_bound(x1, 0, in_width) || check_bound(x2, 0, in_width) || check_bound(y1, 0, in_height) || check_bound(y2, 0, in_height))
        // TODO: proper action required here
        WRN("Wrong crop area calculation")
}


Parameter<float> *RandomCropResizeParam::default_area()
{
    return ParameterFactory::instance()->create_uniform_float_rand_param(CROP_AREA_RANGE[0],
                                                                         CROP_AREA_RANGE[1])->core;
}

Parameter<float> *RandomCropResizeParam::default_x_drift()
{
    return ParameterFactory::instance()->create_uniform_float_rand_param(CROP_AREA_X_DRIFT_RANGE[0],
                                                                         CROP_AREA_X_DRIFT_RANGE[1])->core;
}

Parameter<float> *RandomCropResizeParam::default_y_drift()
{
    return ParameterFactory::instance()->create_uniform_float_rand_param(CROP_AREA_Y_DRIFT_RANGE[0],
                                                                         CROP_AREA_Y_DRIFT_RANGE[1])->core;
}