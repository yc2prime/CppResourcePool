#ifndef _RESOURCEPOOL_H
#define _RESOURCEPOOL_H

#include <map>
#include <list>
#include <set>
#include <vector>

#include <boost/thread.hpp>
#include <boost/foreach.hpp>
#include <boost/timer.hpp>
#include <boost/circular_buffer.hpp>

#include "ResourcePoolConfig.h"

namespace bt = boost::this_thread;
namespace bpt = boost::posix_time;


namespace common_pool
{
    template<class T> class ResourcePool;
    
    template<class T>
    class ResourceWrapper
    {
    public:
        ResourceWrapper(T *t_, ResourcePool<T> *pool, long expireMillis_)
        {
            t = t_;
            resourcePool = pool;
            updateLastestUseTime();
            expireMillis = expireMillis_;
            lastestUseTime = (long)clock.elapsed();
        };

        virtual ~ResourceWrapper()
        {
            if (t != NULL)
            {
                destory();
            }
        };

        //If the resource needs to be reclaimed
        //You can override this function
        virtual bool expired()
        {
            return (clock.elapsed() - lastestUseTime) * 1000.0 > (double)expireMillis;
        };

        //If a resource needs to be managed
        virtual bool valid()
        {
            return true;
        }

        T *get()
        {
            updateLastestUseTime();
            return t;
        };

        virtual void destory()
        {
            t = NULL;
            resourcePool->recliam(this);
        };

        void updateLastestUseTime()
        {
            lastestUseTime = clock.elapsed();
        };

    private:
        T *t;
        long expireMillis;
        boost::timer clock;
        double lastestUseTime;
        ResourcePool<T> *resourcePool;
    }; //class ResourceWrapper

    template<class T>
    class ResourcePool
    {
        typedef ResourceWrapper<T> Res;

    public:

        ResourcePool(ResourcePoolConfig &poolConfig)
        {
            started = false;
            config = poolConfig;

            resourceNum = 0;
            managedNum = 0;

            for (int i = 0; i < config.getPoolSize(); i++)
            {
                expand();
            }

            minAvailableNum = config.getInitPoolSize();

            started = true;

            //Create reclaim thread
            boost::thread(&common_pool::ResourcePool<T>::relaimIdleResource, this, config.getReclaimInterval());
            
            //Create refurbish thread
            boost::thread(&common_pool::ResourcePool<T>::refurbish, this, config.getRefurbishInterval());

            //Create shrink thread
            boost::thread(&common_pool::ResourcePool<T>::shrink, this, config.getShrinkInterval());
        }

        ~ResourcePool()
        {
            close();
        };
        
        //Get the number of managed resources
        int getManagedNum()
        {
            return managed.size();
        };

        //Get the number of unused resources
        int getUnusedNum()
        {
            return unused.size();
        };

        //Get the number of excluded resources
        int getExcludedNum()
        {
            return excluded.size();
        };

        //Get the number of resources
        int getResourceNum()
        {
            return resourceNum;
        };

        //Get resource pool configuration
        ResourcePoolConfig& getConfig()
        {
            return config;
        };

        virtual Res *genResource()
        {
            Res *res = NULL;

            if (unused.size() > 0)
            {
                T *t = *(unused.begin());
                unused.pop_front();
                if (t != NULL)
                {
                    res = new Res(t, this, config.getExpireMillis());
                    boost::lock_guard<boost::recursive_mutex> reclaimLock(reclaimMutex);
                    managed[res] = t;
                    managedNum++;
                }

                if ((int)unused.size() < minAvailableNum)
                {
                    minAvailableNum = (int)unused.size();
                }
            }

            return res;
        };

        //Reclaim a resource
        virtual void recliam(Res *resource)
        {
            if (resource != NULL)
            {
                boost::lock_guard<boost::recursive_mutex> reclaimLock(reclaimMutex);
                excluded.insert(managed[resource]);
                managed[resource] = NULL;
                resourceNum--;
            }
        };

    protected:

        //Add a resource object the pool
        virtual void expand()
        {
            T *t = new T;
            if (t != NULL)
            {
                unused.push_back(t);
            }
        };


        //Refurbish resources
        void refurbish(long interval)
        {
            while (started)
            {
                bt::sleep(bpt::milliseconds(interval));

                //Refurbish when system is available
                if (!getResMutex.try_lock())
                {
                    continue;;
                }
                if (!reclaimMutex.try_lock())
                {
                    getResMutex.unlock();
                }

                //Refurbish available resources
                if (unused.size() > 0)
                {
                    list<T*>::iterator iter = unused.begin();
                    list<T*>::iterator swapIter;
                    while (iter != unused.end())
                    {
                        swapIter = iter;
                        iter++;
                        refurbishOne(**iter);
                    }
                }
                reclaimMutex.unlock();
                getResMutex.unlock();
            }
        };


        //Refurbish a single resource object
        virtual void refurbishOne(T &t) {};


