#include <cassert>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>

#include "interval_map.hpp"

#define compare_not_passed( a, b ) { \
    std::cerr << "Test \"" << __FUNCTION__ << "\" not passed on " << a << " and " << b << "\n"; \
    exit(1); \
}

#define assert_ref( x, ref ) { \
    if (x != ref) { \
        std::cerr << "Test \"" << __FUNCTION__ << "\" not passed: map " << x << " should be " << ref << "\n"; \
        exit(1); \
    } \
}


template<class Key, class T, class Compare, class Allocator, class Container>
std::ostream& operator<<(std::ostream& os, const interval_map<Key, T, Compare, Allocator, Container>& imap)
{
    auto it = imap.begin();

    os << "{" << imap.get_first_val() << ", ";

    if (it != imap.end()) {
        os << "(" << it->first << ", " << it->second << ")";
        it++;
    }

    for (; it != imap.end(); it++) {
        os << ", (" << it->first << ", " << it->second << ")";
    }

    os << "}";

    return os;
}


void test_operator_eq()
{
    interval_map<int, char> a('A', {  });
    interval_map<int, char> b('A', {  });
    if (!(a == b))  compare_not_passed(a, b);

    a = interval_map<int, char>('A', { {3, 'B'} });
    b = interval_map<int, char>('A', { {3, 'B'} });
    if (!(a == b))  compare_not_passed(a, b);

    a = interval_map<int, char>('A', { {3, 'B'} });
    b = interval_map<int, char>('B', { {3, 'B'} });
    if (a == b)  compare_not_passed(a, b);

    a = interval_map<int, char>('A', { {3, 'B'} });
    b = interval_map<int, char>('A', { {3, 'C'} });
    if (a == b)  compare_not_passed(a, b);
}

void test_operator_neq()
{
    interval_map<int, char> a('A', {  });
    interval_map<int, char> b('A', {  });
    if (a != b)  compare_not_passed(a, b);

    a = interval_map<int, char>('A', { {3, 'B'} });
    b = interval_map<int, char>('A', { {3, 'B'} });
    if (a != b)  compare_not_passed(a, b);

    a = interval_map<int, char>('A', { {3, 'B'} });
    b = interval_map<int, char>('B', { {3, 'B'} });
    if (!(a != b))  compare_not_passed(a, b);

    a = interval_map<int, char>('A', { {3, 'B'} });
    b = interval_map<int, char>('A', { {3, 'C'} });
    if (!(a != b))  compare_not_passed(a, b);
}


void test_set_first_val()
{
    interval_map<int, char> ref_imap('B', {  });

    interval_map<int, char> imap('A', {  });
    imap.set_first_val('B');

    assert_ref(imap, ref_imap);
}

void test_set_first_val_eq_to_first_in_set()
{
    interval_map<int, char> ref_imap('B', { {6, 'A'} });

    interval_map<int, char> imap('A', { {3, 'B'}, {6, 'A'} });
    imap.set_first_val('B');

    assert_ref(imap, ref_imap);
}


void test_insert()
{
    interval_map<int, char> ref_imap('A', { {3, 'B'} });

    interval_map<int, char> imap('A', {  });
    imap.insert(3, 'B');

    assert_ref(imap, ref_imap);
}

void test_insert_overwrite()
{
    interval_map<int, char> ref_imap('A', { {3, 'B'}, {6, 'D'}, {9, 'A'} });

    interval_map<int, char> imap('A', { {3, 'B'}, {6, 'C'}, {9, 'A'} });
    imap.insert(6, 'D');

    assert_ref(imap, ref_imap);
}

void test_insert_overwrite_same_as_next()
{
    interval_map<int, char> ref_imap('A', { {3, 'B'}, {6, 'A'} });

    interval_map<int, char> imap('A', { {3, 'B'}, {6, 'C'}, {9, 'A'} });
    imap.insert(6, 'A');

    assert_ref(imap, ref_imap);
}

