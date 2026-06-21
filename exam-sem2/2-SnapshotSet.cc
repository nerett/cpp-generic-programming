/*
 * Task:
 *
 * Design and partially implement the SnapshotSet<T> data structure.
 *
 * Multiple threads concurrently insert elements.
 * A single thread periodically takes a snapshot and performs
 * lower_bound, find, or iteration against it.
 *
 * Elements inserted while a snapshot is being built may be excluded
 * from that snapshot, but must never be lost.
 */

#include <atomic>
#include <vector>
#include <memory>
#include <algorithm>
#include <concepts>
#include <iterator>
#include <thread>
#include <type_traits>
#include <gtest/gtest.h>

template <std::totally_ordered T> requires std::copy_constructible<T>
class SnapshotSet {
    static_assert(std::is_nothrow_move_constructible_v<T>);

    struct Node {
        T value;
        Node* next{};
    };

    std::atomic<Node*> head_{nullptr};
    std::shared_ptr<const std::vector<T>> current_data_{std::make_shared<std::vector<T>>()};

public:
    SnapshotSet() = default;

    SnapshotSet(const SnapshotSet&) = delete;
    SnapshotSet& operator=(const SnapshotSet&) = delete;
    SnapshotSet(SnapshotSet&&) = delete;
    SnapshotSet& operator=(SnapshotSet&&) = delete;

    ~SnapshotSet() {
        Node* curr = head_.load(std::memory_order_relaxed);
        while (curr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
    }

    void insert(T value) {
        Node* new_node = new Node{std::move(value), nullptr};
        new_node->next = head_.load(std::memory_order_relaxed);
        while (!head_.compare_exchange_weak(new_node->next, new_node,
                                            std::memory_order_release,
                                            std::memory_order_relaxed)) {
        }
    }

    class Snapshot {
        std::shared_ptr<const std::vector<T>> data_;

    public:
        explicit Snapshot(std::shared_ptr<const std::vector<T>> data) noexcept
            : data_(std::move(data)) {}

        Snapshot(const Snapshot&) noexcept = default;
        Snapshot(Snapshot&&) noexcept = default;
        Snapshot& operator=(const Snapshot&) noexcept = default;
        Snapshot& operator=(Snapshot&&) noexcept = default;

        auto begin() const noexcept { return data_->begin(); }
        auto end() const noexcept { return data_->end(); }

        auto lower_bound(const T& val) const noexcept {
            return std::ranges::lower_bound(*data_, val);
        }

        auto find(const T& val) const noexcept {
            auto it = lower_bound(val);
            if (it != data_->end() && *it == val) {
                return it;
            }
            return data_->end();
        }
    };

    Snapshot make_snapshot() {
        Node* curr = head_.exchange(nullptr, std::memory_order_acquire);
        if (!curr) {
            return Snapshot{current_data_};
        }

        std::vector<T> new_elements;

        struct NodeDeleter {
            Node* head;
            ~NodeDeleter() {
                while (head) {
                    Node* next = head->next;
                    delete head;
                    head = next;
                }
            }
        } deleter{curr};

        while (deleter.head) {
            Node* current_node = deleter.head;
            new_elements.push_back(std::move(current_node->value));
            deleter.head = current_node->next;
            delete current_node;
        }

        std::ranges::sort(new_elements);
        auto ret = std::ranges::unique(new_elements);
        new_elements.erase(ret.begin(), ret.end());

        auto merged = std::make_shared<std::vector<T>>();
        merged->reserve(current_data_->size() + new_elements.size());

        std::ranges::set_union(*current_data_, new_elements, std::back_inserter(*merged));

        current_data_ = std::move(merged);
        return Snapshot{current_data_};
    }
};

TEST(SnapshotSetTest, SingleThreadOperations) {
    SnapshotSet<int> set;

    set.insert(5);
    set.insert(3);
    set.insert(8);
    set.insert(3);

    auto snap = set.make_snapshot();

    EXPECT_EQ(*snap.lower_bound(4), 5);
    EXPECT_NE(snap.find(3), snap.end());
    EXPECT_EQ(snap.find(4), snap.end());

    set.insert(4);
    auto snap2 = set.make_snapshot();

    EXPECT_NE(snap2.find(4), snap2.end());
    EXPECT_EQ(snap.find(4), snap.end());
}

TEST(SnapshotSetTest, ConcurrentInsertsAndSnapshots) {
    SnapshotSet<int> set;
    std::vector<std::thread> writers;
    std::atomic<bool> done{false};

    const int num_writers = 4;
    const int inserts_per_writer = 1000;

    for (int i = 0; i < num_writers; ++i) {
        writers.emplace_back([&set, i, inserts_per_writer]() {
            for (int j = 0; j < inserts_per_writer; ++j) {
                set.insert(i * inserts_per_writer + j);
            }
        });
    }

    std::thread reader([&set, &done]() {
        while (!done.load(std::memory_order_relaxed)) {
            auto snap = set.make_snapshot();

            auto it = snap.find(500);
            (void)it;

            auto lb = snap.lower_bound(1500);
            (void)lb;

            int prev = -1;
            for (int val : snap) {
                if (prev != -1) {
                    EXPECT_LT(prev, val);
                }
                prev = val;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });

    for (auto& t : writers) {
        t.join();
    }

    done.store(true, std::memory_order_relaxed);
    reader.join();

    auto final_snap = set.make_snapshot();
    int count = 0;
    for (int val : final_snap) {
        (void)val;
        count++;
    }

    EXPECT_EQ(count, num_writers * inserts_per_writer);
}
