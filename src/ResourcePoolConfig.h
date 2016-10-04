#ifndef _RESOURCEPOOLCONFIG_H
#define _RESOURCEPOOLCONFIG_H

namespace pool
{
    class ResourcePoolConfig
    {
    public:
        static const int MAX_POOL_SIZE;
        static const int MIN_POOL_SIZE;
        static const int MAX_INIT_POOL_SIZE;
        static const int MIN_INIT_POOL_SIZE;
        static const long MAX_REFURBISH_INTERVAL;
        static const long MIN_REFURBISH_INTERVAL;
        static const long MAX_SHRINK_INTERVAL;
        static const long MIN_SHRINK_INTERVAL;
        static const long MAX_RECLAIM_INTERVAL;
        static const long MIN_RECLAIM_INTERVAL;
        
    public:
        ResourcePoolConfig();
        virtual ~ResourcePoolConfig();

        int getPoolSize();
        void setPoolSize(int poolSize);

        int getInitPoolSize();
        void setInitPoolSize(int initPoolSize);

        long getRefurbishInterval();
        void setRefurbishInterval(long refurbishInterval);

        long getShrinkInterval();
        void setShrinkInterval(long shrinkInterval);
        
        long getReclaimInterval();
        void setReclaimInterval(long reclaimInterval);

        template<class T> T fixValue(T value, T minValue, T maxValue, T defaultValue=minValue)
        {
            return T >= minValue && T <= max_value ? T : defaultValue;
        };
                
    private:
        int _poolSize;
        int _initPoolSize;
        long _refurbishInterval;
        long _shrinkInterval;
        long _reclaimInterval;
    };

} //namespace pool
#endif //_RESOURCEPOOLCONFIG_H