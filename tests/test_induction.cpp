#include <iet.hh>
#include <iostream>

void test1()
{
    IntervalExchangeTransformation<unsigned long, unsigned long> iet(2);
    Permutation topperm(2);
    Permutation botperm(2);

    botperm[1] = 0;
    botperm[0] = 1;
    iet.setBot(botperm);

    std::vector<unsigned long> l0(2);
    l0[0] = 23;
    l0[1] = 5;
    iet.setLengths(l0);

    std::vector<unsigned long> l1(2);
    l1[0] = 23 - 4*5;
    l1[1] = 5;

    iet.check();
    iet.zorichInductionStep();
    iet.check();

    if (iet.lengths() != l1)
        throw std::runtime_error("Zorich induction broken");
}

void test2()
{
    IntervalExchangeTransformation<unsigned long, unsigned long> iet(5);
    Permutation botperm(5);
    Permutation topperm(5);

    // 0 1 2 3 4   lengths = 977 351 143 321 12
    // 3 2 0 4 1
    botperm[0] = 3;
    botperm[1] = 2;
    botperm[2] = 0;
    botperm[3] = 4;
    botperm[4] = 1;
    topperm[0] = 0;
    topperm[1] = 1;
    topperm[2] = 2;
    topperm[3] = 3;
    topperm[4] = 4;
    iet.setTop(topperm);
    iet.setBot(botperm);

    std::vector<unsigned long> l0(5);
    l0[0] = 977;
    l0[1] = 351;
    l0[2] = 143;
    l0[3] = 321;
    l0[4] = 12;
    iet.setLengths(l0);

    std::vector<unsigned long> l(l0);

    l[0] = 49;

    iet.check();
    iet.zorichInductionStep();
    iet.check();

    std::cout << iet << std::endl;

    if (iet.lengths() != l || iet.botPermutation() != botperm || iet.topPermutation() != topperm)
    {
        throw std::runtime_error("Zorich induction broken 1");
    }

    l[3] = 272;
    topperm[0] = 1;
    topperm[1] = 2;
    topperm[2] = 0;

    iet.swapTopBot();
    iet.zorichInductionStep();
    iet.check();

    std::cout << iet << std::endl;

    if (iet.lengths() != l || iet.botPermutation() != topperm || iet.topPermutation() != botperm)
        throw std::runtime_error("Zorich induction broken 2");

    l[1] = 79;
    botperm[0] = 2;
    botperm[1] = 0;
    botperm[2] = 4;
    botperm[3] = 3;

    iet.swapTopBot();
    iet.zorichInductionStep();
    iet.check();

    std::cout << iet << std::endl;

    if (iet.lengths() != l || iet.botPermutation() != botperm || iet.topPermutation() != topperm)
        throw std::runtime_error("Zorich induction broken 3");

    l[2] = 64;
    
    iet.swapTopBot();
    iet.zorichInductionStep();
    iet.check();

    std::cout << iet << std::endl;

    if (iet.lengths() != l || iet.botPermutation() != topperm || iet.topPermutation() != botperm)
        throw std::runtime_error("Zorich induction broken 4");

    l[1] = 15;

    botperm[0] = 0;
    botperm[1] = 4;
    botperm[2] = 3;
    botperm[3] = 2;

    iet.swapTopBot();
    iet.zorichInductionStep();
    iet.check();

    std::cout << iet << std::endl;

    if (iet.lengths() != l || iet.botPermutation() != botperm || iet.topPermutation() != topperm)
        throw std::runtime_error("Zorich induction broken 5");

    l[0] = 34;
    topperm[0] = 2;
    topperm[1] = 1;

    iet.swapTopBot();
    iet.zorichInductionStep();
    iet.check();

    std::cout << iet << std::endl;

    if (iet.lengths() != l || iet.botPermutation() != topperm || iet.topPermutation() != botperm)
        throw std::runtime_error("Zorich induction broken 6");
}

int main(void)
{
    std::cerr << "test1" << std::endl;
    std::cerr.flush();
    test1();
    std::cerr << "done" << std::endl;

    std::cerr << "test2" << std::endl;
    std::cerr.flush();
    test2();
    std::cerr << "done" << std::endl;

    return 0;
}
