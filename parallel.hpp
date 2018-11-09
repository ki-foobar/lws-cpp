#pragma once

#include <algorithm>
#include <thread>
#include <vector>



namespace gentleman
{
namespace parallel
{

template <class F>
void repeat_internal(F f, size_t n)
{
    if (n == 0)
    {
        return;
    }

    const size_t num_threads = std::max(std::thread::hardware_concurrency(), 1U);
    const size_t step = std::max(1LU, n / num_threads);

    std::vector<std::thread> threads;

    size_t i{};

    for (; i < n - step; i += step)
    {
        threads.emplace_back([=, &f]()
        {
            f(i, i + step);
        });
    }

    threads.emplace_back([=, &f]()
    {
        f(i, n);
    });

    for (auto&& thread : threads)
    {
        thread.join();
    }
}



template <class F>
void repeat(F f, size_t n)
{
    repeat_internal([&f](size_t begin, size_t end)
    {
        for (size_t i = begin; i < end; ++i)
        {
            f(i);
        }
    }, n);
}

}
}
