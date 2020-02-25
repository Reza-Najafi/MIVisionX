#include <vx_ext_rpp.h>
#include <node_pixelate.h>
#include <graph.h>
#include "exception.h"

PixelateNode::PixelateNode(const std::vector<Image*>& inputs, const std::vector<Image*>& outputs):
        Node(inputs, outputs)
{
}

void PixelateNode::create(std::shared_ptr<Graph> graph)
{
    
    _width.resize(_outputs[0]->info().batch_size());
    _height.resize(_outputs[0]->info().batch_size());

    for (int i=0; i < _outputs[0]->info().batch_size(); i++ ) {
         _width[i] = _outputs[0]->info().width();
         _height[i] = _outputs[0]->info().height_single();
    }

    vx_status width_status, height_status;

    if(_node)
        return;

    _graph = graph;

    if(_outputs.empty() || _inputs.empty())
        THROW("Uninitialized input/output arguments")

    _width_array = vxCreateArray(vxGetContext((vx_reference)_graph->get()), VX_TYPE_UINT32, _outputs[0]->info().batch_size());
    _height_array = vxCreateArray(vxGetContext((vx_reference)_graph->get()), VX_TYPE_UINT32, _outputs[0]->info().batch_size());

    width_status = vxAddArrayItems(_width_array,_outputs[0]->info().batch_size(), _width.data(), sizeof(vx_uint32));
    height_status = vxAddArrayItems(_height_array,_outputs[0]->info().batch_size(), _height.data(), sizeof(vx_uint32));

    if(width_status != 0 || height_status != 0)
        THROW(" vxAddArrayItems failed in the pixelate (vxExtrppNode_Pixelate)  node: "+ TOSTR(width_status) + "  "+ TOSTR(height_status))

    _node = vxExtrppNode_PixelatebatchPD(_graph->get(), _inputs[0]->handle(), _width_array, _height_array, _outputs[0]->handle(), _outputs[0]->info().batch_size());

    vx_status status;
    if((status = vxGetStatus((vx_reference)_node)) != VX_SUCCESS)
        THROW("Adding the pixelate (vxExtrppNode_Pixelate) node failed: "+ TOSTR(status))


}

void PixelateNode::update_dimensions()
{
    std::vector<uint> width, height;

    width.resize( _inputs[0]->info().batch_size());
    height.resize( _inputs[0]->info().batch_size());
    for (int i = 0; i < _inputs[0]->info().batch_size(); i++ )
    {
        _width[i] = _inputs[0]->info().get_image_width(i);
        _height[i] = _inputs[0]->info().get_image_height(i);
    }

    vx_status width_status, height_status;
    width_status = vxCopyArrayRange((vx_array)_width_array, 0, _outputs[0]->info().batch_size(), sizeof(vx_uint32), _width.data(), VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST); //vxAddArrayItems(__width_array,_outputs[0]->info().batch_size(), _width, sizeof(vx_uint32));
    height_status = vxCopyArrayRange((vx_array)_height_array, 0, _outputs[0]->info().batch_size(), sizeof(vx_uint32), _height.data(), VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST); //vxAddArrayItems(__height_array,_inputs[0]->info().batch_size(), _height, sizeof(vx_uint32));
    if(width_status != 0 || height_status != 0)
        THROW(" vxCopyArrayRange failed in the brightness_batch (vxExtrppNode_BrightnessbatchPD) node: "+ TOSTR(width_status) + "  "+ TOSTR(height_status))
    // TODO: Check the status codes
}

void PixelateNode::update_parameters()
{
    update_dimensions();
}

