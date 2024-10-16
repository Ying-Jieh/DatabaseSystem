//
// Created by Daniel Ziper on 4/18/24.
//

#ifndef ECE141DB_ORDEREDMAP_HPP
#define ECE141DB_ORDEREDMAP_HPP

#include <vector>
#include <unordered_map>
#include <memory>
#include <optional>
#include <functional>
#include <utility>

namespace ECE141 {
    template <typename K, typename V>
    struct OrderedMap {
    protected:
        std::vector<K> order;

    public:
        std::unordered_map<K, V> lookup;

        OrderedMap()=default;
        ~OrderedMap()=default;

        using Callback = std::function<bool(size_t, const K&, const V&)>;

        bool operator==(const OrderedMap& aOrderedMap) const {
            return order == aOrderedMap.order && lookup == aOrderedMap.lookup;
        }

        virtual void clear() {
            order.clear();
            lookup.clear();
        }
        bool empty() {
            return order.empty();
        }
        size_t size() {
            return order.size();
        }
        bool contains(K key) const {
            return lookup.find(key) != lookup.end();
        }
        virtual bool insert(K key, V val) {
            if (contains(key)) return false;
            lookup.insert({key, val});
            order.push_back(key); // Not very efficient...
            return true;
        }

        bool set(K key, V val) {
            if (contains(key)) return false;
            lookup[key] = val;
            return true;
        }

        std::optional<V> get(const K& key) const {
            auto iter = lookup.find(key);
            if (iter == lookup.end()) return std::nullopt;
            return iter->second;
        }
        typename std::unordered_map<K,V>::iterator find(const K& key) {
            return lookup.find(key);
        }

        bool orderedEach(Callback aCallback) const {
            size_t index = 0;
            for (auto& o : order) {
                auto pair = lookup.find(o);
                if (!aCallback(index, pair->first, pair->second)) {
                    return false;
                }
                index++;
            }
            return true;
        }

        class OrderedMapIterator {
        protected:
            typename std::vector<K>::iterator vecIter;
            std::unordered_map<K, V>* lookupPtr ;

        public:
            OrderedMapIterator(typename std::vector<K>::iterator aIter, std::unordered_map<K, V>* aLookupPtr) {
                vecIter = aIter;
                lookupPtr = aLookupPtr;
            }

            OrderedMapIterator& operator++() {
                ++vecIter;
                return *this;
            }

            std::pair<K, V> operator*() const {
                return *(lookupPtr->find(*vecIter));
            }

            bool operator==(const OrderedMapIterator& other) const {
                return vecIter == other.vecIter;
            }

            bool operator!=(const OrderedMapIterator& other) const {
                return !(*this == other);
            }

        };

        OrderedMapIterator begin() {
            return OrderedMapIterator(order.begin(), &lookup);
        }

        // Function to get iterator to the end of the stack
        OrderedMapIterator end() {
            return OrderedMapIterator(order.end(), &lookup);
        }
    };

    template <typename K, typename V>
    struct IndexedMap : OrderedMap<K,V> {
        std::unordered_map<K, size_t> indexLookup;

        bool insert(K key, V value) override {
            size_t newIndex = OrderedMap<K,V>::order.size();
            if (OrderedMap<K,V>::insert(key, value)) {
                indexLookup.insert({key, newIndex});
                return true;
            }
            return false;
        }

        std::optional<size_t> getIndex(K key) const {
            auto iter = indexLookup.find(key);
            if (iter != indexLookup.end()) {
                return iter->second;
            }
            return std::nullopt;
        }

        void clear() override {
            OrderedMap<K,V>::clear();
            indexLookup.clear();
        }

    };
}
#endif //ECE141DB_ORDEREDMAP_HPP
