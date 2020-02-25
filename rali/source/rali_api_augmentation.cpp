
#include <node_warp_affine.h>
#include "node_exposure.h"
#include "node_vignette.h"
#include "node_jitter.h"
#include "node_snp_noise.h"
#include "node_snow.h"
#include "node_rain.h"
#include "node_color_temperature.h"
#include "node_fog.h"
#include "node_pixelate.h"
#include "node_lens_correction.h"
#include "node_gamma.h"
#include "node_flip.h"
#include "node_crop_resize.h"
#include "node_brightness.h"
#include "node_contrast.h"
#include "node_blur.h"
#include "node_fisheye.h"
#include "node_blend.h"
#include "node_resize.h"
#include "node_rotate.h"
#include "node_color_twist.h"
#include "node_hue.h"
#include "node_saturation.h"
#include "node_crop_mirror_normalize.h"
#include "node_copy.h"
#include "node_nop.h"

#include "commons.h"
#include "context.h"
#include "rali_api.h"



RaliImage  RALI_API_CALL
raliRotate(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliFloatParam angle,
        unsigned dest_width,
        unsigned dest_height)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        if(dest_width == 0 || dest_height == 0)
        {
            dest_width = input->info().width();
            dest_height = input->info().height_single();
        }
        // For the rotate node, user can create an image with a different width and height
        ImageInfo output_info = input->info();
        output_info.width(dest_width);
        output_info.height(dest_height);

        output = context->master_graph->create_image(output_info, is_output);

        // If the user has provided the output size the dimension of all the images after this node will be fixed and equal to that size
        if(dest_width != 0 && dest_height != 0)
            output->reset_image_dims();

        context->master_graph->add_node<RotateNode>({input}, {output})->init(angle);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage  RALI_API_CALL
raliRotateFixed(
        RaliContext context,
        RaliImage input,
        float angle,
        bool is_output,
        unsigned dest_width,
        unsigned dest_height)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        if(dest_width == 0 || dest_height == 0)
        {
            dest_width = input->info().width();
            dest_height = input->info().height_single();
        }
        // For the rotate node, user can create an image with a different width and height
        ImageInfo output_info = input->info();
        output_info.width(dest_width);
        output_info.height(dest_height);

        output = context->master_graph->create_image(output_info, is_output);

        // If the user has provided the output size the dimension of all the images after this node will be fixed and equal to that size
        if(dest_width != 0 && dest_height != 0)
            output->reset_image_dims();

        context->master_graph->add_node<RotateNode>({input}, {output})->init(angle);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliFlip(
        RaliContext context,
        RaliImage input,
        RaliFlipAxis axis,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<FlipNode>({input}, {output});
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}


RaliImage RALI_API_CALL
raliGamma(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliFloatParam alpha)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<GammaNode>({input}, {output})->init(alpha);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliGammaFixed(
        RaliContext context,
        RaliImage input,
        float alpha,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<GammaNode>({input}, {output})->init(alpha);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliHue(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliFloatParam hue)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<HueNode>({input}, {output})->init(hue);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliHueFixed(
        RaliContext context,
        RaliImage input,
        float hue,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<HueNode>({input}, {output})->init(hue);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliSaturation(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliFloatParam sat)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<SatNode>({input}, {output})->init(sat);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliSaturationFixed(
        RaliContext context,
        RaliImage input,
        float sat,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<SatNode>({input}, {output})->init(sat);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage  RALI_API_CALL
raliCropResize(
        RaliContext context,
        RaliImage input,
        unsigned dest_width, unsigned dest_height,
        bool is_output,
        RaliFloatParam area,
        RaliFloatParam x_center_drift,
        RaliFloatParam y_center_drift)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context || dest_width == 0 || dest_height == 0)
            THROW("Null values passed as input")

        // For the crop resize node, user can create an image with a different width and height
        ImageInfo output_info = input->info();
        output_info.width(dest_width);
        output_info.height(dest_height);

        output = context->master_graph->create_image(output_info, is_output);

        // For the nodes that user provides the output size the dimension of all the images after this node will be fixed and equal to that size
        output->reset_image_dims();

        context->master_graph->add_node<CropResizeNode>({input}, {output})->init(area, x_center_drift, y_center_drift);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}


RaliImage  RALI_API_CALL
raliCropResizeFixed(
        RaliContext context,
        RaliImage input,
        unsigned dest_width, unsigned dest_height,
        bool is_output,
        float area,
        float x_center_drift,
        float y_center_drift)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context || dest_width == 0 || dest_height == 0)
            THROW("Null values passed as input")

        // For the crop resize node, user can create an image with a different width and height
        ImageInfo output_info = input->info();
        output_info.width(dest_width);
        output_info.height(dest_height);

        output = context->master_graph->create_image(output_info, is_output);

        context->master_graph->add_node<CropResizeNode>({input}, {output})->init(area, x_center_drift, y_center_drift);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage  RALI_API_CALL
