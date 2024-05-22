#ifndef _INTERVAL_MAP_HPP
#define _INTERVAL_MAP_HPP

#include <map>

/**
 * Class implementing interval map.
 *
 * @tparam Key the type of the key
 * @tparam T the type of the values
 * @tparam Compare callable defining a strict weak ordering for the keys
 */
template<class Key, class T, class Compare, class Allocator, class Container>
class interval_map;

template<class Key, class T, class Compare, class Allocator, class Container>
bool operator==(
    const interval_map<Key, T, Compare, Allocator, Container>& lhs,
    const interval_map<Key, T, Compare, Allocator, Container>& rhs
    );

template<class Key, class T, class Compare, class Allocator, class Container>
bool operator!=(
    const interval_map<Key, T, Compare, Allocator, Container>& lhs,
    const interval_map<Key, T, Compare, Allocator, Container>& rhs
    );

template<class Key, class T, class Compare, class Allocator, class Container>
bool operator<(
    const interval_map<Key, T, Compare, Allocator, Container>& lhs,
    const interval_map<Key, T, Compare, Allocator, Container>& rhs
    );
template<class Key, class T, class Compare, class Allocator, class Container>
bool operator<=(
    const interval_map<Key, T, Compare, Allocator, Container>& lhs,
    const interval_map<Key, T, Compare, Allocator, Container>& rhs
    );

template<class Key, class T, class Compare, class Allocator, class Container>
bool operator>(
    const interval_map<Key, T, Compare, Allocator, Container>& lhs,
    const interval_map<Key, T, Compare, Allocator, Container>& rhs
    );

template<class Key, class T, class Compare, class Allocator, class Container>
bool operator>=(
    const interval_map<Key, T, Compare, Allocator, Container>& lhs,
    const interval_map<Key, T, Compare, Allocator, Container>& rhs
    );

namespace std {
    template<class Key, class T, class Compare, class Allocator, class Container>
    void swap(
        const interval_map<Key, T, Compare, Allocator, Container>& lhs,
        const interval_map<Key, T, Compare, Allocator, Container>& rhs
    );
}

template<
    class Key,
    class T,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<std::pair<const Key, T>>,
    class Container = std::map<Key, T, Compare, Allocator>
>
class interval_map
{
public:
    using container_type = Container;
    using key_type = typename Container::key_type;
    using mapped_type = typename Container::mapped_type;
    using value_type = typename Container::value_type;
    using key_compare = typename Container::key_compare;
    using allocator_type = typename Container::allocator_type;
    using pointer = typename Container::pointer;
    using const_pointer = typename Container::const_pointer;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using size_type = typename Container::size_type;
    using difference_type = typename Container::difference_type;
    using iterator = typename Container::iterator;
    using const_iterator = typename Container::const_iterator;
    using reverse_iterator = typename Container::reverse_iterator;
    using const_reverse_iterator = typename Container::const_reverse_iterator;
    using node_type = typename Container::node_type;
    using insert_return_type = typename Container::insert_return_type;

protected:
    /**
     * Default value.
     *
     * Keys map to this value if no match is found in the map.
     */
    mapped_type first_val_{};

    /**
     * Internal map.
     *
     * Map representing the intervals.
     */
    Container c_{};

public:
    /**
     * Constructor.
     *
     * @param first_val default value to which keys map if no match is found in the map.
     */
    interval_map(const T& first_val) :
        first_val_(first_val)
    {}

    /**
     * Constructor.
     *
     * @param first_val default value to which keys map if no match is found in the map.
     * @param c container to be assigned
     */
    interval_map(const T& first_val, const Container& c) :
        first_val_(first_val),
        c_(c)
    {}

    /**
     * Constructor.
     *
     * @param first_val default value to which keys map if no match is found in the map.
     * @param c container to be assigned
     */
    interval_map(const T& first_val, std::initializer_list<value_type> init) :
        first_val_(first_val),
        c_{ Container(init) }
    {}

