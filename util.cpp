#include "util.h"

std::pair<int, int> makeOrderedPair(int a, int b)
{
    std::pair<int, int> result(a, b);
    if (a > b)
    {
        result.first = b;
        result.second = a;
    }
    return result;
}
