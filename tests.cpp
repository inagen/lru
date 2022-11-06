#include <gtest/gtest.h>
#include <random>
#include "lru_cache.h"


TEST(LRUCache, IsEmpty) {
    lru_cache<int, int, 10> cache;
    EXPECT_TRUE(cache.is_empty());
}

TEST(LRUCache, BasicInsert) {
    lru_cache<int, int, 10> cache;
    cache.put(1, 1);
    cache.put(2, 2);
    EXPECT_EQ(*cache.get(1), 1);
    EXPECT_EQ(*cache.get(2), 2);
    EXPECT_EQ(cache.size(), 2);
}

TEST(LRUCache, InsertSameKey) {
    lru_cache<int, int, 10> cache;
    cache.put(1, 1);
    cache.put(1, 2);
    EXPECT_EQ(*cache.get(1), 2); 
    EXPECT_EQ(cache.size(), 1);
}

TEST(LRUCache, Eviction) {
    lru_cache<int, int, 10> cache;

    for (int i = 0; i < 40; ++i) {
        cache.put(i, i);
    }
    
    EXPECT_EQ(cache.size(), 10);

    for (int i = 0; i < 30; ++i) {
        EXPECT_EQ(cache.get(i), nullptr); 
    }

    for (int i = 30; i < 40; ++i) {
        EXPECT_EQ(*cache.get(i), i);
    }
}

TEST(LRUCache, Deletion) {
    lru_cache<int, int, 100> cache;

    for (int i = 0; i < 100; ++i) {
        cache.put(i, i);
    }

    EXPECT_EQ(cache.size(), 100);

    for (int i = 0; i < 100; i += 2) {
        cache.erase(i);
    }

    EXPECT_EQ(cache.size(), 50);

    for (int i = 0; i < 100; i += 2) {
        EXPECT_EQ(cache.get(i), nullptr); 
    }

    for (int i = 1; i < 100; i += 2) {
        EXPECT_EQ(*cache.get(i), i);
    }
}

TEST(LRUCache, RandomPut) {
    lru_cache<int, int, 10> cache;

    for (int i = 0; i < 10; ++i) {
        cache.put(i, i);
    }
    
    EXPECT_EQ(cache.size(), 10);

    std::default_random_engine e1(std::random_device{}());
    std::uniform_int_distribution<int> uniform_dist(0, 100);

    for (int i = 0; i < 1000; ++i) {
        cache.put(uniform_dist(e1), uniform_dist(e1));
        EXPECT_EQ(cache.size(), 10);
    }
}
