//
// Created by Daniel Ziper on 4/3/24.
//

#ifndef ECE141DB_COMMANDHANDLER_HPP
#define ECE141DB_COMMANDHANDLER_HPP

#include "controller/AppController.hpp"
#include "tokenizer/Tokenizer.hpp"
#include "statement/ExecutionContext.hpp"

namespace ECE141 {
//    class AppController; // forward declare

    class CommandHandler {
    public:
//        explicit CommandHandler(AppController* aController=nullptr) : controller(aController) {};
        explicit CommandHandler(ExecutionContext& aContext) : context(aContext){};
        CommandHandler(CommandHandler& aCopy) = default;
        virtual ~CommandHandler() = default;
        virtual StatusResult handle(Tokenizer&)=0;

    protected:
//        AppController* controller;
        ExecutionContext& context;
    };

}

#endif //ECE141DB_COMMANDHANDLER_HPP
