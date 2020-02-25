#pragma once
#include <VX/vx_types.h>
#include <VX/vx_compatibility.h>
#include <vector>
#include "parameter_factory.h"

template<typename T>
class ParameterVX
{
public:
    ParameterVX(unsigned ovx_param_idx, T default_range_start, T default_range_end):
            OVX_PARAM_IDX(ovx_param_idx),
            _DEFAULT_RANGE_START(default_range_start),
            _DEFAULT_RANGE_END(default_range_end)
    {
        _param = ParameterFactory::instance()->create_uniform_rand_param<T>(_DEFAULT_RANGE_START,
                                                                            _DEFAULT_RANGE_END);
    }
    ParameterVX( T default_range_start, T default_range_end):
            _DEFAULT_RANGE_START(default_range_start),
            _DEFAULT_RANGE_END(default_range_end)
    {
        _param = ParameterFactory::instance()->create_uniform_rand_param<T>(_DEFAULT_RANGE_START,
                                                                            _DEFAULT_RANGE_END);
    }
    void create(vx_node node)
    {
        vx_status status;
        auto ref = vxGetParameterByIndex(node, OVX_PARAM_IDX);
        if( (status = vxQueryParameter(ref,VX_PARAMETER_ATTRIBUTE_REF, &_scalar, sizeof(vx_scalar))) != VX_SUCCESS ||
            (status = vxGetStatus((vx_reference)node)) != VX_SUCCESS)
            THROW("Getting vx scalar from the vx node failed" + TOSTR(status));
        if( (status = vxReadScalarValue(_scalar, &_val)) != VX_SUCCESS)
            THROW("Reading vx scalar failed" + TOSTR(status));
    }
    void create_array(std::shared_ptr<Graph> graph, vx_enum data_type)
    {
        // _arrVal = (T*)malloc(sizeof(T) * _batch_size);
        _arrVal.resize(_batch_size);
        _array = vxCreateArray(vxGetContext((vx_reference)graph->get()), data_type,_batch_size);
        vxAddArrayItems(_array,_batch_size, _arrVal.data(), sizeof(T));
        update_array();
    }
    void set_batch_size(unsigned batch_size)
    {
        _batch_size = batch_size;
    }
    void set_param(Parameter<T>* param)
    {
        if(!param)
            return;

        ParameterFactory::instance()->destroy_param(_param);
        _param = param;
    }
    void set_param(T val)
    {
        ParameterFactory::instance()->destroy_param(_param);
        _param = ParameterFactory::instance()->create_single_value_param(val);
    }
    T default_value()
    {
        return _param->default_value();
    }
    vx_array default_array()
    {
        return _array;
    }
    vx_scalar default_scalar(std::shared_ptr<Graph> _graph, vx_enum data_type)
    {
        _scalar = vxCreateScalar(vxGetContext((vx_reference)_graph->get()), data_type, &_val);
        return _scalar;
    }
    T get()
    {
        return _val;
    }
    void update()
    {
        vx_status status;

        T val = _param->get();

        if(_val == val)
            return;

        if((status = vxWriteScalarValue(_scalar, &val))!= VX_SUCCESS)
            WRN("Updating vx scalar failed")

    }
    void update_array( )
    {
        vx_status status;
        for (uint i=0; i < _batch_size ; i++ )
        {
            _arrVal[i] = renew();
        }
        status = vxCopyArrayRange((vx_array)_array, 0, _batch_size, sizeof(T), _arrVal.data(), VX_WRITE_ONLY, VX_MEMORY_TYPE_HOST);
    }
    T renew()
    {
        _param->renew();
        return _param->get();
    }
private:
    vx_scalar _scalar;
    vx_array _array;
    Parameter<T>* _param;
    T _val;
    std::vector<T> _arrVal;
    unsigned _batch_size;
    unsigned OVX_PARAM_IDX;
    const T _DEFAULT_RANGE_START;
    const T _DEFAULT_RANGE_END;
};
