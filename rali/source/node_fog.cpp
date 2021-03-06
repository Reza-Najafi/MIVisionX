#include <vx_ext_rpp.h>
#include <graph.h>
#include "node_fog.h"
#include "exception.h"

FogNode::FogNode(const std::vector<Image *> &inputs, const std::vector<Image *> &outputs) :
        Node(inputs, outputs),
        _fog_param(FOG_VALUE_RANGE[0], FOG_VALUE_RANGE[1])
{
}

void FogNode::create_node()
{
    if(_node)
        return;

    _fog_param.create_array(_graph , VX_TYPE_FLOAT32, _batch_size);
    _node = vxExtrppNode_FogbatchPD(_graph->get(), _inputs[0]->handle(), _src_roi_width, _src_roi_height, _outputs[0]->handle(), _fog_param.default_array(), _batch_size);

    vx_status status;
    if((status = vxGetStatus((vx_reference)_node)) != VX_SUCCESS)
        THROW("Adding the fog (vxExtrppNode_Fog) node failed: "+ TOSTR(status))
}

void FogNode::init(float fog_param)
{
    _fog_param.set_param(fog_param);
}

void FogNode::init(FloatParam* fog_param)
{
    _fog_param.set_param(core(fog_param));
}

void FogNode::update_node()
{
    _fog_param.update_array();
}
