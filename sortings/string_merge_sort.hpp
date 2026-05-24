#include "../sorting_base.hpp"

#include <string>
#include <vector>

#pragma once

struct SortingResult
{
    size_t idx;
    size_t lcp;
};

template <class R, ThreeWayComparator<char> Cmp = DefaultCharCmp>
class StringMergeSort : public IStringSorting<R, Cmp>
{
public:
    std::string name() const override
    {
        return "StringMergeSort";
    }

    void sort(permutation_view<R> view) override
    {
        sort_impl(view);
    }

private:
    Cmp cmp{};

    std::vector<size_t> sort_impl(permutation_view<R> container)
    {
        if (container.size() == 1)
        {
            return {0};
        }

        size_t m = container.size() >> 1;

        auto l = sort_impl(container.subview(0, m));
        auto r = sort_impl(container.subview(m, container.size()));

        return string_merge(container, std::move(l), std::move(r));
    }

    std::vector<size_t> string_merge(permutation_view<R> container, std::vector<size_t> &&left, std::vector<size_t> &&right)
    {
        std::vector<size_t> result;
        typename permutation_view<R>::perm_type new_perm;

        result.reserve(left.size() + right.size());
        new_perm.reserve(left.size() + right.size());

        size_t i = 0, j = 0;
        while (i < left.size() && j < right.size())
        {
            if (left[i] > right[j])
            {
                new_perm.push_back(i);
                result.push_back(left[i++]);
            }
            else if (left[i] < right[j])
            {
                new_perm.push_back(left.size() + j);
                result.push_back(right[j++]);
            }
            else
            {
                auto res = LcpCompare(container[i], container[left.size() + j], left[i], cmp);

                if (res.result < 0)
                {
                    new_perm.push_back(i);
                    result.push_back(left[i++]);
                    right[j] = res.lcp;
                }
                else
                {
                    new_perm.push_back(left.size() + j);
                    result.push_back(right[j++]);
                    left[i] = res.lcp;
                }
            }
        }

        while (i < left.size())
        {
            new_perm.push_back(i);
            result.push_back(left[i++]);
        }

        while (j < right.size())
        {
            new_perm.push_back(left.size() + j);
            result.push_back(right[j++]);
        }

        container.compose(new_perm);

        return result;
    }
};