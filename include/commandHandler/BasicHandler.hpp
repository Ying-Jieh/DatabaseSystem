//
// Created by Daniel Ziper on 4/3/24.
//

#ifndef ECE141DB_BASICHANDLER_HPP
#define ECE141DB_BASICHANDLER_HPP

#include <functional>
#include <map>
#include <fstream>
#include "CommandHandler.hpp"
#include "controller/AppController.hpp"
#include "misc/Config.hpp"
#include "misc/Logger.hpp"

namespace ECE141 {
    class BasicHandler : public CommandHandler {
    public:
        explicit BasicHandler(ExecutionContext& aContext) : CommandHandler(aContext) {}
        ~BasicHandler() override = default;
        StatusResult handle(Tokenizer& aTokenizer) override {
            TRACE("BasicHandler is handling")
            if (aTokenizer.size() < 1) {
                ERROR("Tokenizer.size < 1 in BasicHandler")
                return Errors::unknownCommand;
            }
            Token& first = aTokenizer.tokenAt(0);

            auto found = handlerMap.find(first.keyword);
            if (found != handlerMap.end()) {
                INFO("Handling ", first.data)
                found->second();
                return Errors::noError;
            }

            TRACE("BasicHandler cannot handle")
            return Errors::unknownCommand;
        }

    protected:
        using Callback = std::function<StatusResult()>;
        bool helpFunc() {
            TRACE("Try to open help.txt")
            std::fstream inputFile("docs/help.txt");
            std::stringstream ss;
            if (!inputFile) {
                ERROR("Failed to open help.txt")
                ss << "Failed to open help.txt";
                StringView sv(ss.str());
                context.viewListener(sv);
                return false;
            }

            TRACE("Opened help.txt")
            std::string line;
            while(getline(inputFile, line)) {
                ss << line << "\n";
            }
            TRACE("Read help.txt")
            StringView sv(ss.str());
            context.viewListener(sv);
            inputFile.close();
            return true;
        }
        std::map<Keywords, Callback> handlerMap = {
                {Keywords::about_kw,[this](){
                        std::stringstream ss;
                        ss << "Authors: " << Config::getMembers() << std::endl;
                        StringView sv(ss.str());
                        context.viewListener(sv);
                        INFO("Successfully handled about")
                        return StatusResult{Errors::noError};
                    }
                },
                {Keywords::version_kw,[this](){
                        std::stringstream ss;
                        ss << "Version: " << Config::getVersion() << std::endl;
                        StringView sv(ss.str());
                        context.viewListener(sv);
                        INFO("Successfully handled version")
                        return StatusResult{Errors::noError};
                    }
                },
                {Keywords::help_kw,[this](){
                        helpFunc();
                        INFO("Successfully handled help")
                        return StatusResult{Errors::noError};
                    }
                },
                {Keywords::quit_kw,[this](){
                    std::stringstream ss;
                    ss << "DB::141 is shutting down" << std::endl;
                    StringView sv(ss.str());
                    context.viewListener(sv);
                    INFO("Successfully handled quit")
                    return StatusResult{Errors::userTerminated};
                    }
                }
        };
    };
}



#endif //ECE141DB_BASICHANDLER_HPP
