#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>

#include "log.h"
#include "actman.h"
#include "error.h"
// #include "boardconfig.h"

void ActMan::parse(std::vector<std::string> argv) {

  uint32_t pos = 0;

  for (auto opt : argv) {
    if(opt[0] == '-') { // option
      if(opt[1] == '-') { // long option
      } else { // short option
        switch(opt[1]) {
          case 'h': // help
          case '?':
            printf("%s\n", this->print_help());
            this->running = false;
            return;
          case 'v': // verbose
            this->core_level = spdlog::level::trace;
            this->client_level = spdlog::level::trace;
            break;
          default:
            throw NoOptError(&opt[1]);
        }
      }
    } else { // positional argument
      switch(pos) {
        case 0: // config file
          this->config_file = std::filesystem::path(opt);
          break;
        case 1:
          this->output_file = std::filesystem::path(opt);
          break;
        default:
          throw UnexpPosOptError(opt.c_str());
      }
      pos++;
    }
  }
  Log::init("ActMan", this->core_level, this->client_level);
}

const char* ActMan::print_help() const {
  std::stringstream ss;
  ss << ActMan::usage() << "\n";
  ss << "\t" << "config_file - path to board configuration file" << "\n";
  ss << "\t" << "output_file - path to output file of simulation" << "\n";
  ss << "\n" << "OPTIONS" << "\n\n";
  ss << "\t" << "-v" << "\t\t" << "Enable verbose output" << "\n";
  return ss.str().c_str();
}

const char* ActMan::usage() const {
  std::stringstream ss;
  ss << "USAGE: actman [OPTIONS] config_file output_file" << "\n";
  return ss.str().c_str();
}

void ActMan::configure() {
  this->config.load_config(this->config_file);
  // Todo: setup scene
}

void ActMan::reset() {

}
