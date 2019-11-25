#pragma once

#include <optix.h>
#include <vector>
#include <glm/glm.hpp>

struct IAS
{
    OptixTraversableHandle gas_handle ;   // geometry to be repeated
    OptixTraversableHandle ias_handle ; 
    CUdeviceptr            d_as_output_buffer;

    std::vector<OptixInstance> instances ; 

    OptixTraversableHandle handle;
    CUdeviceptr            d_instances ;

    void addInstance(const glm::mat4& mat);
    void dump(const float* imp); 

    IAS(OptixTraversableHandle gas_handle_);
 
    void init(); 
    void initInstancesOne();
    void initInstancesTwo();
    void initInstancesMany();

    void build(); 
    OptixTraversableHandle build(OptixBuildInput buildInput); 

};


