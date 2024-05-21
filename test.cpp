#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "interval_map.hpp"

template <
    typename Key,
    typename T,
    typename Map = std::map<Key, T>
>
void test_err(Map ref_map, Map res_map, int test_number)
{
    std::cerr << "Test " << test_number << " failed. Resulting map should be:\n";

    for (auto it = ref_map.begin(); it != ref_map.end(); it++) {
        std::cerr << "  " << it->first << " -> " << it->second << '\n';
    }

    std::cerr << "but is instead:\n";

    for (auto it = res_map.begin(); it != res_map.end(); it++) {
        std::cerr << "  " << it->first << " -> " << it->second << '\n';
    }

    exit(1);
}

void test_1()
{
    std::map<int, char> ref_map;
    ref_map[1] = 'B';
    ref_map[15] = 'A';

    interval_map<int, char> imap('A');
    imap.assign(1, 15, 'B');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 1);
}

void test_2()
{
    std::map<int, char> ref_map;
    ref_map[1] = 'B';
    ref_map[2] = 'C';
    ref_map[7] = 'B';
    ref_map[15] = 'A';

    interval_map<int, char> imap('A');
    imap.assign(1, 15, 'B');
    imap.assign(2, 7, 'C');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 2);
}

void test_3()
{
    std::map<int, char> ref_map;
    ref_map[1] = 'B';
    ref_map[2] = 'C';
    ref_map[4] = 'D';
    ref_map[12] = 'B';
    ref_map[15] = 'A';

    interval_map<int, char> imap('A');
    imap.assign(1, 15, 'B');
    imap.assign(2, 7, 'C');
    imap.assign(4, 12, 'D');


    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 3);
}

void test_4()
{
    std::map<int, char> ref_map;
    ref_map[1] = 'B';
    ref_map[2] = 'C';
    ref_map[4] = 'D';
    ref_map[10] = 'E';
    ref_map[14] = 'B';
    ref_map[15] = 'A';

    interval_map<int, char> imap('A');
    imap.assign(1, 15, 'B');
    imap.assign(2, 7, 'C');
    imap.assign(4, 12, 'D');
    imap.assign(10, 14, 'E');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 4);
}

void test_5()
{
    std::map<int, char> ref_map;
    ref_map[1] = 'B';
    ref_map[4] = 'D';
    ref_map[10] = 'E';
    ref_map[14] = 'B';
    ref_map[15] = 'A';

    interval_map<int, char> imap('A');
    imap.assign(1, 15, 'B');
    imap.assign(2, 7, 'C');
    imap.assign(4, 12, 'D');
    imap.assign(10, 14, 'E');
    imap.assign(2, 4, 'B');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 5);
}

void test_6()
{
    std::map<int, char> ref_map;
    ref_map[1] = 'B';
    ref_map[15] = 'A';

    interval_map<int, char> imap('A');
    imap.assign(1, 15, 'B');
    imap.assign(2, 7, 'C');
    imap.assign(4, 12, 'D');
    imap.assign(10, 14, 'E');
    imap.assign(2, 14, 'B');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 6);
}

void test_7()
{
    std::map<int, char> ref_map;
    ref_map[1] = 'B';
    ref_map[2] = 'C';
    ref_map[15] = 'A';

    interval_map<int, char> imap('A');
    imap.assign(1, 15, 'B');
    imap.assign(2, 7, 'C');
    imap.assign(4, 12, 'D');
    imap.assign(10, 14, 'E');
    imap.assign(3, 15, 'C');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 7);
}

void test_8()
{
    std::map<int, char> ref_map;

    interval_map<int, char> imap('A');
    imap.assign(1, 15, 'B');
    imap.assign(2, 7, 'C');
    imap.assign(4, 12, 'D');
    imap.assign(10, 14, 'E');
    imap.assign(1, 15, 'A');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 8);
}

