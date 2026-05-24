#include "common.hpp"
#include "permutation_view.hpp"

#pragma once

template <class R, ThreeWayComparator<char> Cmp = DefaultCharCmp>
class IStringSorting
{
public:
    virtual void sort(permutation_view<R> arr) = 0;

    virtual std::string name() const = 0;

    virtual ~IStringSorting() = default;
};