    iterator begin() noexcept { return c_.begin(); }
    const_iterator begin() const noexcept { return c_.begin(); }
    iterator end() noexcept { return c_.end(); }
    const_iterator end() const noexcept { return c_.end(); }
    reverse_iterator rbegin() noexcept { return c_.rbegin(); }
    const_reverse_iterator rbegin() const noexcept { return c_.rbegin(); }
    reverse_iterator rend() noexcept { return c_.rend(); }
    const_reverse_iterator rend() const noexcept { return c_.rend(); }

    const_iterator cbegin() const noexcept { return c_.cbegin(); }
    const_iterator cend() const noexcept { return c_.cend(); }
    const_reverse_iterator crbegin() const noexcept { return c_.crbegin(); }
    const_reverse_iterator crend() const noexcept { return c_.crend(); }

    [[nodiscard]] bool empty() const noexcept { return c_.empty(); }
    size_type size() const { return c_.size(); }
    size_type max_size() const { return c_.max_size(); }

    /**
     * Sets the first value.
     */
    void set_first_val(const mapped_type& val) { first_val_ = val; }

    /**
     * Returns a const reference to the first value.
     */
    const mapped_type& get_first_val() const noexcept { return first_val_; }

    /**
     * Manually inserts a pair to the map.
     *
     * @param key the key to which the value maps
     * @param val the value to be assigned
     */
    void insert(const key_type& key, const mapped_type& val)
    {
        // If the map is empty and value is equal to first_val_, do nothing
        if (c_.empty() && val == first_val_)  return;

        // Find the position of the upper bound of key
        iterator it = c_.upper_bound(key);

        // Insert the value of key_end (emplace_hint is faster than insert_or_assign)
        it = c_.emplace_hint(it, key, val);
        // Make sure the value is assigned if the key already exists
        it->second = val;

        // Get the value of the element that comes before key
        mapped_type prev_val = (it == c_.begin() ? first_val_ : std::prev(it)->second);

        // Erase the current value if it's equal to the previous,
        // move forward otherwise
        if (val == prev_val) {
            it = c_.erase(it);
        }
        else {
            it++;
        }

        // Exit if the current element is the end
        if (it == c_.end())  return;

        // Get the value of the element that comes before the current element
        prev_val = (it == c_.begin() ? first_val_ : std::prev(it)->second);

        // Erase the current element if its value is equal to the value of
        // the previous element
        if (it->second == prev_val) {
            c_.erase(it);
        }
    }

    /**
     * Assigns `val` to the interval [`key_begin`, `key_end`).
     *
     * @param key_begin the first key (included) of the interval
     * @param key_end the last key (excluded) of the interval
     * @param val the value to be assigned
     */
    void insert_range(const key_type& key_begin, const key_type& key_end, const mapped_type& val)
    {
        // If the interval is empty, do nothing
        if (key_begin >= key_end)  return;

        // If the map is empty and value is equal to first_val_, do nothing
        if (c_.empty() && val == first_val_)  return;

        // Find the position of the upper bound of key_end
        iterator jt = c_.upper_bound(key_end);

        // Get the value to which key_end is mapped before the insertion
        mapped_type prev_val = (jt == c_.begin() ? first_val_ : std::prev(jt)->second);

        // Insert the value of key_end (emplace_hint is faster than insert_or_assign)
        jt = c_.emplace_hint(jt, key_end, prev_val);
        // Make sure the value is assigned if the key already exists
        jt->second = prev_val;

        // Insert the value of key_begin (emplace_hint is faster than insert_or_assign)
        iterator it = c_.emplace_hint(jt, key_begin, val);
        // Make sure the value is assigned if the key already exists
        it->second = val;

        // Get the value of the element that comes before key_begin
        prev_val = (it == c_.begin() ? first_val_ : std::prev(it)->second);

        // Erase all the previous values in the range (including key_begin
        // if its value is equal to the value of its previous element)
        if (val != prev_val)  it++;
        jt = c_.erase(it, jt);

        // Get the value of the element before the erased range
        prev_val = (jt == c_.begin() ? first_val_ : std::prev(jt)->second);

        // If the value after the erased range is equal to the value before the
        // erased range, erase it
        if (jt->second == prev_val) {
            c_.erase(jt);
        }
    }

    void swap(interval_map& rhs)
    {
        std::swap(first_val_, rhs.first_val_);
        c_.swap(rhs.c_);
    }

