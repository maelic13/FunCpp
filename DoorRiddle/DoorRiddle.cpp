#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

typedef high_resolution_clock hr_clock;

static uint32_t g_seed;


uint32_t fast_rand() {
    g_seed = 16807 * g_seed;
    return g_seed >> 16 & 2147483647;
}


uint64_t play_game(uint64_t cycles, bool change_choice) {
    constexpr bool doors[] = {false, true, false};
    uint64_t successful = 0;

    for (uint64_t i = 0; i < cycles; i++) {
        if (change_choice != doors[fast_rand() % 3]) {
            successful += 1;
        }
    }

    return successful;
}


void print_results(uint64_t successful, uint64_t cycles, bool change_choice, hr_clock::duration duration) {
    cout << endl;
    cout << "Change = " << boolalpha << change_choice << ". Time elapsed: "
         << duration_cast<milliseconds>(duration).count() << " ms." << endl;
    cout << successful << " successful tries, " << cycles << " total. Success rate "
         << static_cast<double>(successful) / static_cast<double>(cycles) * 100 << " %." << endl;
    cout << "Speed = " << cycles / duration_cast<microseconds>(duration).count() << " Miter/s." << endl;
}


int main() {
    constexpr uint64_t cycles = 1000000000;
    g_seed = time(nullptr);

    hr_clock::time_point start = hr_clock::now();
    auto successful = play_game(cycles, false);
    print_results(successful, cycles, false, hr_clock::now() - start);

    start = hr_clock::now();
    successful = play_game(cycles, true);
    print_results(successful, cycles, true, hr_clock::now() - start);

    return 0;
}
