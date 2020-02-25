#include <vx_ext_rpp.h>
#include <VX/vx_compatibility.h>
#include <graph.h>
#include "node_hue.h"
#include "exception.h"


HueNode::HueNode(const std::vector<Image*>& inputs, const std::vector<Image*>& outputs):
        Node(inputs, outputs),
        _hue(HUE_RANGE[0], HUE_RANGE[1])
{
}

void HueNode::create(std::shared_ptr<Graph> graph)
{
    vx_status width_status, height_status, Hue_status;
    if(_node)
        return;

    _graph = graph;

    if(_outputs.empty() || _inputs.empty())
        THROW("Uninitialized input/output arguments")

    _width.resize(_outputs[0]->info().batch_size());// = (vx_uint32* ) malloc(sizeof(vx_uint32) * _outputs[0]->info().batch_size());
    _height.resize(_outputs[0]->info().batch_size());// = (vx_uint32* ) malloc(sizeof(vx_uint32) * _outputs[0]->info().batch_size());
    _width_array = vxCreateArray(vxGetContext((vx_reference)_graph->get()), VX_TYPE_UINT32, _outputs[0]->info().batch_size());
    _height_array = vxCreateArray(vxGetContext((vx_reference)_graph->get()), VX_TYPE_UINT32, _outputs[0]->info().batch_size());
    width_status = vxAddArrayItems(_width_array,_outputs[0]->info().batch_size(), _width.data(), sizeof(vx_uint32));
    height_status = vxAddArrayItems(_height_array,_inputs[0]->info().batch_size(), _height.data(), sizeof(vx_uint32));
    if(width_status != 0 || height_status != 0)
        THROW(" vxAddArrayItems failed in the Hue (vxExtrppNode_HueCorrectionbatchPD) node: "+ TOSTR(width_status) + "  "+ TOSTR(height_status))
    _hue.set_batch_size(_inputs[0]->info().batch_size());
    _hue.create_array(graph ,VX_TYPE_FLOAT32);
    _node = vxExtrppNode_HuebatchPD(_graph->get(), _inputs[0]->handle(), _width_array, _height_array, _outputs[0]->handle(), _hue.default_array(), _outputs[0]->info().batch_size());
    std::cout<< "Shobana here" << std::endl;
    vx_status status;
    if((status = vxGetStatus((vx_reference)_node)) != VX_SUCCESS)
        THROW("Adding the Hue (vxExtrppNode_HueCorrectionbatchPD) node failed: "+ TOSTR(status))

}
void HueNode::update_dimensions()
{
    std::vector<uint> width, height;

    width.resize( _inputs[0]->info().batch_size());
    height.resize( _inputs[0]->info().batch_size());
    for (int i = 0; i < _inputs[0]->info().batch_size(); i++ )
    {
        //std::cerr << " width [i] " << _width[i] << "\t height [i] " << _height[i] << std::endl;
        _width[i] = _inputs[0]->info().get_image_width(i);
        _height[i] = _inputs[0]->info().get_image_height(i);
     }

    vx_status width_status, height_status;
    width_status = vxCopyArrayRange((vx_array)_width_array, 0, _outputs[0]->info().batch_size(), sizeof(vx_uint32), _width.data(), VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST); //vxAddArrayItems(_width_array,_outputs[0]->info().batch_size(), _width, sizeof(vx_uint32));
    height_status = vxCopyArrayRange((vx_array)_height_array, 0, _outputs[0]->info().batch_size(), sizeof(vx_uint32), _height.data(), VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST); //vxAddArrayItems(_height_array,_inputs[0]->info().batch_size(), _height, sizeof(vx_uint32));
    if(width_status != 0 || height_status != 0)
        THROW(" vxCopyArrayRange failed in the Hue (vxExtrppNode_HueCorrectionbatchPD) node: "+ TOSTR(width_status) + "  "+ TOSTR(height_status))
    // TODO: Check the status codes
}

void HueNode::init(float hue)
{
    _hue.set_param(hue);
}

void HueNode::init(FloatParam* hue)
{
    _hue.set_param(core(hue));
}

void HueNode::update_parameters()
{
    update_dimensions();
     _hue.update_array();
}