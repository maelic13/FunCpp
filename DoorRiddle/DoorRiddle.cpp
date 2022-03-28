#include <chrono>
#include <iostream>

using namespace std;


unsigned long xorshf96(void) {
    static unsigned long x = 123456789, y = 362436069, z = 521288629;
    unsigned long t;

    x ^= x << 16;
    x ^= x >> 5;
    x ^= x << 1;

    t = x;
    x = y;
    y = z;
    z = t ^ x ^ y;

    return z % 3;
}


long long PlayGame(long long cycles, bool changeChoice) {
    bool doors[] = { false, true, false };
    long long successful = 0;

    for (int i{}; i < cycles; i++) {
        if (changeChoice != doors[xorshf96()]) {
            successful += 1;
        }
    }

    return successful;
}


void print_results(long long successful, long long cycles, bool changeChoice, std::chrono::high_resolution_clock::time_point start) {
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::duration<double> sec;

    auto elapsed = sec(Clock::now() - start).count();
    cout << endl;
    cout << "Change = " << changeChoice << ". Time elapsed: " << elapsed * 1000 << " ms." << endl;
    cout << successful << " successful tries, " << cycles << " total. Success rate " << static_cast<double>(successful) / cycles * 100. << " %." << endl;
    cout << "Speed = " << cycles / elapsed / 1000000 << " Miter/s." << endl;
}


int main()
{
    auto cycles = 1000000000;

    typedef std::chrono::high_resolution_clock Clock;
    Clock::time_point start = Clock::now();
    auto successful = PlayGame(cycles, false);
    print_results(successful, cycles, false, start);

    start = Clock::now();
    successful = PlayGame(cycles, true);
    print_results(successful, cycles, true, start);
}