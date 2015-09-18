#include <boost/intrusive_ptr.hpp>
#include <atomic>
#include <boost/lockfree/stack.hpp>
#include <cstdint>

template < typename _T >
class intrusive_pool;

template <typename _T >
class intrusive_poolable;

template < typename _T >
class intrusive_pool
{
private:
    boost::lockfree::stack< int32_t > m_freeObjects;
    _T *m_objects;
    int32_t m_maxSize;
    
public:
    inline intrusive_pool(int32_t _size)
        : m_freeObjects(_size), m_maxSize(_size)
    {
        m_objects = new _T[_size];
        
        for (int32_t i = 0; i < m_maxSize; ++i)
        {
            m_objects[i].set_pool(this);
            m_freeObjects.push(i);
        }
    }
    
    inline ~intrusive_pool()
    {
        delete[] m_objects;
    }
    
    inline boost::intrusive_ptr< _T > acquire()
    {
        int32_t index;
        
        if (m_freeObjects.pop(index))
        {
            return boost::intrusive_ptr< _T >(&m_objects[index]);
        }
        else
        {
            _T *obj = new _T();
            obj->set_pool(this);
            return boost::intrusive_ptr< _T >(obj);
        }
    }
    
    inline void release(_T *_obj)
    {
        int32_t index = _obj - m_objects;
        
        if (index >= 0 && index < m_maxSize)
        {
            m_freeObjects.push(index);
        }
        else
        {
            delete _obj;
        }
    }
};

template< typename _T >
class intrusive_poolable
{
friend class intrusive_pool< _T >;

protected:
    std::atomic< std::size_t > m_refCount;
    intrusive_pool< _T > *m_pool;
    
public:
    inline intrusive_poolable() : m_refCount(0), m_pool(nullptr) {}
    
private:
    inline void set_pool(intrusive_pool< _T > *_pool) { m_pool = _pool; }
    
public:
    
    friend inline void intrusive_ptr_add_ref(intrusive_poolable *_obj)
    {
        ++_obj->m_refCount;
    }
    
    friend inline void intrusive_ptr_release(intrusive_poolable *_obj)
    {
        if (--_obj->m_refCount == 0)
        {
            _obj->m_pool->release(static_cast< _T * >(_obj));
        }
    }
};