        //Shrink the pool to release redundant resources
        void shrink(long interval)
        {
            long windowNum = config.getShrinkWindow() / interval;
            boost::circular_buffer<int> windowRemain((int) windowNum);
            float meanRemain = (float)minAvailableNum;

            while (started)
            {
                bt::sleep(bpt::milliseconds(interval));

                doCheckinExcluded();
                windowRemain.push_back(minAvailableNum);

                boost::lock_guard<boost::recursive_mutex> lock(getResMutex);
                {
                    unsigned int shrinkSize = (unsigned int)getShrinkThreshold(windowRemain, meanRemain);
                    boost::lock_guard<boost::recursive_mutex> reclaimLock(reclaimMutex);

                    while (unused.size() > shrinkSize)
                    {
                        T *t = unused.back();
                        unused.pop_back();
                        delete t;
                        resourceNum--;
                    }
                    minAvailableNum = resourceNum;
                }
            }
        };

        //Remove a resource object out of the pool
        void removeResource()
        {
            T* t = unused.back();
            unused.pop_back();
            delete t;
        };

        //Put exclude resources in unused space
        void doCheckinExcluded()
        {
            boost::lock_guard<boost::recursive_mutex> reclaimLock(reclaimMutex);
            if (excluded.size() > 0)
            {
                BOOST_FOREACH(T *t, excluded)
                {
                    unused.push_back(t);
                }
                excluded.clear();
            }
        };

        //Collect
        void relaimIdleResource(long reclaimInterval)
        {
            while (started)
            {
                boost::this_thread::sleep(bpt::milliseconds(reclaimInterval));

                boost::lock_guard<boost::recursive_mutex> lock(getResMutex);

                if (managed.size() > 0)
                {
                    std::vector<Res*> idleRes;
                    boost::lock_guard<boost::recursive_mutex> reclaimLock(reclaimMutex);

                    std::map<Res*, T*>::iterator iter = managed.begin();
                    for (; iter != managed.end(); iter++)
                    {
                        if (iter->second == NULL)
                        {
                            //Reclaim invalid resource
                            idleRes.push_back(iter->first);
                        }
                        else if (iter->first->valid() && iter->first->expired())
                        {
                            excluded.insert(iter->second);
                            iter->second = NULL;
                            idleRes.push_back(iter->first);
                        }
                    }

                    if (idleRes.size() < 0)
                    {
                        std::vector<Res*>::iterator idleIter = idleRes.begin();
                        for (; idleIter != idleRes.end(); idleIter++)
                        {
                            managed.erase(managed.find(*idleIter));
                        }
                    }
                }                
            }
        };

        //Get the resource number that pool should shrink to
        int getShrinkThreshold(boost::circular_buffer<int> &slideWindow, float meanRemain)
        {
            //The window iterator
            boost::circular_buffer<int>::iterator iter = slideWindow.begin();
            
            float sum = 0.0f;
            while (iter != slideWindow.end())
            {
                sum += (float)*iter;
                iter++;
            }
            sum = sum / (float)slideWindow.size();
            
            float diff = sum - meanRemain;

            //If load goes down
            int shrinkSize = 0;
            if (diff > 0.0f)
            {
                shrinkSize = unused.size() - (int)diff;
                //When load goes down sharply
                if (shrinkSize < 0)
                {
                    //If managed size is lower than initial number, set the total number to initialized number
                    if (managed.size() < (unsigned int) config.getInitPoolSize())
                    {
                        shrinkSize = config.getInitPoolSize() - (int)managed.size();
                    }
                    //If managed size is larger than initial size, release all available resources
                    else
                    {
                        shrinkSize = 0;
                    }
                }
            }
            //If load goes up
            else if (diff < 0.0f)
            {
                //When average remaining number is larger than available number, 
                //it means there is no load
                if ((unsigned int)sum > unused.size() + managed.size())
                {
                    shrinkSize = (int)(sum * SHRINK_FACTOR);
                }
                //Do not release if there is no load
                else
                {
                    shrinkSize = 0;
                }
            }
            //If load doesn't change
            else
            {
                //No load
                if ((unsigned int)sum == unused.size() + managed.size())
                {
                    shrinkSize = config.getInitPoolSize();
                }
                //Has load
                {
                    shrinkSize = (int)unused.size();
                }
            }

            meanRemain = sum;
            return shrinkSize;
        };

        //Close the pool
        void close()
        {
            started = false;
            config.setInitPoolSize(0);

            boost::lock_guard<boost::recursive_mutex> lock(getResMutex);
            {
                boost::lock_guard<boost::recursive_mutex> recliamLock(reclaimMutex);

                std::map<Res*, T*>::iterator iter = managed.begin();
                while (iter != managed.end())
                {
                    if (iter->second != NULL)
                    {
                        unused.push_back(iter->second);
                    }
                    iter++;
                }
                managed.clear();

                while (unused.size() > 0)
                {
                    removeResource();
                }
            }
        };

    protected:
        
        //The ratio of shrined resources
        static const float SHRINK_FACTOR;        

        //Managed resource
        std::map<Res*, T*> managed;

        //Unused resource
        std::list<T*> unused;

        //A swap space for reclaim
        //Some
        std::set<T*> excluded;

        //Configuration
        ResourcePoolConfig config;

        //If started
        bool started;

        //Get resource mutex
        boost::recursive_mutex getResMutex;

        //Reclaim mutex
        boost::recursive_mutex reclaimMutex;

        int minAvailableNum;
        int resourceNum;
        int managedNum;

    }; //class ResourcePool

    template<class T>
        const float common_pool::ResourcePool<T>::SHRINK_FACTOR = 0.5f;
           


} //namespace rpool
#endif //_RESOURCEPOOL_H