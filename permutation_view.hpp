#include <ranges>
#include <vector>
#include <numeric>
#include <cassert>
#include <memory>

#pragma once

template <std::ranges::random_access_range V>
    requires std::ranges::view<V> && std::ranges::sized_range<V>
class permutation_view : public std::ranges::view_interface<permutation_view<V>>
{
public:
    using perm_type = std::vector<size_t>;

    V base_;
    std::shared_ptr<perm_type> perm_;

    size_t offset_;
    size_t size_;

    permutation_view(
        V base,
        std::shared_ptr<perm_type> perm,
        size_t offset,
        size_t size) : base_(std::move(base)),
                       perm_(perm),
                       offset_(offset),
                       size_(size)
    {
        assert(base_.size() == perm_->size());
        assert(offset + size_ <= perm_->size());
    }

    permutation_view(V base) : base_(std::move(base)),
                               perm_(std::make_shared<perm_type>(base_.size())),
                               offset_(0),
                               size_(base_.size())
    {
        iota(perm_->begin(), perm_->end(), 0);
    }

    void compose(const perm_type &p)
    {
        assert(p.size() == size_);

        perm_type result(*perm_);

        for (size_t i = 0; i < p.size(); i++)
        {
            result[i + offset_] = (*perm_)[p[i] + offset_];
        }

        *perm_ = std::move(result);
    }

    permutation_view<V> copy() const
    {
        return permutation_view<V>{base_, std::make_shared<perm_type>(*perm_), offset_, size_};
    }

    permutation_view<V> subview(size_t l, size_t r)
    {
        return permutation_view<V>{base_, perm_, offset_ + l, r - l};
    }

    permutation_view<V> &swap(size_t left, size_t right)
    {
        std::swap((*perm_)[left + offset_], (*perm_)[right + offset_]);
        return *this;
    }

    size_t size() const
    {
        return size_;
    }

    decltype(auto) operator[](size_t idx)
    {
        return base_[(*perm_)[idx + offset_]];
    }
};
