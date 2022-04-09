#ifndef __YI_RANDOM__
#define __YI_RANDOM__

#include <random>
namespace yi
{
    class Random
    {
    private:
        std::random_device _random;
        std::mt19937 _gen;
        std::uniform_int_distribution<int32_t> _uniform_dis;

    public:
        Random(int32_t min, int32_t max):_gen(_random()), _uniform_dis(min, max)
        {

        }
        ~Random()
        {

        }
        
        int32_t MakeRandom()
        {
            return _uniform_dis(_gen);
        }
    };
} // namespace yi

#endif