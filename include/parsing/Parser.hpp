//
// Created by Daniel Ziper on 4/21/24.
//

#ifndef ECE141DB_PARSER_HPP
#define ECE141DB_PARSER_HPP

#include "misc/Errors.hpp"
#include "TokenParser.hpp"

namespace ECE141 {
    class TokenParser;

    struct Parser {
    public:
        Errors error = Errors::noError;
        // Setting an error here means TokenParser will fully abort and report the error to the user

        // Additional state for subclasses, such as results of parsing
        Parser()=default;
        virtual ~Parser()=default;
        virtual bool operator()(TokenParser& tokenParser)=0;
        virtual void reset() {error = Errors::noError;};
    };

    template <typename T>
    struct SingleParser : public Parser {
    public:
        T out;

        SingleParser()=default;
        SingleParser(const SingleParser& aCopy)=default;

        virtual void reset() override {
            out = T();
        }
    };
}

#endif //ECE141DB_PARSER_HPP