void test_insert_overwrite_same_as_prev()
{
    interval_map<int, char> ref_imap('A', { {3, 'B'}, {9, 'A'} });

    interval_map<int, char> imap('A', { {3, 'B'}, {6, 'C'}, {9, 'A'} });
    imap.insert(6, 'B');

    assert_ref(imap, ref_imap);
}

void test_insert_overwrite_first()
{
    interval_map<int, char> ref_imap('A', { {6, 'C'}, {9, 'A'} });

    interval_map<int, char> imap('A', { {3, 'B'}, {6, 'C'}, {9, 'A'} });
    imap.insert(3, 'A');

    assert_ref(imap, ref_imap);
}

void test_insert_overwrite_penultimate()
{
    interval_map<int, char> ref_imap('A', { {3, 'B'}, {6, 'A'} });

    interval_map<int, char> imap('A', { {3, 'B'}, {6, 'C'}, {9, 'A'} });
    imap.insert(6, 'A');

    assert_ref(imap, ref_imap);
}

void test_insert_overwrite_last()
{
    interval_map<int, char> ref_imap('A', { {3, 'B'}, {6, 'C'} });

    interval_map<int, char> imap('A', { {3, 'B'}, {6, 'C'}, {9, 'A'} });
    imap.insert(9, 'C');

    assert_ref(imap, ref_imap);
}


void test_insert_range()
{
    interval_map<int, char> ref_imap('A', { {3, 'B'}, {12, 'A'} });

    interval_map<int, char> imap('A');
    imap.insert_range(3, 12, 'B');

    assert_ref(imap, ref_imap);
}

void test_insert_range_overwrite_between()
{
    interval_map<int, char> ref_imap('A', { {3, 'B'}, {6, 'C'}, {9, 'B'}, {12, 'A'} });

    interval_map<int, char> imap('A');
    imap.insert_range(3, 12, 'B');
    imap.insert_range(6, 9, 'C');

    assert_ref(imap, ref_imap);
}

void test_insert_range_overwrite_first()
{
    interval_map<int, char> ref_imap('A', { {3, 'B'}, {6, 'D'}, {8, 'C'}, {9, 'B'}, {12, 'A'} });

    interval_map<int, char> imap('A');
    imap.insert_range(3, 12, 'B');
    imap.insert_range(6, 9, 'C');
    imap.insert_range(6, 8, 'D');

    assert_ref(imap, ref_imap);
}

void test_insert_range_overwrite_second()
{
    interval_map<int, char> ref_imap('A', { {3, 'B'}, {6, 'C'}, {7, 'D'}, {9, 'B'}, {12, 'A'} });

    interval_map<int, char> imap('A');
    imap.insert_range(3, 12, 'B');
    imap.insert_range(6, 9, 'C');
    imap.insert_range(7, 9, 'D');

    assert_ref(imap, ref_imap);
}

void test_insert_range_overwrite_first_and_second()
{
    interval_map<int, char> ref_imap('A', { {3, 'B'}, {6, 'D'}, {9, 'B'}, {12, 'A'} });

    interval_map<int, char> imap('A');
    imap.insert_range(3, 12, 'B');
    imap.insert_range(6, 9, 'C');
    imap.insert_range(6, 9, 'D');

    assert_ref(imap, ref_imap);
}

void test_insert_range_overwrite_cross()
{
    interval_map<int, char> ref_imap('A', { {3, 'B'}, {5, 'D'}, {7, 'C'}, {9, 'B'}, {12, 'A'} });

    interval_map<int, char> imap('A');
    imap.insert_range(3, 12, 'B');
    imap.insert_range(6, 9, 'C');
    imap.insert_range(5, 7, 'D');

    assert_ref(imap, ref_imap);
}

void test_insert_range_overwrite_all()
{
    interval_map<int, char> ref_imap('A', { {3, 'D'}, {12, 'A'} });

    interval_map<int, char> imap('A');
    imap.insert_range(3, 12, 'B');
    imap.insert_range(6, 9, 'C');
    imap.insert_range(3, 12, 'D');

    assert_ref(imap, ref_imap);
}

