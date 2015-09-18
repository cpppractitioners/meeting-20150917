#include <atomic>
#include <boost/lockfree/stack.hpp>
#include <cstdint>
#include <memory>

template < typename _T >
class shared_pool
{
private:
    class deleter
    {
    private:
        shared_pool *m_pool;
        
    public:
        inline deleter(shared_pool *_pool) : m_pool(_pool)
        {
        }
        
        inline void operator()(_T *_obj)
        {
            m_pool->release(_obj);
        }
    };
    
private:
    boost::lockfree::stack< int32_t > m_freeObjects;
    _T *m_objects;
    int32_t m_maxSize;
    deleter m_deleter;
    
public:
    inline shared_pool(int32_t _size)
        : m_freeObjects(_size), m_maxSize(_size), m_deleter(this)
    {
        m_objects = new _T[_size];
        
        for (int32_t i = 0; i < m_maxSize; ++i)
        {
            m_freeObjects.push(i);
        }
    }
    
    inline ~shared_pool()
    {
        delete[] m_objects;
    }
    
    inline std::shared_ptr< _T > acquire()
    {
        int32_t index;
        
        if (m_freeObjects.pop(index))
        {
            return std::shared_ptr< _T >(&m_objects[index], m_deleter);
        }
        else
        {
            return std::shared_ptr< _T >(new _T());
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