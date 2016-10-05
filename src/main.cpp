#include "ResourcePool.hpp"

using namespace std;


int main()
{
    common_pool::ResourcePoolConfig config;
    common_pool::ResourcePool<int> *pool = new common_pool::ResourcePool<int>(config);



    delete pool;
    return 0;
}