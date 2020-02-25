#include <vx_ext_rpp.h>
#include <graph.h>
#include "node_crop_mirror_normalize.h"
#include "exception.h"

CropMirrorNormalizeNode::CropMirrorNormalizeNode(const std::vector<Image*>& inputs, const std::vector<Image*>& outputs):
        Node(inputs, outputs),
        _dest_width(_outputs[0]->info().width()),
        _dest_height(_outputs[0]->info().height_batch()),
        _mean(MEAN_RANGE[0], MEAN_RANGE[1]),
        _sdev(SDEV_RANGE[0], SDEV_RANGE[1]),
        _mirror(MIRROR_RANGE[0], SDEV_RANGE[1])
{
    _crop_param = std::make_shared<RandomCropResizeParam>(inputs[0]->info().width() , inputs[0]->info().height_single());
}

void CropMirrorNormalizeNode::create(std::shared_ptr<Graph> graph)
{
    if(_node)
        return;

    _graph = graph;

    if(_outputs.empty() || _inputs.empty())
        THROW("Uninitialized input/output arguments")

    if(_dest_width == 0 || _dest_height == 0)
        THROW("Uninitialized destination dimension")
    _crop_param->create_array(graph ,_inputs[0]->info().batch_size());
    src_width.resize(_outputs[0]->info().batch_size());
    src_height.resize(_outputs[0]->info().batch_size());
    dst_width.resize(_outputs[0]->info().batch_size());
    dst_height.resize(_outputs[0]->info().batch_size());
    for (uint i=0; i < _inputs[0]->info().batch_size(); i++ ) {
         src_width[i] = _inputs[0]->info().width();
         src_height[i] = _inputs[0]->info().height_single();
         dst_width[i] = _outputs[0]->info().width();
         dst_height[i] = _outputs[0]->info().height_single();
    }
    // std::cerr<<"\n dest width and height"<<_outputs[0]->info().width()<<"\t"<<_outputs[0]->info().height_single();
    src_width_array = vxCreateArray(vxGetContext((vx_reference)_graph->get()), VX_TYPE_UINT32, _inputs[0]->info().batch_size());
    src_height_array = vxCreateArray(vxGetContext((vx_reference)_graph->get()), VX_TYPE_UINT32, _inputs[0]->info().batch_size());
    dst_width_array = vxCreateArray(vxGetContext((vx_reference)_graph->get()), VX_TYPE_UINT32, _outputs[0]->info().batch_size());
    dst_height_array = vxCreateArray(vxGetContext((vx_reference)_graph->get()), VX_TYPE_UINT32, _outputs[0]->info().batch_size());
    vx_status width_status, height_status;
    width_status = vxAddArrayItems(src_width_array,_inputs[0]->info().batch_size(), src_width.data(), sizeof(vx_uint32));
    height_status = vxAddArrayItems(src_height_array,_inputs[0]->info().batch_size(), src_height.data(), sizeof(vx_uint32));
    if(width_status != 0 || height_status != 0)
        THROW(" vxAddArrayItems failed in the crop resize node (vxExtrppNode_CropMirrorNormalizeCropbatchPD    )  node: "+ TOSTR(width_status) + "  "+ TOSTR(height_status))
    width_status = vxAddArrayItems(dst_width_array,_outputs[0]->info().batch_size(), dst_width.data(), sizeof(vx_uint32));
    height_status = vxAddArrayItems(dst_height_array,_outputs[0]->info().batch_size(), dst_height.data(), sizeof(vx_uint32));
    if(width_status != 0 || height_status != 0)
        THROW(" vxAddArrayItems failed in the crop resize node (vxExtrppNode_CropMirrorNormalizeCropbatchPD    )  node: "+ TOSTR(width_status) + "  "+ TOSTR(height_status))
    vx_scalar  chnToggle;
    unsigned int chnShift = 0;
    _node = vxExtrppNode_CropMirrorNormalizebatchPD(_graph->get(), _inputs[0]->handle(), src_width_array, src_height_array, _outputs[0]->handle(), _crop_param->x2_arr,
                                        _crop_param->y2_arr, _crop_param->x1_arr, _crop_param->y1_arr, _mean.default_array(), _sdev.default_array(), _mirror.default_array() ,chnToggle ,_outputs[0]->info().batch_size());

    vx_status status;
    if((status = vxGetStatus((vx_reference)_node)) != VX_SUCCESS)
        THROW("Error adding the crop resize node (vxExtrppNode_CropMirrorNormalizeCropbatchPD    ) failed: "+TOSTR(status))
}

/*void CropMirrorNormalizeNode::init(float mean, float sdev, int mirror)
{
    _mean.set_param(mean);
    _sdev.set_param(sdev);
    _mirror.set_param(mirror);
}

void CropMirrorNormalizeNode::init(FloatParam* mean, FloatParam* sdev, IntParam* mirror)
{
    _mean.set_param(core(mean));
    _sdev.set_param(core(sdev));
    _mirror.set_param(core(mirror));
}*/

void CropMirrorNormalizeNode::update_dimensions()
{
    for (int i = 0; i < _inputs[0]->info().batch_size(); i++ )
    {
        src_width[i] = _inputs[0]->info().get_image_width(i);
        src_height[i] = _inputs[0]->info().get_image_height(i);
     }
    vx_status width_status, height_status;
    width_status = vxCopyArrayRange((vx_array)src_width_array, 0, _outputs[0]->info().batch_size(), sizeof(vx_uint32), src_width.data(), VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST); //vxAddArrayItems(_width_array,_outputs[0]->info().batch_size(), _width, sizeof(vx_uint32));
    height_status = vxCopyArrayRange((vx_array)src_height_array, 0, _outputs[0]->info().batch_size(), sizeof(vx_uint32), src_height.data(), VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST); //vxAddArrayItems(_height_array,_inputs[0]->info().batch_size(), _height, sizeof(vx_uint32));
    if(width_status != 0 || height_status != 0)
        THROW(" vxCopyArrayRange failed in the crop resize node (vxExtrppNode_CropMirrorNormalizeCropbatchPD    )  node: "+ TOSTR(width_status) + "  "+ TOSTR(height_status))
}

void CropMirrorNormalizeNode::update_parameters()
{
    update_dimensions();
    _crop_param->update_array_for_cmn();
}


