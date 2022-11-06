#pragma once

#include <unordered_map>
#include <list>
#include <stdexcept>

template <typename K, typename V, size_t CAPACITY = 10>
class lru_cache {
private:
    using list_element_t = std::pair<K, V>;
    using list_t = std::list<list_element_t>;
    using list_iterator_t = typename list_t::iterator;
    using map_t = std::unordered_map<K, list_iterator_t>;
    using return_val_t = const V*;

    list_t queue{};
    map_t map{};

    void prepare_insert() {
        assert(queue.size() <= CAPACITY);
        if (queue.size() == CAPACITY) {
            auto &[k, v] = queue.back();
            map.erase(k);
            queue.pop_back();
        }
        assert(queue.size() <= CAPACITY);
    }

    void move_to_front(list_iterator_t element){
        if (element != queue.begin()) {
            queue.splice(queue.begin(), queue, element, std::next(element));
        }
    }

public:
    void put(const K& key, const V& value) noexcept {
        auto it = map.find(key);
        if (it == map.end()) {
            prepare_insert();
            queue.push_front({key, value});
            map[key] = queue.begin();
        } else {
            move_to_front(it->second);
            queue.front().second = value;
        }
        assert(map.find(key) != map.end());
    }

    [[nodiscard]] return_val_t get(const K& key) {
        auto it = map.find(key);
        if (it == map.end()) {
            return nullptr;
        }
        
        move_to_front(it->second);
        return &queue.front().second;
    }

    void erase(const K& key) noexcept {
        auto it = map.find(key);
        if (it == map.end()) {
            return;
        }

        queue.erase(it->second);
        map.erase(it);
        assert(map.find(key) == map.end());
    }

    [[nodiscard]] size_t size() const noexcept {
        return queue.size();
    }

    [[nodiscard]] bool is_empty() const noexcept {
        return queue.empty();
    }
};
