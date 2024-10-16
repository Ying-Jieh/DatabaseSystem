//
// Created by Daniel Ziper on 5/3/24.
//

#include "database/RowCache.hpp"

namespace ECE141 {

    bool RowCache::eachRequired(const CacheCallback& aCallback) {
        return eachPrimitive(aCallback, required, [&](IdentifierHash id){
            return CacheStatus{true, dirty.find(id) == dirty.end()};
        });
    }

    bool RowCache::eachDirty(const CacheCallback& aCallback) {
        return eachPrimitive(aCallback, dirty, [&](IdentifierHash id){
            return CacheStatus{required.find(id) == required.end(), true};
        });
    }

    bool RowCache::eachPrimitive(const ECE141::CacheCallback &aCallback, std::set<IdentifierHash> &aSet,
                                 const ECE141::RowCache::StatusCreatorCallback &statusCreator) {
        for (const auto& id : aSet) {
            CacheStatus status = statusCreator(id);
            const auto& pair = *rows.find(id);
            if (!aCallback(pair.first, pair.second, status)) {
                return false;
            }
        }
        return true;
    }

    bool RowCache::each(const CacheCallback& aCallback) {
        for (const auto& pair : rows) {
            CacheStatus status {
                    required.find(pair.first) == required.end(),
                    dirty.find(pair.first) == dirty.end()
            };
            if (!aCallback(pair.first, pair.second, status)) {
                return false;
            }
        }
        return true;
    }

    bool RowCache::contains(const IdentifierHash& id) {
        return rows.count(id);
    }

    bool RowCache::isRequired(const IdentifierHash& id) {
        return required.find(id) != required.end();
    }
    bool RowCache::isDirty(const IdentifierHash& id) {
        return dirty.find(id) != dirty.end();
    }

    bool RowCache::markDirty(const IdentifierHash& id, bool newDirty) {
        auto iter = rows.find(id);
        if (iter == rows.end()) return false; // Don't have item
        dirty.insert(id);
        if (!newDirty) dirty.erase(id);
        else dirty.insert(id);
        return true;
    }

    bool RowCache::markRequired(const IdentifierHash& id, bool newRequired) {
        auto iter = rows.find(id);
        if (iter == rows.end()) return false; // Don't have item
        if (!newRequired) required.erase(id);
        else required.insert(id);
        return true;
    }

    void RowCache::resetDirty() {
        dirty.clear();
    }
    void RowCache::resetRequired() {
        required.clear();
    }

    void RowCache::clear() {
        required.clear();
        dirty.clear();
        rows.clear();
    }

    TableRow& RowCache::getRow(const IdentifierHash& id) {
        auto iter = rows.find(id);
        return iter->second;
    }

    bool RowCache::addRow(const IdentifierHash& id, const TableRow& aRow, bool aRequired, bool aDirty) {
        // Does NOT update row if exists, but will mark as required/dirty (and return false)
        bool contains = rows.find(id) != rows.end();
        if (!contains) {
            rows.insert({id, aRow});
        }

        if (aRequired) required.insert(id);
        if (aDirty) dirty.insert(id);

        return !contains;
    }

    bool RowCache::remove(const IdentifierHash &id) {
        if (!rows.count(id)) return false;
        rows.erase(id);
        required.erase(id);
        dirty.erase(id);
        return true;
    }

    bool RowCache::updateRow(const IdentifierHash& id, const TableRow& aRow, bool aRequired, bool aDirty) {// True if overwrote. False if row did not exist. Still insert when did not exist
        DEBUG("RowCache::updateRow, id=", id)
        bool replaced = rows.insert_or_assign(id, aRow).second;
        if (aRequired) required.insert(id);
        if (aDirty) dirty.insert(id);
        return replaced;
    }
}
