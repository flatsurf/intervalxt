#include <iet.hh>
#include <iostream>

int main(void)
{
    {
        IntervalExchangeTransformation<unsigned long, unsigned long> iet(3);
        Permutation topperm(3);
        Permutation botperm(3);

        botperm[0] = 2;
        botperm[1] = 1;
        botperm[2] = 0;
        iet.setBot(botperm);
        if (iet.isReducible())
            throw std::runtime_error("failed irreducibility test 1");

        botperm[0] = 1;
        botperm[1] = 0;
        botperm[2] = 2;
        iet.setBot(botperm);
        if (not iet.isReducible())
            throw std::runtime_error("failed irreducibility test 2");

        topperm[0] = 2;
        topperm[1] = 0;
        topperm[2] = 1;
        botperm[0] = 0;
        botperm[1] = 2;
        botperm[2] = 1;
        iet.setTop(topperm);
        iet.setBot(botperm);
        if (not iet.isReducible())
            throw std::runtime_error("failed irreducibility test 3");
    }

    return 0;
}
