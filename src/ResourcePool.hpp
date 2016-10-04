#ifndef _RESOURCEPOOL_H
#define _RESOURCEPOOL_H

#include <map>
#include <list>
#include <set>

#include <boost/thread.hpp>
#include <boost/circular_buffer.hpp>

namespace pool
{
    template<class T>
    class ResourcePool
    {
    public:
        
        //Get the number of managed resources
        int getManagedNum();

        //Get the number of unused resources
        int getUnusedNum();

        //Get the number of excluded resources
        int getExcludedNum();

        //Get the number of resources
        int getResourceNum();
        
    protected:

        //Add a resource object the pool
        virtual void expand();

        //Refurbish a single resource object
        virtual void refurbish(T &t);

        //Shrink the pool to release redundant resources
        virtual void shrink();

        //Remove a resource object out of the pool
        virtual void removeResource();

        //Put exclude resources in unused space
        virtual void doCheckinExcluded();

        //Collect
        void relaimIdleResource(long reclaimInterval);

        //Get the resource number that pool should shrink to
        void getShrinkThreshold(boost::circular_buffer<int> &slideWindow, float meanRemain)
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
                //If managed size is lower than initialized number, set the total number to initialized number
                if (shrinkSize < 0)
                {
                    
                }
            }
            
        };

    protected:

        //The length of slide window for shrink
        static const long SHRINK_WINDOW;

        //The ratio of shrined resources
        static const long SHRINK_FACTOR;

        //Managed resource
        std::map<ResourceWrapper*, T*> managed;

        //Unused resource
        std::list<T*> unused;

        //A swap space for reclaim
        //Some
        std::set<T*> excluded;

    }; //class ResourcePool


    template<class T>
    class ResourceWrapper
    {
    public:
        ResourceWrapper(T &t_)
        {
            t = &t_;
        };

        virtual ~ResourceWrapper()
        {

        };

        //If the resource needs to be reclaimed
        //You can override this function
        virtual bool expired()
        {

        };
                
        const T & get()
        {
            return *t;
        };

    private:
        T *t;
    }; //class ResourceWrapper


} //namespace rpool
#endif //_RESOURCEPOOL_H