void test_insert_range_first_val()
{
    interval_map<int, char> ref_imap('A', { {3, 'B'}, {6, 'C'}, {7, 'A'} });

    interval_map<int, char> imap('A');
    imap.insert_range(3, 12, 'B');
    imap.insert_range(6, 9, 'C');
    imap.insert_range(7, 12, 'A');

    assert_ref(imap, ref_imap);
}

void test_insert_range_first_val_overwrite_all()
{
    interval_map<int, char> ref_imap('A', { });

    interval_map<int, char> imap('A');
    imap.insert_range(3, 12, 'B');
    imap.insert_range(6, 9, 'C');
    imap.insert_range(3, 12, 'A');

    assert_ref(imap, ref_imap);
}

void test_insert_range_extend_previous()
{
    interval_map<int, char> ref_imap('A', { {3, 'B'}, {7, 'C'}, {9, 'B'}, {12, 'A'} });

    interval_map<int, char> imap('A');
    imap.insert_range(3, 12, 'B');
    imap.insert_range(6, 9, 'C');
    imap.insert_range(4, 7, 'B');

    assert_ref(imap, ref_imap);
}

void test_insert_range_extend_next()
{
    interval_map<int, char> ref_imap('A', { {3, 'B'}, {5, 'C'}, {9, 'B'}, {12, 'A'} });

    interval_map<int, char> imap('A');
    imap.insert_range(3, 12, 'B');
    imap.insert_range(6, 9, 'C');
    imap.insert_range(5, 7, 'C');

    assert_ref(imap, ref_imap);
}


void test_swap()
{
    interval_map<int, char> ref_1('A', { {3, 'B'}, {6, 'C'}, {9, 'B'}, {12, 'A'} });
    interval_map<int, char> ref_2('D', { {3, 'E'}, {6, 'F'}, {9, 'E'}, {12, 'D'} });

    interval_map<int, char> map_1('D', { {3, 'E'}, {6, 'F'}, {9, 'E'}, {12, 'D'} });
    interval_map<int, char> map_2('A', { {3, 'B'}, {6, 'C'}, {9, 'B'}, {12, 'A'} });

    std::swap(map_1, map_2);

    assert_ref(map_1, ref_1);
    assert_ref(map_2, ref_2);
}


std::chrono::duration<double> benchmark_imap(
    interval_map<int, int>& imap,
    int n_tests,
    int key_begin_size,
    int key_end_size,
    int val_size
)
{
    const auto start{ std::chrono::steady_clock::now() };
    for (int i = 0; i < n_tests; i++) {
        imap.insert_range(rand() % key_begin_size, rand() % key_end_size, rand() % val_size);
    }
    const auto end{ std::chrono::steady_clock::now() };
    const std::chrono::duration<double> elapsed_seconds{ end - start };
    return elapsed_seconds;
}


int main()
{
    void (*tests[])() = {
        test_operator_eq,
        test_operator_neq,
        test_set_first_val,
        test_set_first_val_eq_to_first_in_set,
        test_insert,
        test_insert_overwrite,
        test_insert_overwrite_same_as_next,
        test_insert_overwrite_same_as_prev,
        test_insert_overwrite_first,
        test_insert_overwrite_penultimate,
        test_insert_overwrite_last,
        test_insert_range,
        test_insert_range_overwrite_between,
        test_insert_range_overwrite_first,
        test_insert_range_overwrite_second,
        test_insert_range_overwrite_first_and_second,
        test_insert_range_overwrite_cross,
        test_insert_range_overwrite_all,
        test_insert_range_first_val,
        test_insert_range_first_val_overwrite_all,
        test_insert_range_extend_previous,
        test_insert_range_extend_next,
        test_swap
    };

    for (auto it = std::cbegin(tests); it != std::cend(tests); it++) {
        (*it)();
    }

    std::cout << "All test passed!\nBenchmarking insert_range function...\n";

    interval_map<int, int> imap(0);
    std::srand(0);
    const std::chrono::duration<double> elapsed_seconds = benchmark_imap(imap, 2000, 100, 100, 20);
    std::cout << "Benchmark completed in " << elapsed_seconds.count() << " seconds.\n";

    return 0;
}
