//
// Created by Daniel Ziper on 4/22/24.
//

#ifndef ECE141DB_INDEX_HPP
#define ECE141DB_INDEX_HPP

#include <map>
#include <optional>
#include "storage/Storage.hpp"
#include "misc/Helpers.hpp"

namespace ECE141 {
    template<typename ID=IdentifierHash>
    class Index : public Storable, ByteIO<IdentifierHash>, ByteIO<uint32_t> {
    public:
        Index(const std::string &aName="") : name(aName) {
            loaded=false;
        }
        ~Index()=default;

        using IndexCallback = std::function<bool(ID, uint32_t)>;

        size_t size() const {return items.size();};
        void clear() {
            dirty = true;
            items.clear();
        }
        bool contains(ID anID) const {
            return items.find(anID) != items.end();
        }
        virtual bool remove(ID anID) {
            dirty = true;
            return items.erase(anID);
        }
        virtual std::optional<uint32_t> getBlock(ID anID) {
            auto iter = items.find(anID);
            if (iter == items.end()) return std::nullopt;
            return iter->second;
        }
        bool isLoaded() {return loaded;}
        virtual bool add(ID anID, uint32_t aBlockNum) {
            dirty = true;
            if (items.find(anID) == items.end()) {
                items.insert({anID, aBlockNum});
                DEBUG("Added (", anID, ", ", aBlockNum, ") to items")
                return true;
            }
            return false;
        };

        bool each(IndexCallback aCallback) const {
            for (auto& item : items) {
                TRACE("item: ", item.first, ", ", item.second)
                if (!aCallback(item.first, item.second)) {
                    return false;
                }
            }
            return true;
        }

        // Storable
        inline StatusResult encode(std::ostream &anOutput) const override {
            INFO("Start to encode an Index")
            uint32_t size = items.size();
            ByteIO<uint32_t>::writeBytes(anOutput, size);
            for (auto& item : items) {
                DEBUG("ID: ", item.first, " BlockNum: ", item.second)
                ByteIO<ID>::writeBytes(anOutput, item.first);
                ByteIO<uint32_t>::writeBytes(anOutput, item.second);
            }
            INFO("Successfully encoded an Index")
            return {Errors::noError};
        };
        inline StatusResult decode(std::istream &anInput) override {
            INFO("Start to decode an Index")
            uint32_t size;
            if (!ByteIO<uint32_t>::readBytes(anInput, size)) {
                ERROR("readError when decoding an Index")
                return {Errors::readError};
            };
            for (uint32_t index = 0; index < size; index++) {
                std::pair<ID, uint32_t> pair;
                if (!ByteIO<ID>::readBytes(anInput, pair.first) ||
                    !ByteIO<uint32_t>::readBytes(anInput, pair.second)) {
                    ERROR("readError when decoding an Index")
                    return {Errors::readError};
                };
                items.insert(pair);
                DEBUG("items.insert(", pair.first, ", ", pair.second, ")")
            }
            loaded = true;
            INFO("Successfully decode an Index")
            return {Errors::noError};
        };
        bool initHeader(BlockHeader &header) const override {
            header.hash = Helpers::hash(name);
            header.type=BlockType::index;
            INFO("Initialized a Index header")
            return true;
        };

        ID getNextID() {
            if (items.empty()) return 1; // ?
            return items.rbegin()->first + 1;
        }

        bool operator==(const Index& other) const {
            return other.items == items;
        }

        bool dirty = false;

    protected:
        std::map<
            ID,    // Could be Table name (for Schema index) or ID/hash of key (for TableRow index)
            uint32_t   // Block num
        > items;
        std::string name;
        bool loaded;
    };

    template <>
    inline StatusResult Index<std::string>::encode(std::ostream &anOutput) const {
        INFO("Start to encode an Index<string>")
        uint32_t size = items.size();
        ByteIO<uint32_t>::writeBytes(anOutput, size);

        char null = 0;
        for (auto& item : items) {
            TRACE("Encoded ", item.first)
            anOutput << item.first;
            anOutput.write(&null, 1);
            ByteIO<uint32_t>::writeBytes(anOutput, item.second);
        }
        INFO("Successfully encoded an Index<string>")
        return {Errors::noError};
    };

    template <>
    inline StatusResult Index<std::string>::decode(std::istream &anInput) {
        INFO("Start to decode an Index<string>")
        uint32_t size;
        if (!ByteIO<uint32_t>::readBytes(anInput, size)) {
            ERROR("readError when decoding an Index<string>")
            return {Errors::readError};
        };
        char null = 0;
        for (uint32_t index = 0; index < size; index++) {
            std::pair<std::string, uint32_t> pair;
//            anInput >> pair.first;
            std::getline(anInput, pair.first, null);
            if (!ByteIO<uint32_t>::readBytes(anInput, pair.second)) {
                ERROR("readError when decoding an Index<string>")
                return {Errors::readError};
            }
            add(pair.first, pair.second);
        }
        loaded = true;
        INFO("Successfully decoded an Index<string>")
        return {Errors::noError};
    };

    class StringIndex : public Index<std::string> {
        // Store a hashLookup (reverse lookup) so that we can quickly check if a given hash exists in the Index
        // Useful for looking up which Table a Block belongs to
    public:
        StringIndex(const std::string& aName="") : Index<std::string>(aName) {};

        bool add(std::string key, uint32_t aBlockNum) override {
            if (Index::add(key, aBlockNum)) {
                hashLookup.insert({
                    Helpers::hash(key), key
                });
                DEBUG("hashLookup.insert(", Helpers::hash(key), ", ", key, ")")
                return true;
            }
            return false;
        }

        bool remove(std::string key) override {
            if (Index::remove(key)) {
                hashLookup.erase(Helpers::hash(key));
                DEBUG("hashLookup.erase(", Helpers::hash(key), ", ", key, ")")
                return true;
            }
            return false;
        }

        std::optional<std::string> lookupHash(IdentifierHash hash) {
            auto iter = hashLookup.find(hash);
            if (iter == hashLookup.end()) {return std::nullopt;}
            return iter->second;
        }

    protected:
        std::map<IdentifierHash, std::string> hashLookup;
    };

}

#endif //ECE141DB_INDEX_HPP