raliResize(
        RaliContext context,
        RaliImage input,
        unsigned dest_width,
        unsigned dest_height,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context || dest_width == 0 || dest_height == 0)
            THROW("Null values passed as input")

        // For the resize node, user can create an image with a different width and height
        ImageInfo output_info = input->info();
        output_info.width(dest_width);
        output_info.height(dest_height);

        output = context->master_graph->create_image(output_info, is_output);

        // For the nodes that user provides the output size the dimension of all the images after this node will be fixed and equal to that size
        output->reset_image_dims();

        context->master_graph->add_node<ResizeNode>({input}, {output});
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliBrightness(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliFloatParam alpha,
        RaliIntParam beta)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<BrightnessNode>({input}, {output})->init(alpha, beta);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliBrightnessFixed(
        RaliContext context,
        RaliImage input,
        float alpha,
        int beta,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<BrightnessNode>({input}, {output})->init(alpha, beta);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliBlur(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliIntParam sdev)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<BlurNode>({input}, {output})->init(sdev);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliBlurFixed(
        RaliContext context,
        RaliImage input,
        int sdev,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<BlurNode>({input}, {output})->init(sdev);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage  RALI_API_CALL
raliBlend(
        RaliContext context,
        RaliImage input1,
        RaliImage input2,
        bool is_output,
        RaliFloatParam ratio)
{
    RaliImage output = nullptr;
    try
    {
        if(!input1 || !input2 || !context)
            THROW("Null values passed as input")

        if(!(input1->info() == input2->info()))
            THROW("Input images to the blend operation must have the same info")

        output = context->master_graph->create_image(input1->info(), is_output);

        context->master_graph->add_node<BlendNode>({input1, input2}, {output})->init(ratio);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage  RALI_API_CALL
raliBlendFixed(
        RaliContext context,
        RaliImage input1,
        RaliImage input2,
        float ratio,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input1 || !input2 || !context)
            THROW("Null values passed as input")

        if(!(input1->info() == input2->info()))
            THROW("Input images to the blend operation must have the same info")

        output = context->master_graph->create_image(input1->info(), is_output);

        context->master_graph->add_node<BlendNode>({input1, input2}, {output})->init(ratio);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}


RaliImage  RALI_API_CALL
raliWarpAffine(
        RaliContext context,
        RaliImage input,
        bool is_output,
        unsigned dest_height, unsigned dest_width,
        RaliFloatParam x0, RaliFloatParam x1,
        RaliFloatParam y0, RaliFloatParam y1,
        RaliFloatParam o0, RaliFloatParam o1)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        if(dest_width == 0 || dest_height == 0)
        {
            dest_width = input->info().width();
            dest_height = input->info().height_single();
        }
        // For the warp affine node, user can create an image with a different width and height
        ImageInfo output_info = input->info();
        output_info.width(dest_width);
        output_info.height(dest_height);

        output = context->master_graph->create_image(output_info, is_output);

        // If the user has provided the output size the dimension of all the images after this node will be fixed and equal to that size
        if(dest_width != 0 && dest_height != 0)
            output->reset_image_dims();

        context->master_graph->add_node<WarpAffineNode>({input}, {output})->init(x0, x1, y0, y1, o0, o1);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage  RALI_API_CALL
raliWarpAffineFixed(
        RaliContext context,
        RaliImage input,
        float x0, float x1,
        float y0, float y1,
        float o0, float o1,
        bool is_output,
        unsigned int dest_height,
        unsigned int dest_width)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        if(dest_width == 0 || dest_height == 0)
        {
            dest_width = input->info().width();
            dest_height = input->info().height_single();
        }
        // For the warp affine node, user can create an image with a different width and height
        ImageInfo output_info = input->info();
        output_info.width(dest_width);
        output_info.height(dest_height);

        output = context->master_graph->create_image(input->info(), is_output);

        // If the user has provided the output size the dimension of all the images after this node will be fixed and equal to that size
        if(dest_width != 0 && dest_height != 0)
            output->reset_image_dims();

        context->master_graph->add_node<WarpAffineNode>({input}, {output})->init(x0, x1, y0, y1, o0, o1);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage  RALI_API_CALL
raliFishEye(
        RaliContext context,
        RaliImage input,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<FisheyeNode>({input}, {output});
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliVignette(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliFloatParam sdev)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<VignetteNode>({input}, {output})->init(sdev);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliVignetteFixed(
        RaliContext context,
        RaliImage input,
        float sdev,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<VignetteNode>({input}, {output})->init(sdev);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliJitter(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliIntParam kernel_size)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<JitterNode>({input}, {output})->init(kernel_size);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliJitterFixed(
        RaliContext context,
        RaliImage input,
        int kernel_size,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<JitterNode>({input}, {output})->init(kernel_size);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}


RaliImage RALI_API_CALL
raliSnPNoise(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliFloatParam sdev)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<SnPNoiseNode>({input}, {output})->init(sdev);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliSnPNoiseFixed(
        RaliContext context,
        RaliImage input,
        float sdev,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<SnPNoiseNode>({input}, {output})->init(sdev);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliFlip(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliIntParam flip_axis)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<FlipNode>({input}, {output})->init(flip_axis);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliFlipFixed(
        RaliContext context,
        RaliImage input,
        int flip_axis,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<FlipNode>({input}, {output})->init(flip_axis);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}


RaliImage RALI_API_CALL
raliContrast(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliIntParam min,
        RaliIntParam max)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<RaliContrastNode>({input}, {output})->init(min, max);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliContrastFixed(
        RaliContext context,
        RaliImage input,
        unsigned min,
        unsigned max,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<RaliContrastNode>({input}, {output})->init(min, max);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliSnow(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliFloatParam shift)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<SnowNode>({input}, {output})->init(shift);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliSnowFixed(
        RaliContext context,
        RaliImage input,
        float shift,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<SnowNode>({input}, {output})->init(shift);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliRain(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliFloatParam rain_value,
        RaliIntParam rain_width,
        RaliIntParam rain_height,
        RaliFloatParam rain_transparency)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<RainNode>({input}, {output})->init(rain_value, rain_width, rain_height, rain_transparency);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliRainFixed(
        RaliContext context,
        RaliImage input,
        float rain_value,
        int rain_width,
        int rain_height,
        float rain_transparency,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<RainNode>({input}, {output})->init(rain_value, rain_width, rain_height, rain_transparency);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliColorTemp(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliIntParam adj_value_param)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<ColorTemperatureNode>({input}, {output})->init(adj_value_param);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliColorTempFixed(
        RaliContext context,
        RaliImage input,
        int adj_value_param,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<ColorTemperatureNode>({input}, {output})->init(adj_value_param);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}


RaliImage RALI_API_CALL
raliFog(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliFloatParam fog_param)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<FogNode>({input}, {output})->init(fog_param);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliFogFixed(
        RaliContext context,
        RaliImage input,
        float fog_param,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<FogNode>({input}, {output})->init(fog_param);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage  RALI_API_CALL
raliPixelate(
        RaliContext context,
        RaliImage input,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<PixelateNode>({input}, {output});
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliLensCorrection(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliFloatParam strength,
        RaliFloatParam zoom)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<LensCorrectionNode>({input}, {output})->init(strength, zoom);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliLensCorrectionFixed(
        RaliContext context,
        RaliImage input,
        float strength,
        float zoom,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<LensCorrectionNode>({input}, {output})->init(strength, zoom);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliExposure(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliFloatParam shift)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<ExposureNode>({input}, {output})->init(shift);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliExposureFixed(
        RaliContext context,
        RaliImage input,
        float shift,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<ExposureNode>({input}, {output})->init(shift);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliColorTwist(
        RaliContext context,
        RaliImage input,
        bool is_output,
        RaliFloatParam alpha,
        RaliFloatParam beta,
        RaliFloatParam hue,
        RaliFloatParam sat)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<ColorTwistBatchNode>({input}, {output})->init(alpha, beta, hue, sat);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage RALI_API_CALL
raliColorTwistFixed(
        RaliContext context,
        RaliImage input,
        float alpha,
        float beta,
        float hue,
        float sat,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<ColorTwistBatchNode>({input}, {output})->init(alpha, beta, hue, sat);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage  RALI_API_CALL
raliCropMirrorNormalize(
        RaliContext context,
        RaliImage input,
        unsigned dest_width, unsigned dest_height,
        bool is_output)/*,
        RaliFloatParam mean,
        RaliFloatParam sdev,
        RaliIntParam mirror)*/
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context || dest_width == 0 || dest_height == 0)
            THROW("Null values passed as input")

        // For the crop resize node, user can create an image with a different width and height
        ImageInfo output_info = input->info();
        output_info.width(dest_width);
        output_info.height(dest_height);

        output = context->master_graph->create_image(output_info, is_output);

        // For the nodes that user provides the output size the dimension of all the images after this node will be fixed and equal to that size
        output->reset_image_dims();

        context->master_graph->add_node<CropMirrorNormalizeNode>({input}, {output});//->init(mean, sdev, mirror);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

/*RaliImage  RALI_API_CALL
raliCropMirrorNormalizeFixed(
        RaliContext context,
        RaliImage input,
        unsigned dest_width, unsigned dest_height,
        float mean, float sdev, int mirror,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context || dest_width == 0 || dest_height == 0)
            THROW("Null values passed as input")

        // For the crop resize node, user can create an image with a different width and height
        ImageInfo output_info = input->info();
        output_info.width(dest_width);
        output_info.height(dest_height);

        output = context->master_graph->create_image(output_info, is_output);

        // For the nodes that user provides the output size the dimension of all the images after this node will be fixed and equal to that size
        output->reset_image_dims();

        context->master_graph->add_node<CropResizeNode>({input}, {output})->init(mean, sdev, mirror);
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}*/

RaliImage  RALI_API_CALL
raliCopy(
        RaliContext context,
        RaliImage input,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<CopyNode>({input}, {output});
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}

RaliImage  RALI_API_CALL
raliNop(
        RaliContext context,
        RaliImage input,
        bool is_output)
{
    RaliImage output = nullptr;
    try
    {
        if(!input || !context)
            THROW("Null values passed as input")

        output = context->master_graph->create_image(input->info(), is_output);

        context->master_graph->add_node<NopNode>({input}, {output});
    }
    catch(const std::exception& e)
    {
        context->capture_error(e.what());
        ERR(e.what())
    }
    return output;
}