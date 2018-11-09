#pragma once

#include <algorithm>
#include <thread>
#include <vector>



namespace absolutegentleman
{

template <class F>
void repeat_parallel(F f, size_t n)
{
    if (n == 0)
    {
        return;
    }

    const auto num_threads = std::max(std::thread::hardware_concurrency(), 1U);
    const auto step = std::max(1LU, n / num_threads);

    std::vector<std::thread> threads;

    size_t i{};

    for (; i < n - step; i += step)
    {
        threads.emplace_back([=, &f]()
        {
            for (size_t k = i; k < i + step; ++k)
            {
                f(k);
            }
        });
    }

    threads.emplace_back([=, &f]()
    {
        for (size_t k = i; k < n; ++k)
        {
            f(k);
        }
    });

    for (auto&& thread : threads)
    {
        thread.join();
    }
}

}
