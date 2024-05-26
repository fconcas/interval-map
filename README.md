# Interval Map

Fork of [Interval Map by Daniel Gorbatov](https://github.com/projected1/interval-map/tree/master)

Interval map is a sorted associative container that contains key-value pairs with unique keys, and works by using the C++ STL Map class. It is designed to be efficient when intervals of consecutive keys map to the same value.

Each key-value pair $(k, v)$ means that the value $v$ is associated with all keys with a value greater or equal than $k$ and lower than the key in the following pair. If $(k_1, v_1)$ and $(k_2, v_2)$ are consecutive pairs, all keys in the range $[k_1, k_2)$ map to the value $v_1$. Interval map also contains an initial value, to which keys map if they do not belong to any interval.

For example, in an interval map with an initial value $A$ and a map containing the pairs $(2, B)$ and $(5, A)$, the values encoded by the interval map are:

```text
...
-2 -> A
-1 -> A
 0 -> A
 1 -> A
 2 -> B
 3 -> B
 4 -> B
 5 -> A
 6 -> A
 7 -> A
...
```

## Project files

- `interval_map.h` contains the actual implementation of the data structure.
- `test.cpp` contains the tests, and can be compiled using the `MAKEFILE`.

## Specifications

- Consecutive map entries must not contain the same value (e.g. consecutive entries $(1, A)$ and $(3, A)$ are not valid).
- Key type implements the `std::less` operator (but no other comparison operators).
- Value type is hashable (or implements the equality operator).
- Map entries must be modified by implementing a function `insert_range(key_begin, key_end, val)`, which assigns and overwrites value `val` ($v$) to keys between `key_begin` ($k_1$) included and `key_end` ($k_2$) excluded, that is, $[k_1, k_2)$.
- An additional function `insert(key, val)`, which manually sets a pair (if doesn't violate the first specification) is provided. This can be useful, for example, to set a last value.
- When an interval $[k_1, k_2) \rightarrow v$ is inserted, it must overwrite all values that belonged to such interval before insertion.
- If an interval replaces all intervals in the map, and the value is the map's initial value, the whole map should be emptied.