void test_9()
{
    std::map<int, char> ref_map;
    ref_map[14] = 'B';
    ref_map[15] = 'A';

    interval_map<int, char> imap('A');
    imap.assign(1, 15, 'B');
    imap.assign(2, 7, 'C');
    imap.assign(4, 12, 'D');
    imap.assign(10, 14, 'E');
    imap.assign(1, 14, 'A');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 9);
}

void test_10()
{
    std::map<int, char> ref_map;
    ref_map[1] = 'B';
    ref_map[3] = 'C';
    ref_map[5] = 'A';

    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(3, 5, 'C');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 10);
}

void test_11()
{
    std::map<int, char> ref_map;
    ref_map[1] = 'B';
    ref_map[3] = 'C';
    ref_map[5] = 'D';
    ref_map[7] = 'A';

    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(3, 5, 'C');
    imap.assign(5, 7, 'D');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 11);
}

void test_12()
{
    std::map<int, char> ref_map;
    ref_map[1] = 'B';
    ref_map[3] = 'C';
    ref_map[7] = 'A';

    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(3, 5, 'C');
    imap.assign(5, 7, 'C');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 12);
}

void test_13()
{
    std::map<int, char> ref_map;

    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(3, 5, 'C');
    imap.assign(1, 7, 'A');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 13);
}

void test_14()
{
    std::map<int, char> ref_map;
    ref_map[1] = 'B';
    ref_map[5] = 'A';

    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.assign(3, 5, 'C');
    imap.assign(3, 5, 'B');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 14);
}

void test_15()
{
    std::map<int, char> ref_map;
    ref_map[1] = 'B';
    ref_map[15] = 'A';

    interval_map<int, char> imap('A');
    imap.assign(1, 15, 'B');
    imap.set_pair(2, 'B');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 15);
}

void test_16()
{
    std::map<int, char> ref_map;
    ref_map[1] = 'B';

    interval_map<int, char> imap('A');
    imap.assign(1, 3, 'B');
    imap.set_pair(3, 'B');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 16);
}

void test_17()
{
    std::map<int, char> ref_map;
    ref_map[1] = 'B';
    ref_map[2] = 'C';
    ref_map[15] = 'A';

    interval_map<int, char> imap('A');
    imap.assign(1, 15, 'B');
    imap.set_pair(2, 'C');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 17);
}

void test_18()
{
    std::map<int, char> ref_map;
    ref_map[1] = 'C';
    ref_map[15] = 'A';

    interval_map<int, char> imap('A');
    imap.assign(1, 15, 'B');
    imap.set_pair(1, 'C');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 18);
}

void test_19()
{
    std::map<int, char> ref_map;

    interval_map<int, char> imap('A');
    imap.assign(1, 15, 'B');
    imap.set_pair(1, 'A');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 19);
}

void test_20()
{
    std::map<int, char> ref_map;

    interval_map<int, char> imap('A');
    imap.assign(14, 15, 'B');
    imap.assign(1, 15, 'A');

    std::map<int, char> res_map = imap.get_map();

    if (ref_map != res_map)  test_err<int, char>(ref_map, res_map, 19);
}

void (*tests[])() = { test_1, test_2, test_3, test_4, test_5, test_6, test_7, test_8, test_9, test_10, test_11, test_12, test_13, test_14, test_15, test_16, test_17, test_18, test_19, test_20 };


int main()
{
    std::srand(0);

    for (auto it = std::cbegin(tests); it != std::cend(tests); it++) {
        (*it)();
    }

    std::cout << "All test passed!\n";

    std::cout << "Benchmarking assign function...\n";

    const auto start{ std::chrono::steady_clock::now() };

    interval_map<int, int> imap(0);

    for (int i = 1; i < 2000; i++) {
        imap.assign(rand() % 100, rand() % 100, rand() % 20);
    }

    const auto end{ std::chrono::steady_clock::now() };
    const std::chrono::duration<double> elapsed_seconds{ end - start };

    std::cout << "Benchmark completed in " << elapsed_seconds.count() << " seconds.\n";

    return 0;
}
