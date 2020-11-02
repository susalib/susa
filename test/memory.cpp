#include <iostream>

#include <vector>
#include <list>
#include <set>

#ifndef SUSA_NDEBUG
#define SUSA_NDEBUG
#endif

#include "test.h"


template <template <typename T, typename AT> typename TYPE> void test()
{
    SUSA_LOG_DBG("[before] total allocated memory : " << susa::memory_tacker::instance().read() << " bytes.");
    TYPE <unsigned, susa::allocator_log<unsigned>> container;
    for (unsigned cnt = 0; cnt < 1e3; cnt++)
    {
        container.insert(std::end(container), cnt);
    }

    SUSA_LOG_DBG("[after] total allocated memory : " << susa::memory_tacker::instance().read() << " bytes.");
}

template <typename T, typename AT> using set_default_comparator = std::set<T, std::less<>, AT>;

int main()
{
    SUSA_TEST_EQ(susa::memory_tacker::instance().read(), 0, "initial memory tracker must be zero");

    {
    std::vector<int, susa::allocator_log<int> > int_vec(1024, 0, susa::allocator_log<int>());

    test <std::vector>();
    test <std::list>();
    test <set_default_comparator>();
    }

    SUSA_TEST_EQ(susa::memory_tacker::instance().read(), 0, "std types memory leak with susa allocator");


    {
    susa::array<int, susa::allocator_log<int> > int_array_a({2,3,4});
    susa::array<int, susa::allocator_log<int> > int_array_b({20,313,473,5});
    susa::array<int, susa::allocator_log<int> > int_array_c;
    int_array_b = int_array_a;
    susa::array<int, susa::allocator_log<int> > int_array_d(std::move(int_array_a));

    susa::array <int, susa::allocator_log<int> > arr_a({21,6,5,15,43});
    arr_a(2,4,3,0,1) = 55;
    arr_a(12,4,3,5,1) = 32;
    susa::array <int, susa::allocator_log<int> > arr_b = arr_a;
    }

    SUSA_TEST_EQ(susa::memory_tacker::instance().read(), 0, "susa::array memory leak with susa allocator");

    SUSA_TEST_PRINT_STATS();

    return (uint_failed);
}