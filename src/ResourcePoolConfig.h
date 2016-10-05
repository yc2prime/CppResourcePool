#ifndef _RESOURCEPOOLCONFIG_H
#define _RESOURCEPOOLCONFIG_H

namespace common_pool
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
        static const long MAX_SHRINK_WINDOW;
        static const long MIN_SHRINK_WINDOW;
        static const long MAX_EXPIRE_MILLIS;
        static const long MIN_EXPIRE_MILLIS;

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

        long getShrinkWindow();
        void setShrinkWindow(long shrinkWindow);

        long getExpireMillis();
        void setExpireMillis(long expireMillis);

        template<class T> T fixValue(T value, T minValue, T maxValue)
        {
            return value >= minValue && value <= maxValue ? value : minValue;
        };

        template<class T> T fixValue(T value, T minValue, T maxValue, T defaultValue)
        {
            return value >= minValue && value <= maxValue ? value : defaultValue;
        };
                
    private:
        int _poolSize;
        int _initPoolSize;
        long _refurbishInterval;
        long _shrinkInterval;
        long _reclaimInterval;
        long _shrinkWindow;
        long _expireMillis;
    };

} //namespace pool
#endif //_RESOURCEPOOLCONFIG_H