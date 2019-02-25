#include "iet.hh"
#include <iostream>

int main(void)
{

    {
        std::cout << "Example 1" << std::endl;
        IntervalExchangeTransformation<unsigned long, unsigned long> iet(2);
        std::vector<unsigned long> v(2);
        v[0] = 18;
        v[1] = 3;
        iet.setLengths(v);

        if (iet.lengths() != v)
            throw std::runtime_error("badly initialized");
    }

    {
        std::cout << "Example 2" << std::endl;
        Permutation top(4);
        Permutation bot(4);

        top[0] = 0;
        top[1] = 1;
        top[2] = 3;
        top[3] = 2;
        bot[0] = 3;
        bot[1] = 0;
        bot[2] = 1;
        bot[3] = 2;

        IntervalExchangeTransformation<unsigned long, unsigned long> iet(top, bot);

        if (iet.topPermutation() != top || iet.botPermutation() != bot)
            throw std::runtime_error("badly initialized");
    }

}
