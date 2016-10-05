# CppResourcePool
A C++ resource pool implemented with Boost


## Basic usage
````C++

#include "ResourcePool.hpp"

using namespace std;
namespace cp = common_pool;

class Resource
{
public:
    int a;
    int b;

    int calc()
    {
        return a + b;
    };
};

int main()
{
    cp::ResourcePoolConfig config;
    config.setInitPoolSize(3);
    config.setPoolSize(50);
    cp::ResourcePool<Resource> *pool = new cp::ResourcePool<Resource>(config);


    cp::ResourceWrapper<Resource> *resource = pool->genResource();
    if (resource != NULL)
    {
        resource->get()->calc();
        delete resource;
    }

    delete pool;
    return 0;
}

`````