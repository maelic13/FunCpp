#include <chrono>
#include <iostream>
#include <random>

using namespace std;
using namespace std::chrono;

typedef high_resolution_clock Clock;
static int g_seed;


u_int32_t fast_rand() {
    g_seed = 16807 * g_seed;
    return g_seed >> 16 & 2147483647;
}

u_int64_t play_game(u_int64_t cycles, bool change_choice) {
    constexpr bool doors[] = {false, true, false};
    u_int64_t successful = 0;

    for (u_int64_t i = 0; i < cycles; i++) {
        if (change_choice != doors[fast_rand() % 3]) {
            successful += 1;
        }
    }

    return successful;
}


void print_results(u_int64_t successful, u_int64_t cycles, bool change_choice, auto duration) {
    cout << endl;
    cout << "Change = " << boolalpha << change_choice << ". Time elapsed: "
         << duration_cast<milliseconds>(duration).count() << " ms." << endl;
    cout << successful << " successful tries, " << cycles << " total. Success rate "
         << static_cast<double>(successful) / static_cast<double>(cycles) * 100 << " %." << endl;
    cout << "Speed = " << cycles / duration_cast<microseconds>(duration).count() << " Miter/s." << endl;
}


int main() {
    constexpr u_int64_t cycles = 1000000000;
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
