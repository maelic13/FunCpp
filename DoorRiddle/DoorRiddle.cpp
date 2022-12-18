#include <chrono>
#include <iostream>
#include <random>

using namespace std;
using namespace std::chrono;

typedef high_resolution_clock Clock;
static int g_seed;


inline int fast_rand() {
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}

long long play_game(const long long cycles, const bool change_choice) {
    constexpr bool doors[] = {false, true, false};
    long long successful = 0;

    for (int i{}; i < cycles; i++) {
        if (change_choice != doors[fast_rand() % 3 + 1]) {
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
    random_device rd;
    g_seed = (int) time(nullptr);

    Clock::time_point start = Clock::now();
    auto successful = play_game(cycles, false);
    print_results(successful, cycles, false, Clock::now() - start);

    start = Clock::now();
    successful = play_game(cycles, true);
    print_results(successful, cycles, true, Clock::now() - start);

    return 0;
}
