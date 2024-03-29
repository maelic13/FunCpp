#include <chrono>
#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <cmath>

using namespace std;
using namespace std::chrono;

typedef high_resolution_clock hr_clock;


unsigned long xorshf96() {
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


void PlayGame(int cycles, bool changeChoice, int &successful) {
    bool doors[] = {false, true, false};

    for (int i = 0; i < cycles; i++) {
        if (changeChoice != doors[xorshf96()]) {
            successful++;
        }
    }
}

void PlayGameMultiThreaded(int n, const int maxCycles, const int trash, bool changeChoice, int *successful) {
    std::vector<thread> threads(n);
    int cycles = maxCycles / n;

    for (int i = 0; i < n; i++) {
        if (i == (n - 1))
            cycles += trash;
        threads[i] = thread(PlayGame, cycles, changeChoice, ref(successful[i]));
    }
    for (auto &th: threads)
        th.join();
}


void print_results(int successful, int cycles, bool changeChoice, long elapsed) {

    cout << endl;
    cout << "Change = " << changeChoice << ". Time elapsed: " << elapsed << " ms." << endl;
    cout << successful << " successful tries, " << cycles << " total. Success rate "
         << static_cast<double>(successful) / cycles * 100. << " %." << endl;
    cout << "Speed = " << cycles / elapsed / 1000 << " Miter/s." << endl;
}


int main() {
    const int maxCycles = 1000000000;
    long elapsedPrev = maxCycles;
    long elapsed2Prev = maxCycles;
    long elapsed = 0;
    long elapsed2 = 0;
    int cycles = 0;

    int minN = 608; //minimum number of threads
    int maxN = 608; //maximum number of threads
    const int step = 32; //number of threads as step

    int successful[3000];

    const bool wantIterativeCMD = false;

    for (int n = minN; n <= maxN; n += step) { //n division
        int cycles = maxCycles / n;
        const int trash = llroundl((((double) maxCycles / (double) n) - (double) cycles) * n);

        // prvni
        fill(successful, successful + n, 0);
        hr_clock::time_point start = hr_clock::now();
        PlayGameMultiThreaded(n, maxCycles, trash, false, successful);

        if (wantIterativeCMD) {
            elapsed = duration_cast<milliseconds>(hr_clock::now() - start).count();
            cout << "n-threads = " << n << ", Change = 0" << ", Time elapsed : " << elapsed << " ms." << endl;
        } else
            elapsed = duration_cast<milliseconds>(hr_clock::now() - start).count();
        print_results(accumulate(successful, successful + n, 0), (cycles * n) + trash, false,
                      duration_cast<milliseconds>(hr_clock::now() - start).count());



        // druhy
        fill(successful, successful + n, 0);
        start = hr_clock::now();
        PlayGameMultiThreaded(n, maxCycles, trash, true, successful);

        if (wantIterativeCMD) {
            elapsed2 = duration_cast<milliseconds>(hr_clock::now() - start).count();
            cout << "n-threads = " << n << ", Change = 1" << ", Time elapsed : " << elapsed2 * 1000 << " ms." << endl;
            cout << endl;
            if (elapsed + elapsed2 < elapsedPrev + elapsed2Prev) {
                elapsedPrev = elapsed;
                elapsed2Prev = elapsed2;
                minN = n;
            }
        } else
            print_results(accumulate(successful, successful + n, 0), (cycles * n) + trash, true,
                          duration_cast<milliseconds>(hr_clock::now() - start).count());
    }

    if (wantIterativeCMD)
        cout << "min time: " << "n-threads = " << minN << ", Time elapsed 0: " << elapsedPrev * 1000 << " ms"
             << ", Time elapsed 1: " << elapsed2Prev * 1000 << " ms." << endl;
    return 0;
}
