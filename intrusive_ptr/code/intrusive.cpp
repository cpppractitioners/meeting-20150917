#include "intrusive_pool.hpp"
#include <cstdint>
#include <set>

class poolable_object
    : public intrusive_poolable< poolable_object >
{
private:
    int32_t m_i;
};

int main(int _argc, char *_argv[])
{
    intrusive_pool< poolable_object > pool(32);
    
    std::set< boost::intrusive_ptr< poolable_object > > objects;
    
    for (int32_t i = 0; i < 32; ++i)
    {
        boost::intrusive_ptr< poolable_object > obj = pool.acquire();
        objects.insert(obj);
    }
    
    objects.clear();
    
    return 0;
}