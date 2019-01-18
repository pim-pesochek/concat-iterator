#pragma once

#pragma once

#include <vector>

template <typename T>
struct iter_trait {
    using reference = typename T::reference;
};

template <typename T>
struct iter_trait<T*> {
    using reference = T&;
};

template <typename Iter>
class concat_iterator {
    using collection_t = std::vector<std::pair<Iter, Iter>>;
    collection_t iterators;
public:

    concat_iterator(std::vector<std::pair<Iter, Iter>> list):
            iterators(std::move(list)) {
    }

    class Iterator;

    Iterator begin() {
        if (iterators.size() == 0) {
            return Iterator(iterators);
        }

        return Iterator(iterators, 0, iterators[0].first);
    }

    Iterator end() {
        if (iterators.size() == 0) {
            return Iterator(iterators);
        }
        auto last = iterators.size() - 1;
        return Iterator(iterators,
                        last,
                        iterators[last].second);
    }

    class Iterator {
        collection_t &iterators;
        size_t idx;
        Iter iter;
        bool empty{false};

    public:
        Iterator(collection_t &iterators, size_t idx, Iter iter):
                iterators(iterators), idx(idx), iter(std::move(iter)) {
        }

        Iterator(collection_t &iterators): iterators(iterators), empty(true) {
        }

        Iterator& operator ++() {
            if (empty) {
                return *this;
            }

            if (idx == iterators.size() - 1 && iter == iterators[idx].second) {
                return *this;
            }

            ++iter;

            if (idx == iterators.size() - 1 && iter == iterators[idx].second) {
                return *this;
            }

            if (iter == iterators[idx].second) {
                ++idx;
                iter = iterators[idx].first;
            }

            return *this;
        }

        bool operator != (const Iterator& rhs) {
            if (empty && rhs.empty) {
                return false;
            }

            if (empty || rhs.empty) {
                return true;
            }

            if (idx != rhs.idx) {
                return true;
            }

            return iter != rhs.iter;
        }

        typename iter_trait<Iter>::reference operator *() {
            return *iter;
        }
    };
};

template <typename T>
struct collection_trait {
    using iterator = typename T::iterator;
};

template <typename T>
struct collection_trait<const T> {
    using iterator = typename T::const_iterator;
};

template <typename ...Args>
struct get_iter_type;

template <typename T, typename ...Args>
struct get_iter_type<T, Args...>
{
    using type = typename collection_trait<T>::iterator;
};

template <typename T>
std::pair<typename collection_trait<T>::iterator, typename collection_trait<T>::iterator> get_pair(T& c) {
    return std::make_pair(c.begin(), c.end());
}

template <typename ...Args>
concat_iterator<typename get_iter_type<Args...>::type> concat(Args & ...args) {
    using pair_type = std::pair<typename get_iter_type<Args...>::type,
                                typename get_iter_type<Args...>::type>;
    std::vector<pair_type> pairs;

    auto p = { get_pair(args)... };

    for (auto &pair : p) {
        if (pair.first != pair.second) {
            pairs.push_back(pair);
        }
    }

    return concat_iterator<typename get_iter_type<Args...>::type>(std::move(pairs));
}