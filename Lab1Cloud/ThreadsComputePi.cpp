#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <algorithm>
#include <execution>
#include <time.h>

double ComputePIpart(int start, int end) {
    double pi = 1.0;
    double sign = -1;
    for (int i = start; i <= end; i++) {
        pi += sign / (2.0 * i + 1.0);
        sign = -sign;
    }
    return pi;
}

double compute_pi_thread(int iterations, int NumOfThreads) {
    std::vector<std::thread> threads(NumOfThreads);
    std::vector<double> pi_parts(NumOfThreads);
    int step = iterations / NumOfThreads;
    for (int i = 0; i < NumOfThreads; i++) {
        int start = i * step + 1;
        int end = (i == NumOfThreads - 1) ? iterations : (i + 1) * step;
        threads[i] = std::thread([&pi_parts, i, start, end] { pi_parts[i] = ComputePIpart(start, end); });
    }
    for (auto& thread : threads) { thread.join(); }
    double pi = 0.0;
    for (auto& pi_part : pi_parts) {
        pi += pi_part;
    }
    pi -= NumOfThreads - 1;
    pi *= 4.0;
    return pi;
}
double compute_pi_async(int iterations, int NumOfTasks) {
    std::vector<std::future<double>> futures(NumOfTasks);
    int step = iterations / NumOfTasks;
    for (int i = 0; i < NumOfTasks; i++) {
        int start = i * step + 1;
        int end = (i == NumOfTasks - 1) ? iterations : (i + 1) * step;
        futures[i] = std::async(std::launch::async, ComputePIpart, start, end);
    }
    double pi = 0.0;
    for (auto& future : futures) {
        pi += future.get();
    }
    pi -= NumOfTasks -1;
    pi *= 4.0;
    return pi;
}

int main() {
    int iterations = 1e9;
    int NumOfThreads = 4;
    clock_t begin, end;
    time_t startTime, endTime;
    //standart
    begin = clock();
    double pi = 4* ComputePIpart(1,iterations);
    end = clock();
    // print
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    std::cout << "pi = " << std::setprecision(20) << pi << "\t" << "Time:" << std::fixed << time_spent << std::cout.precision(5) << " sec " << std::endl;
    //threads
    begin = clock();
    pi = compute_pi_thread(iterations, NumOfThreads);
    end = clock();
    // print
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    std::cout << "pi = " << std::setprecision(20) << pi << "\t" << "Time:" << std::fixed << time_spent << std::cout.precision(5) << " sec " << std::endl;
    // async
    begin = clock();
    pi = compute_pi_async(iterations, NumOfThreads);
    end = clock();
    // print
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    std::cout << "pi = " << std::setprecision(20) << pi << "\t" << "Time:" << std::fixed << time_spent << std::cout.precision(5) << " sec " << std::endl;
    return 0;
}
