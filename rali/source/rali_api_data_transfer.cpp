#include "commons.h"
#include "context.h"
#include "rali_api.h"
#include "CL/cl.h"

RaliStatus RALI_API_CALL
raliCopyToOutputTensor(RaliContext rali_context, float *out_ptr, RaliTensorLayout tensor_format, float multiplier,
                       float offset)
{
    try
    {
        auto tensor_layout = (tensor_format == RALI_NHWC) ?  RaliTensorFormat::NHWC : RaliTensorFormat::NCHW;
        rali_context->master_graph->copy_out_tensor(out_ptr, tensor_layout, multiplier, offset);
    }
    catch(const std::exception& e)
    {
        rali_context->capture_error(e.what());
        ERR(e.what())
        return RALI_RUNTIME_ERROR;
    }
    return RALI_OK;
}

RaliStatus RALI_API_CALL
raliCopyToOutput(
        RaliContext rali_context,
        cl_mem out_ptr,
        size_t out_size)
{
    try
    {
        rali_context->master_graph->copy_output(out_ptr, out_size);
    }
    catch(const std::exception& e)
    {
        rali_context->capture_error(e.what());
        ERR(e.what())
        return RALI_RUNTIME_ERROR;
    }
    return RALI_OK;
}

RaliStatus RALI_API_CALL
raliCopyToOutput(
        RaliContext rali_context,
        unsigned char * out_ptr,
        size_t out_size)
{
    try
    {
        rali_context->master_graph->copy_output(out_ptr);
    }
    catch(const std::exception& e)
    {
        rali_context->capture_error(e.what());
        ERR(e.what())
        return RALI_RUNTIME_ERROR;
    }
    return RALI_OK;
}

