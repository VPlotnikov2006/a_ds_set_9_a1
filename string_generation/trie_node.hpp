#include <map>
#include <string>
#include <random>
#include <vector>
#include <memory>

#pragma once

struct Node
{
    size_t depth;
    size_t term_here;
    char pc;
    std::map<char, Node *> child;

    Node(char pc, Node *parent = nullptr) : depth(0), term_here(0), pc(pc), child()
    {
        if (parent)
        {
            depth = parent->depth + 1;
            parent->child[pc] = this;
        }
    }

    ~Node()
    {
        for (const auto &[_, ptr] : child)
        {
            std::destroy_at(ptr);
        }
    }
};

void build_random_trie(
    Node *root,
    size_t n_rem,
    size_t min_depth,
    size_t max_depth,
    const std::string_view &alphabet,
    std::mt19937 &gen,
    std::uniform_int_distribution<size_t> &char_dst)
{
    root->child.clear();
    if (root->depth < min_depth)
    {
        root->term_here = 0;
    }
    else if (root->depth >= max_depth)
    {
        root->term_here = n_rem;
        return;
    }
    else
    {
        double p_term = 1. / (max_depth - root->depth + 1);
        root->term_here = 0;

        std::binomial_distribution<size_t> term_dst(n_rem, p_term);

        root->term_here = term_dst(gen);
    }

    n_rem -= root->term_here;
    std::map<char, size_t> child_cnt;

    (0, alphabet.size() - 1);

    for (size_t i = 0; i < n_rem; i++)
    {
        child_cnt[alphabet[char_dst(gen)]]++;
    }

    for (const auto [pc, cnt] : child_cnt)
    {
        build_random_trie(new Node(pc, root), cnt, min_depth, max_depth, alphabet, gen, char_dst);
    }
}

void collect(Node *root, std::vector<std::string> &to, std::string &prefix)
{
    for (size_t i = 0; i < root->term_here; i++)
    {
        to.push_back(prefix);
    }

    for (const auto &[pc, ptr] : root->child)
    {
        prefix.push_back(pc);
        collect(ptr, to, prefix);
        prefix.pop_back();
    }
}