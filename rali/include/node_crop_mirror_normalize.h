#pragma once
#include "node.h"
#include "parameter_factory.h"
#include "parameter_vx.h"
#include "parameter_crop_resize.h"

class RandomCropResizeParam;

class CropMirrorNormalizeNode : public Node
{
public:
    explicit CropMirrorNormalizeNode(const std::vector<Image*>& inputs, const std::vector<Image*>& outputs);
    CropMirrorNormalizeNode() = delete;
    void create(std::shared_ptr<Graph> graph) override;
    // void init(float mean, float sdev, int mirror);
    // void init(FloatParam *mean, FloatParam *sdev, IntParam *mirror);
    void update_parameters() override;
private:

    size_t _dest_width;
    size_t _dest_height;
    std::shared_ptr<RandomCropResizeParam> _crop_param;

    ParameterVX<float> _mean;
    constexpr static float MEAN_RANGE [2] = {122.0, 130.0};//{0.0, 255.0};

    ParameterVX<float> _sdev;
    constexpr static float SDEV_RANGE [2] = {0.5, 3.5};//{1.0, 125.0};
    
    ParameterVX<int> _mirror;
    constexpr static int   MIRROR_RANGE [2] =  {0, 1};

    vx_array src_width_array,src_height_array ,dst_width_array ,dst_height_array;
    std::vector<vx_uint32> src_width ,src_height, dst_width, dst_height; 
    void update_dimensions();
};



