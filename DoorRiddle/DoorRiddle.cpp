#include <chrono>
#include <iostream>

using namespace std;


unsigned long xorshf96(void) {
    static unsigned long x = 123456789, y = 362436069, z = 521288629;

    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    const unsigned long t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

    return z % 3;
}


long long play_game(const long long cycles, const bool change_choice) {
    constexpr bool doors[] = { false, true, false };
    long long successful = 0;

    for (int i{}; i < cycles; i++) {
        if (change_choice != doors[xorshf96()]) {
            successful += 1;
        }
    }

    return successful;
}


void print_results(const long long successful, const long long cycles, const bool change_choice, const double elapsed) {
    cout << endl;
    cout << "Change = " << change_choice << ". Time elapsed: " << elapsed * 1000 << " ms." << endl;
    cout << successful << " successful tries, " << cycles << " total. Success rate " << static_cast<double>(successful)
        / cycles * 100. << " %." << endl;
    cout << "Speed = " << cycles / elapsed / 1000000 << " Miter/s." << endl;
}


int main()
{
    constexpr auto cycles = 1000000000;

    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::duration<double> sec;

    Clock::time_point start = Clock::now();
    auto successful = play_game(cycles, false);
    print_results(successful, cycles, false, sec(Clock::now() - start).count());

    start = Clock::now();
    successful = play_game(cycles, true);
    print_results(successful, cycles, true, sec(Clock::now() - start).count());
}