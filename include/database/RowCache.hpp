//
// Created by Daniel Ziper on 5/3/24.
//

#ifndef ECE141DB_ROWCACHE_HPP
#define ECE141DB_ROWCACHE_HPP

#include <set>
#include <vector>
#include <utility>
#include <optional>
#include "TableRow.hpp"

namespace ECE141 {
    struct CacheStatus {
        bool required = false;
        bool dirty = false;
    };

    using CacheCallback = std::function<bool(const IdentifierHash&, const TableRow&, const CacheStatus&)>;

    class RowCache {
    public:
        RowCache()=default;
        ~RowCache()=default;
        RowCache(const RowCache&)=default;
        bool operator==(const RowCache&)=delete;


        bool eachRequired(const CacheCallback& aCallback);
        bool eachDirty(const CacheCallback& aCallback);
        bool each(const CacheCallback& aCallback);
        bool markDirty(const IdentifierHash& id, bool newDirty=true);

        bool contains(const IdentifierHash& id);
        bool isRequired(const IdentifierHash& id);
        bool isDirty(const IdentifierHash& id);
        bool markRequired(const IdentifierHash& id, bool newRequired=true);
        void resetDirty();
        void resetRequired();

        const TableRow& operator [](const IdentifierHash& id) {
            return getRow(id);
        }

        void clear();

        TableRow& getRow(const IdentifierHash& id);

        bool addRow(const IdentifierHash& id, const TableRow& aRow, bool aRequired=false, bool aDirty=false);
        bool remove(const IdentifierHash& id);
        bool updateRow(const IdentifierHash& id, const TableRow& aRow, bool aRequired=false, bool aDirty=false);
        bool empty() {return rows.empty();}
        bool hasDirty() {return !dirty.empty();}
        bool hasRequired() {return !required.empty();}
        size_t requiredSize() const {return required.size();}
        size_t size() const {return rows.size();}


        class CacheIterator {
        protected:
            typename std::set<IdentifierHash>::iterator mapIter;
            RowCache* cache;

        public:
            CacheIterator(typename std::set<IdentifierHash>::iterator aIter, RowCache* aCache) {
                mapIter = aIter;
                cache = aCache;
            }
            CacheIterator(const CacheIterator& aCopy) {
                mapIter = aCopy.mapIter;
                cache = aCopy.cache;
            }

            CacheIterator& operator++() {
                ++mapIter;
                return *this;
            }

            const TableRow& operator*() const {
                return cache->getRow(*mapIter);
            }

            bool operator==(const CacheIterator& other) const {
                return mapIter == other.mapIter;
            }

            bool operator!=(const CacheIterator& other) const {
                return !(*this == other);
            }
        };

        CacheIterator begin() {
            return CacheIterator(required.begin(), this);
        }

        CacheIterator end() {
            return CacheIterator(required.end(), this);
        }

    protected:
        using StatusCreatorCallback = std::function<CacheStatus(IdentifierHash)>;
        bool eachPrimitive(const CacheCallback& aCallback, std::set<IdentifierHash>& aSet, const StatusCreatorCallback& statusCreator);

//        TODO: CachePolicy for removing nonrequired rows from Cache?
        std::set<IdentifierHash> dirty; // Row was updated/inserted but NOT written to Storage yet
        std::set<IdentifierHash> required;
        std::map<IdentifierHash, TableRow> rows;
    };
}

#endif //ECE141DB_ROWCACHE_HPP
