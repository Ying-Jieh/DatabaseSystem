//
//  FolderReader.hpp
//  Database5
//
//  Created by rick gessner on 4/4/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef FolderReader_h
#define FolderReader_h

#include <string>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace ECE141 {

    using FileVisitor = std::function<bool(const std::string &)>;

    class FolderReader {
    public:
        FolderReader(const std::string &aPath) : path(aPath) {}

        virtual ~FolderReader() {}

        virtual bool exists(const std::string &aFilename) {
            std::ifstream theStream(aFilename);
            return !theStream ? false : true;
        }

        virtual void each(const std::string &anExt,
                          const FileVisitor &aVisitor) const {
            for (const auto& entry : fs::directory_iterator(path)) {
                if (entry.is_regular_file() && entry.path().extension() == anExt) {
                    if (!aVisitor(entry.path().string())) {
                        break;
                    }
                }
            }
        };

        std::string path;
    };

}

#endif /* FolderReader_h */
