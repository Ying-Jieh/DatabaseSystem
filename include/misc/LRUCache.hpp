//
//  LRUCache.hpp
//
//  Created by rick gessner on 5/27/23.
//

#ifndef LRUCache_h
#define LRUCache_h

#include <unordered_map>
#include <list>
#include <cstddef>
#include <stdexcept>

namespace ECE141 {

    template<typename KeyT, typename ValueT>
    class LRUCache {
    public:
        using keyValuePair = std::pair<KeyT, ValueT>;
        using listIterator = typename std::list<keyValuePair>::iterator;

        explicit LRUCache(size_t aCapacity=200) : theMaxCapacity(aCapacity) {}

        [[nodiscard]] size_t size() const { return theCacheMap.size(); }

        bool contains(const KeyT& key) const { return theCacheMap.find(key) != theCacheMap.end(); }


        //only call this if you know the key is new
//        void add(const KeyT& key, const ValueT& value);
        void put(const KeyT& key, const ValueT& value) {
            auto it = theCacheMap.find(key);
            theCacheList.push_front(keyValuePair(key, value));
            if (it != theCacheMap.end()) {
                theCacheList.erase(it->second);
                theCacheMap.erase(key);
            }
            theCacheMap[key] = theCacheList.begin();
            evictIf();
        }

        const ValueT& get(const KeyT& key) {
            auto it = theCacheMap.find(key);
            if (it == theCacheMap.end()) { throw std::range_error("There is no such key in cache"); }
            else {
                theCacheList.splice(theCacheList.begin(), theCacheList, it->second);
                return it->second->second;
            }
        }

        const keyValuePair peek() {
            if (theCacheList.empty()) { throw std::range_error("There is no such key in cache"); }
            return theCacheList.front();
        }

        void clear() {
            theCacheMap.clear();
            theCacheList.clear();
        }

        bool erase(const KeyT &aKey) {
            auto it = theCacheMap.find(aKey);
            if (it == theCacheMap.end()) { return false; }
            else {
                theCacheList.erase(it);
                theCacheMap.erase(aKey);
                return true;
            }
        }

//        bool remove(const KeyT& aKey);

    protected:
        size_t theMaxCapacity;
        std::list<keyValuePair> theCacheList;
        std::unordered_map<KeyT, listIterator> theCacheMap;

        void evictIf() {
            if (theCacheMap.size() > theMaxCapacity) {
                auto theLast = theCacheList.end();
                theLast--;
                theCacheMap.erase(theLast->first);
                theCacheList.pop_back();
            }
        }
    };
}

#endif /* LRUCache_h */
