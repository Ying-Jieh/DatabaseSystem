//
//  CommandProcessor.cpp
//  ECEDatabase
//
//  Created by rick gessner on 3/30/23.
//  Copyright © 2018-2023 rick gessner. All rights reserved.
//

#include <iostream>
#include <memory>
#include "controller/AppController.hpp"
#include "tokenizer/Tokenizer.hpp"
#include "commandHandler/BasicHandler.hpp"
#include "commandHandler/DatabaseHandler.hpp"
#include "commandHandler/SQLProcessor.hpp"
#include "misc/Logger.hpp"

namespace ECE141 {
  
  AppController::AppController(std::ostream &anOutput) : running{true}, output{anOutput} {
      Logger::setLevel(Config::getLogLevel());
      TRACE("Creating AppController")
      handlers.push_back(new BasicHandler(context));
      handlers.push_back(new DatabaseHandler(context));
      handlers.push_back(new SQLProcessor(context));
  }
  
  AppController::~AppController() {
      for (auto handler : handlers) {
          delete handler;
      }
  }
  
  // USE: -----------------------------------------------------
  
  //build a tokenizer, tokenize input, ask processors to handle...
  StatusResult AppController::handleInput(std::istream &anInput, ViewListener aViewer) {
      INFO("AppController started handling input")
      currentViewListener = aViewer;
      if (!isRunning()) {
          return Errors::userTerminated;
      }

      Tokenizer theTokenizer(anInput);
      StatusResult theResult=theTokenizer.tokenize();

      theTimer=Config::getTimer();
      theStart=theTimer.now();
      for (auto handler : handlers) {
          theResult = handler->handle(theTokenizer);
          if (theResult.error == Errors::noError || theResult.error != Errors::unknownCommand) {
              break;
          }
      }

      if (running) {
          QueryView theView(theResult, theTimer.elapsed(theStart));
          this->currentViewListener(theView);
          return theResult.error;
      } else {
          INFO("User terminated")
          return Errors::userTerminated;
      }
  }

  OptString AppController::getError(StatusResult &aResult) {

    static std::map<ECE141::Errors, std::string_view> theMessages = {
      {Errors::userTerminated, "User Terminated"},
      {Errors::illegalIdentifier, "Illegal identifier"},
      {Errors::unknownIdentifier, "Unknown identifier"},
      {Errors::databaseExists, "Database exists"},
      {Errors::tableExists, "Table Exists"},
      {Errors::syntaxError, "Syntax Error"},
      {Errors::unknownCommand, "Unknown command"},
      {Errors::unknownDatabase,"Unknown database"},
      {Errors::unknownTable,   "Unknown table"},
      {Errors::unknownError,   "Unknown error"}
    };

    std::string_view theMessage="Unknown Error";
    if(theMessages.count(aResult.error)) {
      theMessage=theMessages[aResult.error];
    }
    return theMessage;
  }




}
