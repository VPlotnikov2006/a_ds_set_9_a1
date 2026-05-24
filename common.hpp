#include <string>

#pragma once

template <class Cmp, class T>
concept ThreeWayComparator =
    requires(Cmp cmp, T const &a, T const &b) {
        { cmp(a, b) } -> std::signed_integral;
    } && std::default_initializable<Cmp>;

struct ComparisonResult
{
    int result;
    size_t lcp;
};

struct DefaultCharCmp
{
    int operator()(char a, char b) const
    {
        return static_cast<unsigned char>(a) - static_cast<unsigned char>(b);
    }
};

struct CountCharCmp
{
    inline static size_t Counter = 0;

    int operator()(char a, char b) const
    {
        Counter++;
        return static_cast<unsigned char>(a) - static_cast<unsigned char>(b);
    }
};

template <ThreeWayComparator<char> Cmp>
ComparisonResult LcpCompare(std::string_view a, std::string_view b, size_t k = 0, Cmp cmp = {})
{
    size_t n = std::min(a.size(), b.size());
    k = std::min(n, k);
    int r;

    for (; k < n && (r = cmp(a[k], b[k])) == 0; k++)
        ;

    if (k == n)
    {
        if (a.size() < b.size())
            return {-1, k};

        if (a.size() > b.size())
            return {1, k};

        return {0, k};
    }

    return {r, k};
}