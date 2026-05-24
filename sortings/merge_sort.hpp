#include "../sorting_base.hpp"

#include <string>
#include <vector>

#pragma once

template <class R, ThreeWayComparator<char> Cmp = DefaultCharCmp>
class MergeSort : public IStringSorting<R, Cmp>
{
public:
    std::string name() const override
    {
        return "MergeSort";
    }

    void sort(permutation_view<R> view) override
    {
        if (view.size() == 1)
        {
            return;
        }

        size_t m = view.size() >> 1;

        auto left = view.subview(0, m);
        auto right = view.subview(m, view.size());
        sort(left);
        sort(right);

        merge(view, left, right);
    }

private:
    Cmp cmp{};

    void merge(permutation_view<R> view, permutation_view<R> left, permutation_view<R> right)
    {
        typename permutation_view<R>::perm_type new_perm;
        new_perm.reserve(view.size());

        size_t i = 0, j = 0;
        while (i < left.size() && j < right.size())
        {
            if (LcpCompare(left[i], right[j], 0, cmp).result < 0)
            {
                new_perm.push_back(i++);
            }
            else
            {
                new_perm.push_back(left.size() + (j++));
            }
        }

        while (i < left.size())
        {
            new_perm.push_back(i++);
        }

        while (j < right.size())
        {
            new_perm.push_back(left.size() + (j++));
        }

        view.compose(new_perm);
    }
};