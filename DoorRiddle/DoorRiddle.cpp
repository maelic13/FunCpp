#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

typedef high_resolution_clock Clock;


unsigned long xorshf96() {
    static unsigned long x = 123456789, y = 362436069, z = 521288629;

    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    const unsigned long t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

    return z;
}


unsigned long xorshf96_modulo() {
    return xorshf96() % 3;
}

long long play_game(const long long cycles, const bool change_choice) {
    constexpr bool doors[] = {false, true, false};
    long long successful = 0;

    for (int i{}; i < cycles; i++) {
        if (change_choice != doors[xorshf96_modulo()]) {
            successful += 1;
        }
    }

    return successful;
}


void print_results(const long long successful, const long long cycles, const bool change_choice, auto duration) {
    cout << endl;
    cout << "Change = " << boolalpha << change_choice << ". Time elapsed: "
         << duration_cast<milliseconds>(duration).count() << " ms." << endl;
    cout << successful << " successful tries, " << cycles << " total. Success rate "
         << static_cast<double>(successful) / cycles * 100. << " %." << endl;
    cout << "Speed = " << cycles / duration_cast<microseconds>(duration).count() << " Miter/s." << endl;
}


int main() {
    constexpr auto cycles = 1000000000;

    Clock::time_point start = Clock::now();
    auto successful = play_game(cycles, false);
    print_results(successful, cycles, false, Clock::now() - start);

    start = Clock::now();
    successful = play_game(cycles, true);
    print_results(successful, cycles, true, Clock::now() - start);

    return 0;
}
