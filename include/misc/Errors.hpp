//
//  basetypes.hpp
//  Database
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//

#ifndef Errors_h
#define Errors_h

#include "misc/BasicTypes.hpp"
#include <cstdint>

#define returnIfError(x) if (!x) return x;

namespace ECE141 {

    enum class Errors : uint16_t {

        noError = 0,

        //parse related...
        keywordExpected = 100,
        identifierExpected,
        illegalIdentifier,
        unknownIdentifier,
        unexpectedIdentifier,
        joinTypeExpected,
        keyExpected,
        syntaxError,
        unexpectedKeyword,
        unexpectedValue,
        valueExpected,
        operatorExpected,
        punctuationExpected,
        parsingError,
        eofError,

        //statement related...
        statementExpected = 200,
        noDatabaseSpecified,

        //table/DB related...
        tableExists = 300,
        unknownEntity,
        unknownTable,
        unknownDatabase,
        databaseExists,
        databaseCreationError,
        primaryKeyRequired,
        invalidTableName,

        //type related...
        unknownType = 400,
        unknownAttribute,
        invalidAttribute,
        invalidArguments,
        keyValueMismatch, //# of fieldnames doesn't match values...
        notNullable,
        hashExists,
        typeMismatch,
        lengthMismatch,
        keyExists,

        //storage/io related...
        readError = 500,
        writeError,
        seekError,
        storageFull,
        invalidBlockType,

        //index related
        indexExists = 600,
        cantCreateIndex,
        unknownIndex,
        indexError,

        //command related...
        unknownCommand = 3000,
        invalidCommand,

        //general purpose...
        userTerminated = 4998,
        notImplemented,

        unknownError = 10000
    };

    struct StatusResult {
        Errors error;
        uint32_t value;

        StatusResult(Errors anError = Errors::noError) :
                error(anError), value(0) {}

        StatusResult(Errors anError, size_t aValue) :
                error(anError), value(aValue) {}

        operator bool() const { return Errors::noError == error; }

//    operator Errors() {return error;}
        bool operator==(Errors anError) { return anError == error; }
    };

}

#endif /* errors */
