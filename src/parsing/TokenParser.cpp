//
// Created by Daniel Ziper on 4/21/24.
//

#include "parsing/TokenParser.hpp"
#include "tokenizer/Tokenizer.hpp"
#include "tokenizer/keywords.hpp"
#include <initializer_list>
#include <set>

namespace ECE141 {


    TokenParser::TokenParser(Tokenizer &aTokenizer) : tokenizer(aTokenizer), state{Errors::noError} {};

    TokenParser& TokenParser::nextIs(const KWList &aList, bool autoSkip) {
        if (!checkNextIs(aList, autoSkip)) {
            state = {Errors::keywordExpected, tokenizer.pos()};
        }
        return *this;
    }


    // Check next keywords without returning an error if failed
    bool TokenParser::checkNextIs(const KWList &aList, bool autoSkip) {
        if (state) {
            std::vector<Keywords> theList{aList};
            int thePos{0};
            for (auto theKW: theList) {
                if (tokenizer.remaining() == 0) {
                    return false;
                }
                Token &theToken = tokenizer.peek(thePos++);
                if (theKW != theToken.keyword) {
                    return false;
                }
            }
            if (state) {
                if (autoSkip) skip(aList.size());
                return true;
            }
        }
        return false;
    }

    bool TokenParser::checkNextInList(const ECE141::KWList &aList) {
        for (auto kw : aList) {
            if (checkNextIs({kw}, false)) return true;
        }
        return false;
    }

    TokenParser& TokenParser::getKeywordIn(Keywords &aKeyword, const KWList &aList, bool autoSkip) {
        if (!state) return *this;
        Token &theToken = tokenizer.current();
        if (theToken.type != TokenType::keyword) {
            state = {Errors::keywordExpected, tokenizer.pos()};
            return *this;
        }
        std::set<Keywords> list{aList};
        if (list.find(theToken.keyword) != list.end()) {
            aKeyword = theToken.keyword;
            if (autoSkip) skip(1);
            return *this;
        }
        state = {Errors::keywordExpected, tokenizer.pos()};
        return *this;
    }

    TokenParser& TokenParser::getKeyword(Keywords &aKeyword, bool autoSkip) {
        if (!state) return *this;
        Token &theToken = tokenizer.current();
        if (theToken.type != TokenType::keyword) {
            state = {Errors::keywordExpected, tokenizer.pos()};
            return *this;
        }
        aKeyword = theToken.keyword;
        if (autoSkip) skip(1);
        return *this;
    }

    TokenParser& TokenParser::getIdentifier(std::string &aName, bool autoSkip) {
        if (!state) return *this;
        Token &theToken = tokenizer.current();
        if (theToken.type != TokenType::identifier) {
            state = {Errors::identifierExpected, tokenizer.pos()};
            return *this;
        }
        aName = theToken.data;
        if (autoSkip) skip(1);
        return *this;
    }

    TokenParser& TokenParser::getNumber(int &aValue, bool autoSkip) {
        if (!state) return *this;
        Token &theToken = tokenizer.current();
        if (theToken.type != TokenType::number) {
            state = {Errors::valueExpected, tokenizer.pos()};
            return *this;
        }
        aValue = std::stoi(theToken.data);
        if (autoSkip) skip(1);
        return *this;
    }

    TokenParser& TokenParser::skipPast(Keywords aKW) {
        if (state) {
            do {
                auto theToken = tokenizer.current();
                if (theToken.keyword == aKW) {
                    tokenizer.next();//skip it...
                    return *this;
                }
            } while (tokenizer.next());
            state = {Errors::keywordExpected, tokenizer.pos()};
        }
        return *this;
    }

    TokenParser& TokenParser::skipPast(char aChar) {
        if (state) {
            do {
                auto theToken = tokenizer.current();
                if (theToken.type == TokenType::punctuation) {
                    if (theToken.data[0] == aChar) {
                        tokenizer.next();//skip it...
                        return *this;
                    }
                }
            } while (tokenizer.next());
            state = {Errors::punctuationExpected, tokenizer.pos()};
        }
        return *this;
    }

    TokenParser& TokenParser::skip(size_t aCount) {
        if (state) {
            tokenizer.next((int) aCount); //actually skip...
        }
        return *this;
    }

    TokenParser& TokenParser::skipIfData(const std::string& aData, bool& skipped, bool required) {
        if (state) {
            if (tokenizer.current().data == aData) {
                skipped = true;
                tokenizer.next();
            } else {
                skipped = false;
                if (required) {
                    state = {Errors::parsingError};
                }
            }
        } else {
            skipped = false;
        }
        return *this;
    }

    bool TokenParser::currentIsType(ECE141::TokenType type) {
        if (state) {
            if (tokenizer.current().type == type) return true;
        }
        return false;
    }

    TokenParser& TokenParser::tryParseWith(ECE141::Parser &aParser,
                                       bool& succeeded)  {
        if (state) {
            size_t checkpointPos = tokenizer.pos();
            StatusResult checkpointState = state;
            // Store other state here?
            if (aParser(*this)) { // fall through
                succeeded = true;
            } else {
                succeeded = false;
                tokenizer.seek(checkpointPos);
                state = checkpointState;
            }
        }
        return *this;
    }
    TokenParser& TokenParser::parseWith(ECE141::Parser &aParser,
                                        bool requireSuccess) {
        if (state) {
            bool succeeded = false;
            tryParseWith(aParser, succeeded);
            if (!succeeded && requireSuccess) {
                if (aParser.error != Errors::noError) {
                    state = {aParser.error, tokenizer.pos()};
                } else {
                    if (state) {
                        // No error from parsing in Parser, some unknown error
                        state.error = Errors::parsingError;
                    }
                    state.value = tokenizer.pos();
                }
            }
        }
        return *this;
    }

    TokenParser& TokenParser::clear() {
        state = {Errors::noError};
        tokenizer.seek(checkpoint);
        return *this;
    }

};
