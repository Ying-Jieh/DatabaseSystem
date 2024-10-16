//
//  TokenParser.hpp
//  Database
//

#ifndef TokenParser_h
#define TokenParser_h

#include "tokenizer/Tokenizer.hpp"
#include "tokenizer/keywords.hpp"
#include "parsing/Parser.hpp"
#include "misc/ParseHelper.hpp"
#include <initializer_list>

namespace ECE141 {

    class TokenParser {
    public:
        TokenParser(Tokenizer &aTokenizer);

        TokenParser &nextIs(const KWList &aList, bool autoSkip = true);

        bool checkNextIs(const KWList &aList, bool autoSkip = true); // Checks if all of the next keywords occur in the order of aList
        bool checkNextInList(const KWList &aList); // Checks if the next keyword is in the list

        TokenParser &getKeyword(Keywords &aKeyword, bool autoSkip = true);
        TokenParser &getKeywordIn(Keywords &aKeyword, const KWList &aList, bool autoSkip = true);
        TokenParser &getIdentifier(std::string &aName, bool autoSkip = true);
        TokenParser &getNumber(int &aValue, bool autoSkip = true);

        TokenParser &skipPast(Keywords aKW);
        TokenParser &skipPast(char aChar);
        TokenParser &skip(size_t aCount);
        TokenParser &skipIfData(const std::string& aData, bool& skipped, bool required=true);

        bool currentIsType(TokenType type);

        TokenParser& parseWith(Parser& aParser, bool requireSuccess=true);
        TokenParser& tryParseWith(Parser& aParser, bool& succeeded);

        operator bool() const { return state; }
        TokenParser& setCheckpoint() {checkpoint = tokenizer.pos(); return *this;}
        TokenParser &clear();

        Token &current() const { return tokenizer.current(); }

        StatusResult getState() {
            return state;
        }

        Tokenizer &tokenizer;

    protected:
        StatusResult state;
        size_t checkpoint = 0;
    };

}

#endif /* TokenParser_h */
