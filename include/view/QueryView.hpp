//
// Created by YING JIEH XIA on 2024/4/15.
//

#ifndef ECE141DB_QUERYVIEW_HPP
#define ECE141DB_QUERYVIEW_HPP

#include "View.hpp"
#include "misc/Errors.hpp"
#include "controller/AppController.hpp"
#include "misc/Helpers.hpp"

namespace ECE141 {

    //completely generic view, which you will subclass to show information
    class QueryView : public View {
    public:
        QueryView(StatusResult& aResult, double aDuration=-1)
            : theDuration(aDuration), theResult(aResult) {};

        bool show(std::ostream &aStream) override {
            if (theResult.error == Errors::noError) {
                aStream << "Query OK, "
                        << theResult.value
                        << " rows affected ("
                        << std::fixed << theDuration << " sec)"
                        << " - "
                        << std::endl;
                return true;
            } else {
                aStream << "Error " << (int)theResult.error << ": "
                        << Helpers::ErrorToString(theResult.error)
                        << " at line 1";
            }
            return false;
        }

    private:
        double theDuration;
        StatusResult theResult;

//        ViewListener listener; // Member variable that stores a function or function object
    };

}


#endif //ECE141DB_QUERYVIEW_HPP
