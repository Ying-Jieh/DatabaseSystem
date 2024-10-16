//
//  Config.hpp
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//


#ifndef Config_h
#define Config_h
#include <sstream>
#include "Timer.hpp"
#include "Logger.hpp"
#include <filesystem>

namespace ECE141 {

  enum class CacheType : int {block=0, rows, views};

  struct Config {

    static size_t cacheSize[3];
    static bool   indexing;
    static constexpr size_t joinedRowSize = 2;

    static const char* getDBExtension() {return ".db";}

    static const std::string getConnectionString() {
      return "localhost";
    }

    static const LogLevel getLogLevel() {
        return LogLevel::Error;
    }

    static const std::string getStoragePath() {
        
      #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        
        //STUDENT: If you're on windows, return a path to folder on your machine...
        return std::filesystem::temp_directory_path().string();
      
      #elif __APPLE__ || defined __linux__ || defined __unix__

        return std::string("/tmp");
              
      #endif
    }
    
    static std::string getDBPath(const std::string &aDBName) {
      #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        const std::string theSep="\\";
      #elif __APPLE__ || defined __linux__ || defined __unix__
        const std::string theSep="/";
      #endif

      std::ostringstream theStream;
      theStream << Config::getStoragePath() << theSep << aDBName << ".db";
      return theStream.str();
    }
      
    static Timer& getTimer() {
      static Timer theTimer;
      return theTimer;
    }
    
    static std::string getAppName() {return "DB::141";}
    static std::string getVersion() {return "10";}
    static std::string getMembers() {return "Authors: Chris and Daniel";}


    //cachetype: block, row, view...
    static size_t getCacheSize(CacheType aType) {
      return cacheSize[(int)aType];
    }

    static void setCacheSize(CacheType aType, size_t aSize) {
      cacheSize[(int)aType]=aSize;
    }

    //cachetype: block, row, view...
    static bool useCache(CacheType aType) {
      return cacheSize[(int)aType]>0;
    }

    static bool useIndex() {return indexing;}
  };

}

#endif /* Config_h */
