#include "shared_pool.hpp"
#include <cstdint>
#include <set>

int main(int _argc, char *_argv[])
{
    shared_pool< int > pool(32);
    
    std::set< std::shared_ptr< int > > objects;
    
    for (int32_t i = 0; i < 32; ++i)
    {
        std::shared_ptr< int > obj = pool.acquire();
        objects.insert(obj);
    }
    
    objects.clear();
    
    return 0;
}