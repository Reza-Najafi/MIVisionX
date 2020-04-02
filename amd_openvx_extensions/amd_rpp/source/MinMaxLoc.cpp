#include <kernels_rpp.h>
#include <vx_ext_rpp.h>
#include <stdio.h>
#include <iostream>
#include "internal_rpp.h"
#include "internal_publishKernels.h"
#include </opt/rocm/rpp/include/rpp.h>
#include </opt/rocm/rpp/include/rppdefs.h>
#include </opt/rocm/rpp/include/rppi.h>


struct MinMaxLocLocalData { 
	RPPCommonHandle handle;
	rppHandle_t rppHandle; 
	RppiSize srcDimensions; 
	Rpp32u device_type;
	RppPtr_t pSrc;
	Rpp8u min;
	Rpp8u max;
	Rpp32u minLoc;
	Rpp32u maxLoc;
#if ENABLE_OPENCL
	cl_mem cl_pSrc;
#endif 
};

static vx_status VX_CALLBACK refreshMinMaxLoc(vx_node node, const vx_reference *parameters, vx_uint32 num, MinMaxLocLocalData *data)
{
	vx_status status = VX_SUCCESS;
 	STATUS_ERROR_CHECK(vxQueryImage((vx_image)parameters[0], VX_IMAGE_HEIGHT, &data->srcDimensions.height, sizeof(data->srcDimensions.height)));
	STATUS_ERROR_CHECK(vxQueryImage((vx_image)parameters[0], VX_IMAGE_WIDTH, &data->srcDimensions.width, sizeof(data->srcDimensions.width)));
	STATUS_ERROR_CHECK(vxReadScalarValue((vx_scalar)parameters[1], &data->min));
	STATUS_ERROR_CHECK(vxReadScalarValue((vx_scalar)parameters[2], &data->max));
	STATUS_ERROR_CHECK(vxReadScalarValue((vx_scalar)parameters[3], &data->minLoc));
	STATUS_ERROR_CHECK(vxReadScalarValue((vx_scalar)parameters[4], &data->maxLoc));
	if(data->device_type == AGO_TARGET_AFFINITY_GPU) {
#if ENABLE_OPENCL
		STATUS_ERROR_CHECK(vxQueryImage((vx_image)parameters[0], VX_IMAGE_ATTRIBUTE_AMD_OPENCL_BUFFER, &data->cl_pSrc, sizeof(data->cl_pSrc)));
#endif
	}
	if(data->device_type == AGO_TARGET_AFFINITY_CPU) {
		STATUS_ERROR_CHECK(vxQueryImage((vx_image)parameters[0], VX_IMAGE_ATTRIBUTE_AMD_HOST_BUFFER, &data->pSrc, sizeof(vx_uint8)));
	}
	return status; 
}

static vx_status VX_CALLBACK validateMinMaxLoc(vx_node node, const vx_reference parameters[], vx_uint32 num, vx_meta_format metas[])
{
	vx_status status = VX_SUCCESS;
	vx_enum scalar_type;
	STATUS_ERROR_CHECK(vxQueryScalar((vx_scalar)parameters[1], VX_SCALAR_TYPE, &scalar_type, sizeof(scalar_type)));
 	if(scalar_type != VX_TYPE_UINT8) return ERRMSG(VX_ERROR_INVALID_TYPE, "validate: Paramter: #1 type=%d (must be size)\n", scalar_type);
	STATUS_ERROR_CHECK(vxQueryScalar((vx_scalar)parameters[2], VX_SCALAR_TYPE, &scalar_type, sizeof(scalar_type)));
 	if(scalar_type != VX_TYPE_UINT8) return ERRMSG(VX_ERROR_INVALID_TYPE, "validate: Paramter: #2 type=%d (must be size)\n", scalar_type);
	STATUS_ERROR_CHECK(vxQueryScalar((vx_scalar)parameters[3], VX_SCALAR_TYPE, &scalar_type, sizeof(scalar_type)));
 	if(scalar_type != VX_TYPE_UINT32) return ERRMSG(VX_ERROR_INVALID_TYPE, "validate: Paramter: #3 type=%d (must be size)\n", scalar_type);
	STATUS_ERROR_CHECK(vxQueryScalar((vx_scalar)parameters[4], VX_SCALAR_TYPE, &scalar_type, sizeof(scalar_type)));
 	if(scalar_type != VX_TYPE_UINT32) return ERRMSG(VX_ERROR_INVALID_TYPE, "validate: Paramter: #4 type=%d (must be size)\n", scalar_type);
	STATUS_ERROR_CHECK(vxQueryScalar((vx_scalar)parameters[5], VX_SCALAR_TYPE, &scalar_type, sizeof(scalar_type)));
 	if(scalar_type != VX_TYPE_UINT32) return ERRMSG(VX_ERROR_INVALID_TYPE, "validate: Paramter: #5 type=%d (must be size)\n", scalar_type);
	// Check for input parameters 
	vx_parameter input_param; 
	vx_image input; 
	vx_df_image df_image;
	input_param = vxGetParameterByIndex(node,0);
	STATUS_ERROR_CHECK(vxQueryParameter(input_param, VX_PARAMETER_ATTRIBUTE_REF, &input, sizeof(vx_image)));
	STATUS_ERROR_CHECK(vxQueryImage(input, VX_IMAGE_ATTRIBUTE_FORMAT, &df_image, sizeof(df_image))); 
	if(df_image != VX_DF_IMAGE_U8 && df_image != VX_DF_IMAGE_RGB) 
	{
		return ERRMSG(VX_ERROR_INVALID_FORMAT, "validate: MinMaxLoc: image: #0 format=%4.4s (must be RGB2 or U008)\n", (char *)&df_image);
	}

	vxReleaseImage(&input);
	vxReleaseParameter(&input_param);
	return status;
}

