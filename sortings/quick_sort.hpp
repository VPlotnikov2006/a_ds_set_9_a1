#include "../sorting_base.hpp"

#include <string>
#include <vector>
#include <random>

#pragma once

template <class R, ThreeWayComparator<char> Cmp = DefaultCharCmp>
class QuickSort : public IStringSorting<R, Cmp>
{
public:
    std::string name() const override
    {
        return "QuickSort";
    }

    void sort(permutation_view<R> view) override
    {
        if (view.size() <= 1)
        {
            return;
        }

        size_t l = 0, r = 1;
        size_t pivot = std::uniform_int_distribution<size_t>(0, view.size() - 1)(gen);
        view.swap(0, pivot);

        for (size_t i = 1; i < view.size(); i++)
        {
            auto res = LcpCompare(view[i], view[l], 0, cmp);
            if (res.result == 0)
            {
                view.swap(r++, i);
            }
            else if (res.result < 0)
            {
                view.swap(l++, i);
                view.swap(r++, i);
            }
        }
        sort(view.subview(0, l));
        sort(view.subview(r, view.size()));
    }

private:
    Cmp cmp{};
    std::mt19937 gen{42};
};