#ifndef ACTMAN_H
#define ACTMAN_H

#include "application.h"
#include "board/boardconfig.h"

class ActMan : public Application {
public:
  virtual void parse(std::vector<std::string> argv) override;
  virtual void configure() override;
  virtual const char* usage() const override;
  virtual void reset();
protected:
  virtual const char* print_help() const override;

  BoardConfig config;

  std::filesystem::path config_file;
  std::filesystem::path output_file;
};

#endif // ACTMAN_H
