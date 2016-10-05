#include "ResourcePoolConfig.h"

//It's meaningless to make a pool if resources are easy to get.
//Thus, the number should be limited
const int common_pool::ResourcePoolConfig::MAX_POOL_SIZE = 4096;
const int common_pool::ResourcePoolConfig::MIN_POOL_SIZE = 1;

//Cannot be larger than MAX_POOL_SIZE
const int common_pool::ResourcePoolConfig::MAX_INIT_POOL_SIZE = MAX_POOL_SIZE;
const int common_pool::ResourcePoolConfig::MIN_INIT_POOL_SIZE = 0;

//10 Minutes
const long common_pool::ResourcePoolConfig::MAX_REFURBISH_INTERVAL = 600000L;
//10 Seconds
const long common_pool::ResourcePoolConfig::MIN_REFURBISH_INTERVAL = 10000L;

//10 Minutes
const long common_pool::ResourcePoolConfig::MAX_SHRINK_INTERVAL = 600000L;
//10 Seconds
const long common_pool::ResourcePoolConfig::MIN_SHRINK_INTERVAL = 10000L;

//10 Minutes
const long common_pool::ResourcePoolConfig::MAX_RECLAIM_INTERVAL = 600000L;
//10 Seconds
const long common_pool::ResourcePoolConfig::MIN_RECLAIM_INTERVAL = 10000L;

const long common_pool::ResourcePoolConfig::MAX_SHRINK_WINDOW = MAX_SHRINK_INTERVAL * 5L;
const long common_pool::ResourcePoolConfig::MIN_SHRINK_WINDOW = MIN_SHRINK_INTERVAL * 5L;


const long common_pool::ResourcePoolConfig::MAX_EXPIRE_MILLIS = MAX_REFURBISH_INTERVAL;
const long common_pool::ResourcePoolConfig::MIN_EXPIRE_MILLIS = MIN_REFURBISH_INTERVAL;


common_pool::ResourcePoolConfig::ResourcePoolConfig()
{
    _poolSize = 1024;
    _initPoolSize = MIN_POOL_SIZE;
    _refurbishInterval = MAX_REFURBISH_INTERVAL;
    _shrinkInterval = MAX_SHRINK_INTERVAL;
    _reclaimInterval = MAX_RECLAIM_INTERVAL;
}

common_pool::ResourcePoolConfig::~ResourcePoolConfig()
{
    //Do nothing
}

int common_pool::ResourcePoolConfig::getPoolSize()
{
    return _poolSize;
}

void common_pool::ResourcePoolConfig::setPoolSize(int poolSize)
{
    _poolSize = fixValue(poolSize, MIN_POOL_SIZE, MAX_POOL_SIZE);
}

int common_pool::ResourcePoolConfig::getInitPoolSize()
{
    return _initPoolSize;
}

void common_pool::ResourcePoolConfig::setInitPoolSize(int initPoolSize)
{
    _initPoolSize = fixValue(initPoolSize, MIN_INIT_POOL_SIZE, MAX_INIT_POOL_SIZE);
}

long common_pool::ResourcePoolConfig::getRefurbishInterval()
{
    return _refurbishInterval;
}

void common_pool::ResourcePoolConfig::setRefurbishInterval(long refurbishInterval)
{
    _refurbishInterval = fixValue(refurbishInterval, MIN_REFURBISH_INTERVAL, MAX_REFURBISH_INTERVAL);
}

long common_pool::ResourcePoolConfig::getShrinkInterval()
{
    return _shrinkInterval;
}

void common_pool::ResourcePoolConfig::setShrinkInterval(long shrinkInterval)
{
    _shrinkInterval = fixValue(shrinkInterval, MIN_SHRINK_INTERVAL, MAX_SHRINK_INTERVAL);
}

long common_pool::ResourcePoolConfig::getReclaimInterval()
{
    return _reclaimInterval;
}

void common_pool::ResourcePoolConfig::setReclaimInterval(long reclaimInterval)
{
    _reclaimInterval = fixValue(reclaimInterval, MIN_RECLAIM_INTERVAL, MAX_RECLAIM_INTERVAL);
}

long common_pool::ResourcePoolConfig::getShrinkWindow()
{
    return _shrinkWindow;
}

void common_pool::ResourcePoolConfig::setShrinkWindow(long shrinkWindow)
{
    _shrinkWindow = fixValue(shrinkWindow, MIN_SHRINK_WINDOW, MAX_SHRINK_WINDOW);
}


long common_pool::ResourcePoolConfig::getExpireMillis()
{
    return _expireMillis;
}

void common_pool::ResourcePoolConfig::setExpireMillis(long expireMillis)
{
    _expireMillis = expireMillis;
}
