#include "ResourcePoolConfig.h"

//It's meaningless to make a pool if resources are easy to get.
//Thus, the number should be limited
const int pool::ResourcePoolConfig::MAX_POOL_SIZE = 4096;
const int pool::ResourcePoolConfig::MIN_POOL_SIZE = 1;

//Cannot be larger than MAX_POOL_SIZE
const int pool::ResourcePoolConfig::MAX_INIT_POOL_SIZE = MAX_POOL_SIZE;
const int pool::ResourcePoolConfig::MIN_INIT_POOL_SIZE = 0;

//10 Minutes
const long pool::ResourcePoolConfig::MAX_REFURBISH_INTERVAL = 600000L;
//10 Seconds
const long pool::ResourcePoolConfig::MIN_REFURBISH_INTERVAL = 10000L;

//10 Minutes
const long pool::ResourcePoolConfig::MAX_SHRINK_INTERVAL = 600000L;
//10 Seconds
const long pool::ResourcePoolConfig::MIN_SHRINK_INTERVAL = 10000L;

//10 Minutes
const long pool::ResourcePoolConfig::MAX_RECLAIM_INTERVAL = 600000L;
//10 Seconds
const long pool::ResourcePoolConfig::MIN_RECLAIM_INTERVAL = 10000L;

pool::ResourcePoolConfig::ResourcePoolConfig()
{
    _poolSize = 1024;
    _initPoolSize = MIN_POOL_SIZE;
    _refurbishInterval = MAX_REFURBISH_INTERVAL;
    _shrinkInterval = MAX_SHRINK_INTERVAL;
    _reclaimInterval = MAX_RECLAIM_INTERVAL;
}

pool::ResourcePoolConfig::~ResourcePoolConfig()
{
    //Do nothing
}

int pool::ResourcePoolConfig::getPoolSize()
{
    return _poolSize;
}

void pool::ResourcePoolConfig::setPoolSize(int poolSize)
{
    _poolSize = fixValue(poolSize, MIN_POOL_SIZE, MAX_POOL_SIZE);
}

int pool::ResourcePoolConfig::getInitPoolSize()
{
    return _initPoolSize;
}

void pool::ResourcePoolConfig::setInitPoolSize(int initPoolSize)
{
    _initPoolSize = fixValue(initPoolSize, MIN_INIT_POOL_SIZE, MAX_INIT_POOL_SIZE);
}

long pool::ResourcePoolConfig::getRefurbishInterval()
{
    return _refurbishInterval;
}

void pool::ResourcePoolConfig::setRefurbishInterval(long refurbishInterval)
{
    _refurbishInterval = fixValue(refurbishInterval, MIN_REFURBISH_INTERVAL, MAX_REFURBISH_INTERVAL);
}

long pool::ResourcePoolConfig::getShrinkInterval()
{
    return _shrinkInterval;
}

void pool::ResourcePoolConfig::setShrinkInterval(long shrinkInterval)
{
    _shrinkInterval = fixValue(shrinkInterval, MIN_SHRINK_INTERVAL, MAX_SHRINK_INTERVAL);
}

long pool::ResourcePoolConfig::getReclaimInterval()
{
    return _reclaimInterval;
}

void pool::ResourcePoolConfig::setReclaimInterval(long reclaimInterval)
{
    _reclaimInterval = fixValue(reclaimInterval, MIN_RECLAIM_INTERVAL, MAX_RECLAIM_INTERVAL);
}
