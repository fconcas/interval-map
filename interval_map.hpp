#ifndef __INTERVAL_MAP_H
#define __INTERVAL_MAP_H

#include <algorithm>
#include <functional>
#include <map>

/**
 * Class implementing interval map.
 *
 * @tparam Key the type of the key
 * @tparam T the type of the values
 * @tparam Compare callable defining a strict weak ordering for the keys
 */
template<
    typename Key,
    typename T,
    typename Compare = std::less<Key>
>
class interval_map
{
private:
    /**
     * Default value.
     *
     * Keys map to this value if no match is found in the map.
     */
    T initial_val_{};

    /**
     * Internal map.
     *
     * Map representing the intervals.
     */
    std::map<Key, T, Compare>* map_{};

public:
    /**
     * Constructor.
     *
     * @param initial_val default value to which keys map if no match is found in the map.
     */
    interval_map(const T& initial_val) :
        initial_val_{ initial_val },
        map_{ new std::map<Key, T, Compare>() }
    {}

    /**
     * Destructor.
     */
    ~interval_map() { delete map_; }

    /**
     * Assigns `val` to the interval [`key_begin`, `key_end`).
     *
     * @param key_begin the first key (included) of the interval
     * @param key_end the last key (excluded) of the interval
     * @param val the value to be assigned
     */
    void assign(const Key& key_begin, const Key& key_end, const T& val);

    /**
     * Manually sets a pair to the map.
     *
     * @param key the key to which the value maps
     * @param val the value to be assigned
     */
    void set_pair(const Key& key, const T& val);

    /**
     * Returns a reference to the value that is mapped to a key equivalent to `key`.
     *
     * @param key the key of the element to find
     * @return a reference to the mapped value of the existing element whose key is equivalent to `key`.
     */
    const T& operator[](const Key& key) const
    {
        auto it = map_->upper_bound(key);
        return (it == map_->begin() ? initial_val_ : std::prev(it)->second);
    }

    /**
     * Returns the value of the internal map.
     * 
     * Used for testing purposes only.
     *
     * @return the value of the internal map
     */
    const std::map<Key, T, Compare> get_map() const { return *map_; }
};


template<typename Key, typename T, typename Compare>
void interval_map<Key, T, Compare>::assign(const Key& key_begin, const Key& key_end, const T& val)
{
    // If the interval is empty, do nothing
    if (key_begin >= key_end)  return;

    // If the map is empty and value is equal to initial_val_, do nothing
    if (map_->empty() && val == initial_val_)  return;

    // Find the position of the upper bound of key_end
    auto jt = map_->upper_bound(key_end);

    // Get the value to which key_end is mapped before the insertion
    T prev_val = (jt == map_->begin() ? initial_val_ : std::prev(jt)->second);

    // Insert the value of key_end (emplace_hint is faster than insert_or_assign)
    jt = map_->emplace_hint(jt, key_end, prev_val);
    // Make sure the value is assigned if the key already exists
    jt->second = prev_val;

    // Insert the value of key_begin (emplace_hint is faster than insert_or_assign)
    auto it = map_->emplace_hint(jt, key_begin, val);
    // Make sure the value is assigned if the key already exists
    it->second = val;

    // Get the value of the element that comes before key_begin
    prev_val = (it == map_->begin() ? initial_val_ : std::prev(it)->second);

    // Erase all the previous values in the range (including key_begin
    // if its value is equal to the value of its previous element)
    if (val != prev_val)  it++;
    jt = map_->erase(it, jt);

    // Get the value of the element before the erased range
    prev_val = (jt == map_->begin() ? initial_val_ : std::prev(jt)->second);

    // If the value after the erased range is equal to the value before the
    // erased range, erase it
    if (jt->second == prev_val) {
        map_->erase(jt);
    }
}

template<typename Key, typename T, typename Compare>
void interval_map<Key, T, Compare>::set_pair(const Key& key, const T& val)
{
    // If the map is empty and value is equal to initial_val_, do nothing
    if (map_->empty() && val == initial_val_)  return;

    // Find the position of the upper bound of key
    auto it = map_->upper_bound(key);

    // Insert the value of key_end (emplace_hint is faster than insert_or_assign)
    it = map_->emplace_hint(it, key, val);
    // Make sure the value is assigned if the key already exists
    it->second = val;

    // Get the value of the element that comes before key
    T prev_val = (it == map_->begin() ? initial_val_ : std::prev(it)->second);

    // Erase the current value if it's equal to the previous,
    // move forward otherwise
    if (val == prev_val) {
        it = map_->erase(it);
    }
    else {
        it++;
    }

    // Exit if the current element is the end
    if (it == map_->end())  return;

    // Get the value of the element that comes before the current element
    prev_val = (it == map_->begin() ? initial_val_ : std::prev(it)->second);

    // Erase the current element if its value is equal to the value of
    // the previous element
    if (it->second == prev_val) {
        map_->erase(it);
    }
}

#endif