static vx_status VX_CALLBACK processMinMaxLoc(vx_node node, const vx_reference * parameters, vx_uint32 num) 
{ 
	RppStatus status = RPP_SUCCESS;
	MinMaxLocLocalData * data = NULL;
	STATUS_ERROR_CHECK(vxQueryNode(node, VX_NODE_LOCAL_DATA_PTR, &data, sizeof(data)));
	vx_df_image df_image = VX_DF_IMAGE_VIRT;
	STATUS_ERROR_CHECK(vxQueryImage((vx_image)parameters[0], VX_IMAGE_ATTRIBUTE_FORMAT, &df_image, sizeof(df_image)));
	if(data->device_type == AGO_TARGET_AFFINITY_GPU) {
#if ENABLE_OPENCL
		cl_command_queue handle = data->handle.cmdq;
		refreshMinMaxLoc(node, parameters, num, data);
		if (df_image == VX_DF_IMAGE_U8 ){ 
 			// status = rppi_min_max_loc_u8_pln1_gpu((void *)data->cl_pSrc,data->srcDimensions,&data->min,&data->max,&data->minLoc,&data->maxLoc,data->rppHandle);
		}
		else if(df_image == VX_DF_IMAGE_RGB) {
			// status = rppi_min_max_loc_u8_pkd3_gpu((void *)data->cl_pSrc,data->srcDimensions,&data->min,&data->max,&data->minLoc,&data->maxLoc,data->rppHandle);
		}
		STATUS_ERROR_CHECK(vxWriteScalarValue((vx_scalar)parameters[1], &data->min));
		STATUS_ERROR_CHECK(vxWriteScalarValue((vx_scalar)parameters[2], &data->max));
		STATUS_ERROR_CHECK(vxWriteScalarValue((vx_scalar)parameters[3], &data->minLoc));
		STATUS_ERROR_CHECK(vxWriteScalarValue((vx_scalar)parameters[4], &data->maxLoc));
		return status;
#endif
	}
	if(data->device_type == AGO_TARGET_AFFINITY_CPU) {
		refreshMinMaxLoc(node, parameters, num, data);
		if (df_image == VX_DF_IMAGE_U8 ){
			// status = rppi_min_max_loc_u8_pln1_host(data->pSrc,data->srcDimensions,&data->min,&data->max,&data->minLoc,&data->maxLoc,data->rppHandle);
		}
		else if(df_image == VX_DF_IMAGE_RGB) {
			// status = rppi_min_max_loc_u8_pkd3_host(data->pSrc,data->srcDimensions,&data->min,&data->max,&data->minLoc,&data->maxLoc,data->rppHandle);
		}
		STATUS_ERROR_CHECK(vxWriteScalarValue((vx_scalar)parameters[1], &data->min));
		STATUS_ERROR_CHECK(vxWriteScalarValue((vx_scalar)parameters[2], &data->max));
		STATUS_ERROR_CHECK(vxWriteScalarValue((vx_scalar)parameters[3], &data->minLoc));
		STATUS_ERROR_CHECK(vxWriteScalarValue((vx_scalar)parameters[4], &data->maxLoc));
		return status;
	}
}