    /**
     * Returns a const reference to the value that is mapped to a key equivalent to `key`.
     *
     * @param key the key of the element to find
     * @return a const reference to the mapped value of the existing element whose key is equivalent to `key`.
     */
    const_reference at(const key_type& key) const
    {
        iterator it = c_.upper_bound(key);
        return (it == c_.begin() ? first_val_ : std::prev(it)->second);
    }

    /**
     * Returns the value of the internal container.
     *
     * @return the value of the internal container
     */
    const Container& get_container() const { return c_; }

    friend bool operator== <>(
        const interval_map<Key, T, Compare, Allocator, Container>& lhs,
        const interval_map<Key, T, Compare, Allocator, Container>& rhs
        );

    friend bool operator!= <>(
        const interval_map<Key, T, Compare, Allocator, Container>& lhs,
        const interval_map<Key, T, Compare, Allocator, Container>& rhs
        );

    friend bool operator< <>(
        const interval_map<Key, T, Compare, Allocator, Container>& lhs,
        const interval_map<Key, T, Compare, Allocator, Container>& rhs
        );

    friend bool operator<= <>(
        const interval_map<Key, T, Compare, Allocator, Container>& lhs,
        const interval_map<Key, T, Compare, Allocator, Container>& rhs
        );

    friend bool operator> <>(
        const interval_map<Key, T, Compare, Allocator, Container>& lhs,
        const interval_map<Key, T, Compare, Allocator, Container>& rhs
        );

    friend bool operator>= <>(
        const interval_map<Key, T, Compare, Allocator, Container>& lhs,
        const interval_map<Key, T, Compare, Allocator, Container>& rhs
        );
};

template<class Key, class T, class Compare, class Allocator, class Container>
bool operator==(
    const interval_map<Key, T, Compare, Allocator, Container>& lhs,
    const interval_map<Key, T, Compare, Allocator, Container>& rhs
    )
{
    return lhs.first_val_ == rhs.first_val_ && lhs.c_ == rhs.c_;
}

template<class Key, class T, class Compare, class Allocator, class Container>
bool operator!=(
    const interval_map<Key, T, Compare, Allocator, Container>& lhs,
    const interval_map<Key, T, Compare, Allocator, Container>& rhs
    )
{
    return lhs.first_val_ != rhs.first_val_ || lhs.c_ != rhs.c_;
}

template<class Key, class T, class Compare, class Allocator, class Container>
bool operator<(
    const interval_map<Key, T, Compare, Allocator, Container>& lhs,
    const interval_map<Key, T, Compare, Allocator, Container>& rhs
    )
{
    if (lhs.first_val_ < rhs.first_val_)  return true;
    return lhs.c_ < rhs.c_;
}

template<class Key, class T, class Compare, class Allocator, class Container>
bool operator<=(
    const interval_map<Key, T, Compare, Allocator, Container>& lhs,
    const interval_map<Key, T, Compare, Allocator, Container>& rhs
    )
{
    if (lhs.first_val_ > rhs.first_val_)  return false;
    return lhs.c_ <= rhs.c_;
}

template<class Key, class T, class Compare, class Allocator, class Container>
bool operator>(
    const interval_map<Key, T, Compare, Allocator, Container>& lhs,
    const interval_map<Key, T, Compare, Allocator, Container>& rhs
)
{
    if (lhs.first_val_ > rhs.first_val_)  return true;
    return lhs.c_ > rhs.c_;
}

template<class Key, class T, class Compare, class Allocator, class Container>
bool operator>=(
    const interval_map<Key, T, Compare, Allocator, Container>& lhs,
    const interval_map<Key, T, Compare, Allocator, Container>& rhs
    )
{
    if (lhs.first_val_ < rhs.first_val_)  return false;
    return lhs.c_ >= rhs.c_;
}

namespace std {
    template<class Key, class T, class Compare, class Allocator, class Container>
    void swap(
        const interval_map<Key, T, Compare, Allocator, Container>& lhs,
        const interval_map<Key, T, Compare, Allocator, Container>& rhs
    )
    {
        lhs.swap(rhs);
    }
}

#endif
