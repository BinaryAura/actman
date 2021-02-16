#ifndef ACTMAN_H
#define ACTMAN_H

#include "actmancommon.h"
#include "application.h"
#include "board/boardconfig.h"
#include "physics/tilephysics.h"

class ActMan : public Application {
public:
  virtual void parse(std::vector<std::string> argv) override;
  virtual void configure() override;
  virtual const std::string usage() const override;
  virtual void reset() override;
protected:
  virtual const std::string print_help() const override;
  static void punky_ai(Scene&, Entity);
  static void bunky_ai(Scene&, Entity);
  static void dunky_ai(Scene&, Entity);
  static void runky_ai(Scene&, Entity);

  static BoardConfig config;
  std::filesystem::path config_file;
  std::filesystem::path output_file;
};


#endif // ACTMAN_H