static vx_status VX_CALLBACK initializeMinMaxLoc(vx_node node, const vx_reference *parameters, vx_uint32 num) 
{
	MinMaxLocLocalData * data = new MinMaxLocLocalData;
	memset(data, 0, sizeof(*data));
#if ENABLE_OPENCL
	STATUS_ERROR_CHECK(vxQueryNode(node, VX_NODE_ATTRIBUTE_AMD_OPENCL_COMMAND_QUEUE, &data->handle.cmdq, sizeof(data->handle.cmdq)));
#endif
	STATUS_ERROR_CHECK(vxCopyScalar((vx_scalar)parameters[5], &data->device_type, VX_READ_ONLY, VX_MEMORY_TYPE_HOST));
	refreshMinMaxLoc(node, parameters, num, data);
#if ENABLE_OPENCL
	if(data->device_type == AGO_TARGET_AFFINITY_GPU)
		rppCreateWithStream(&data->rppHandle, data->handle.cmdq);
#endif
	if(data->device_type == AGO_TARGET_AFFINITY_CPU)
	rppCreateWithBatchSize(&data->rppHandle, 1);
	STATUS_ERROR_CHECK(vxSetNodeAttribute(node, VX_NODE_LOCAL_DATA_PTR, &data, sizeof(data)));
	return VX_SUCCESS;
}

static vx_status VX_CALLBACK uninitializeMinMaxLoc(vx_node node, const vx_reference *parameters, vx_uint32 num)
{
	MinMaxLocLocalData * data; 
	STATUS_ERROR_CHECK(vxQueryNode(node, VX_NODE_LOCAL_DATA_PTR, &data, sizeof(data)));
#if ENABLE_OPENCL
	if(data->device_type == AGO_TARGET_AFFINITY_GPU)
		rppDestroyGPU(data->rppHandle);
#endif
	if(data->device_type == AGO_TARGET_AFFINITY_CPU)
		rppDestroyHost(data->rppHandle);
	delete(data);
	return VX_SUCCESS; 
}

vx_status MinMaxLoc_Register(vx_context context)
{
	vx_status status = VX_SUCCESS;
	// Add kernel to the context with callbacks
	vx_kernel kernel = vxAddUserKernel(context, "org.rpp.MinMaxLoc",
		VX_KERNEL_RPP_MINMAXLOC,
		processMinMaxLoc,
		6,
		validateMinMaxLoc,
		initializeMinMaxLoc,
		uninitializeMinMaxLoc);
	ERROR_CHECK_OBJECT(kernel);
	AgoTargetAffinityInfo affinity;
	vxQueryContext(context, VX_CONTEXT_ATTRIBUTE_AMD_AFFINITY,&affinity, sizeof(affinity));
#if ENABLE_OPENCL
	// enable OpenCL buffer access since the kernel_f callback uses OpenCL buffers instead of host accessible buffers
	vx_bool enableBufferAccess = vx_true_e;
	if(affinity.device_type == AGO_TARGET_AFFINITY_GPU)
		STATUS_ERROR_CHECK(vxSetKernelAttribute(kernel, VX_KERNEL_ATTRIBUTE_AMD_OPENCL_BUFFER_ACCESS_ENABLE, &enableBufferAccess, sizeof(enableBufferAccess)));
#else
	vx_bool enableBufferAccess = vx_false_e;
#endif
	if (kernel)
	{
		PARAM_ERROR_CHECK(vxAddParameterToKernel(kernel, 0, VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED));
		PARAM_ERROR_CHECK(vxAddParameterToKernel(kernel, 1, VX_BIDIRECTIONAL, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED));
		PARAM_ERROR_CHECK(vxAddParameterToKernel(kernel, 2, VX_BIDIRECTIONAL, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED));
		PARAM_ERROR_CHECK(vxAddParameterToKernel(kernel, 3, VX_BIDIRECTIONAL, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED));
		PARAM_ERROR_CHECK(vxAddParameterToKernel(kernel, 4, VX_BIDIRECTIONAL, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED));
		PARAM_ERROR_CHECK(vxAddParameterToKernel(kernel, 5, VX_INPUT, VX_TYPE_SCALAR, VX_PARAMETER_STATE_REQUIRED));
		PARAM_ERROR_CHECK(vxFinalizeKernel(kernel));
	}
	if (status != VX_SUCCESS)
	{
	exit:	vxRemoveKernel(kernel);	return VX_FAILURE; 
 	}
	return status;
}
