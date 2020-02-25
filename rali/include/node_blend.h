#pragma once
#include "node.h"
#include "parameter_factory.h"
#include "parameter_vx.h"

class BlendNode : public Node
{
public:
    explicit BlendNode(const std::vector<Image*>& inputs, const std::vector<Image*>& outputs);
    BlendNode() = delete;
    void create(std::shared_ptr<Graph> graph) override;
    void init(float ratio);
    void init(FloatParam* ratio);
    void update_parameters() override;

private:
    ParameterVX<float> _ratio;
    constexpr static float RATIO_RANGE [2] = {0.1, 0.9};

    std::vector<vx_uint32> _width, _height;
    vx_array _width_array ,_height_array;
    void update_dimensions();
};