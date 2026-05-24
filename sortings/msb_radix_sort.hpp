#include "../sorting_base.hpp"
#include "../string_generation/alphabet.hpp"

#include <string>
#include <vector>

#pragma once

template <class R, ThreeWayComparator<char> Cmp = DefaultCharCmp>
class MSBRadixSort : public IStringSorting<R, Cmp>
{
public:
    std::string name() const override
    {
        return "MSBRadixSort";
    }

    void sort(permutation_view<R> view) override
    {
        sort_impl(view, 0);
    }

private:
    Cmp cmp{};
    size_t n_buckets = alphabet::get_alphabet().size();

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
        std::vector<std::vector<size_t>> buckets{n_buckets};

        for (size_t i = 0; i < view.size(); i++)
        {
            buckets[alphabet::inv[view[i][L]]].push_back(i);
        }

        std::vector<size_t> perm;
        perm.reserve(view.size());

        for (const auto &b : buckets)
        {
            for (size_t i : b)
            {
                perm.push_back(i);
            }
        }

        view.compose(perm);
        size_t start = 0;
        for (const auto &b : buckets)
        {
            sort_impl(view.subview(start, start + b.size()), L + 1);
            start += b.size();
        }
    }
};