#ifndef _RESOURCEPOOL_H
#define _RESOURCEPOOL_H

#include <boost/thread.hpp>
#include <map>
#include <list>
#include <list>

namespace pool
{
    template<class T>
    class ResourcePool
    {
    public:
        
        //Get the number of managed resource objects
        int getManagedNum();

        //Get the number of unused resource objects
        int getUnusedNum();

        //Get the number of excluded resource objects
        int getExcludedNum();

        //Get the number of resource objects
        int getResourceNum();

    protected:

        bool started;

        //The length of slide window for shrink
        static const long SHRINK_WINDOW;
        
        //The ratio of shrined resource objects
        static const long SHRINK_FACTOR;

        //Managed resource
        std::map<ResourceWrapper*, T*> managed;

        //Unused resource
        std::list<T*> unused;

        //A swap space for reclaim
        std::list<T*> unused;

        //Add a resource object the pool
        virtual void expand();

        //Refurbish a single resource object
        virtual void refurbish(T &t);

        //Shrink the pool to release redundant resource objects
        virtual void shrink();

        //Remove a resource object out of the pool
        virtual void removeResource();

        //Put exclude resource objects in unused space
        virtual void doCheckinExcluded();

        //Collect
        void relaimIdleResource(long reclaimInterval);

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

        const T & unwrap()
        {
            return *t;
        };

    protected:
        T *t;
    }; //class ResourceWrapper


} //namespace rpool
#endif //_RESOURCEPOOL_H