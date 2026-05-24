#include "../sorting_base.hpp"

#include <string>
#include <vector>
#include <random>

#pragma once

template <class R, ThreeWayComparator<char> Cmp = DefaultCharCmp>
class MSBRadixQuickSort;

template <class R, ThreeWayComparator<char> Cmp = DefaultCharCmp>
class StringQuickSort : public IStringSorting<R, Cmp>
{
public:
    friend class MSBRadixQuickSort<R, Cmp>;
    std::string name() const override
    {
        return "StringQuickSort";
    }

    void sort(permutation_view<R> view) override
    {
        sort_impl(view, 0);
    }

private:
    void sort_impl(permutation_view<R> view, size_t L)
    {
        if (view.size() <= 1)
        {
            return;
        }

        size_t excl = 0;
        for (size_t i = 0; i < view.size(); i++)
        {
            if (view[i].size() == L)
            {
                view.swap(excl++, i);
            }
        }

        view = view.subview(excl, view.size());

        size_t l = 0, r = 1;
        size_t pivot_idx = std::uniform_int_distribution<size_t>(0, view.size() - 1)(gen);
        char pivot = view[pivot_idx][L];
        view.swap(0, pivot_idx);

        for (size_t i = 1; i < view.size(); i++)
        {
            int result = cmp(view[i][L], pivot);
            if (result == 0)
            {
                view.swap(r++, i);
            }
            if (result < 0)
            {
                view.swap(l++, i);
                view.swap(r++, i);
            }
        }

        sort_impl(view.subview(0, l), L);
        sort_impl(view.subview(l, r), L + 1);
        sort_impl(view.subview(r, view.size()), L);
    }

    Cmp cmp{};
    std::mt19937 gen{42};
};