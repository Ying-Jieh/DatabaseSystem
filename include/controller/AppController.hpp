//
//  AppProcessor.hpp
//  Database5
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//

#ifndef AppController_hpp
#define AppController_hpp

#include <stdio.h>
#include <memory>
#include <ostream>
#include "misc/Config.hpp"
#include "misc/Errors.hpp"
#include "view/View.hpp"
#include "commandHandler/CommandHandler.hpp"
#include "database/Database.hpp"
#include "statement/ExecutionContext.hpp"

namespace ECE141 {
    static Timer theTimer = Config::getTimer();;
    static Timer::Marker theStart = theTimer.started();
    class AppController {
    public:

        AppController(std::ostream &anOutput = std::cout);

        virtual ~AppController();

        virtual StatusResult handleInput(std::istream &anInput,
                                         ViewListener aViewer);

        bool isRunning() const { return running; }

        static OptString getError(StatusResult &aResult);

        bool running;
        std::ostream &output;
        ViewListener currentViewListener;
        std::shared_ptr<Database> currentDatabase = std::make_shared<Database>();
        ExecutionContext context{currentDatabase, currentViewListener};

    protected:
        std::vector<CommandHandler *> handlers;
    };

}

#endif /* AppController_hpp */
