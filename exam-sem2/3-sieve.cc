/*
 * Task:
 *
 * Write a program that computes primes up to N using segmentation:
 * first, single-threaded, find the base primes up to sqrt(N), then
 * split the range [sqrt(N), N] into segments and process them
 * independently across several worker threads, making the most of
 * the available hardware parallelism.
 *
 * The result must be the output of all primes up to N and must match
 * a simple single-threaded implementation.
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <span>
#include <ranges>
#include <algorithm>
#include <concepts>
#include <atomic>
#include <gtest/gtest.h>

template <std::integral T>
std::vector<T> simple_sieve(T N) {
    if (N < 2) return {};
    std::vector<bool> is_prime(N + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (T i = 2; i * i <= N; ++i) {
        if (is_prime[i]) {
            for (T j = i * i; j <= N; j += i) {
                is_prime[j] = false;
            }
        }
    }
    std::vector<T> primes;
    for (T i = 2; i <= N; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }
    return primes;
}

template <std::unsigned_integral T>
std::vector<T> segmented_sieve(T N) {
    if (N < 2) return {};

    T L = static_cast<T>(std::sqrt(N));
    std::vector<T> base_primes = simple_sieve(L);

    if (N <= L) {
        return base_primes;
    }

    T start_val = L + 1;
    T end_val = N;
    T S = 262144;

    auto chunks = std::views::iota(start_val, end_val + 1) | std::views::chunk(S);
    std::vector<std::vector<T>> chunk_results(std::ranges::distance(chunks));

    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 1;

    std::atomic<size_t> current_chunk_idx{0};
    std::span<const T> base_primes_span(base_primes);

    auto worker = [&]() {
        std::vector<bool> sieve_buf;
        auto it = chunks.begin();
        while (true) {
            size_t idx = current_chunk_idx.fetch_add(1, std::memory_order_relaxed);
            if (idx >= chunk_results.size()) break;

            auto chunk = *(it + idx);
            T low = *chunk.begin();
            T range_size = static_cast<T>(std::ranges::size(chunk));
            T high = low + range_size - 1;

            sieve_buf.assign(range_size, true);

            for (T p : base_primes_span) {
                if (p * p > high) break;
                T start = ((low - 1) / p + 1) * p;
                if (start < p * p) {
                    start = p * p;
                }
                for (T j = start; j <= high; j += p) {
                    sieve_buf[j - low] = false;
                }
            }

            auto& local_primes = chunk_results[idx];
            for (T j = 0; j < range_size; ++j) {
                if (sieve_buf[j]) {
                    local_primes.push_back(low + j);
                }
            }
        }
    };

    std::vector<std::jthread> threads;
    threads.reserve(num_threads);
    for (unsigned int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker);
    }
    threads.clear();

    size_t total_primes = base_primes.size();
    for (const auto& res : chunk_results) {
        total_primes += res.size();
    }

    std::vector<T> all_primes;
    all_primes.reserve(total_primes);
    all_primes.insert(all_primes.end(), base_primes.begin(), base_primes.end());
    std::ranges::copy(chunk_results | std::views::join, std::back_inserter(all_primes));

    return all_primes;
}

TEST(SieveTest, SegmentedMatchesSimple) {
    uint64_t N = 1000000;
    auto par_primes = segmented_sieve(N);
    auto seq_primes = simple_sieve(N);
    EXPECT_EQ(par_primes, seq_primes);
    EXPECT_EQ(par_primes.size(), 78498);
}

TEST(SieveTest, EdgeCases) {
    EXPECT_TRUE(segmented_sieve(0ULL).empty());
    EXPECT_TRUE(segmented_sieve(1ULL).empty());
    EXPECT_EQ(segmented_sieve(2ULL).size(), 1);
}
