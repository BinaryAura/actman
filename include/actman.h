#ifndef ACTMAN_H
#define ACTMAN_H

#include <hadron/hadron.h>
#include "actmancommon.h"
// #include "application.h"
#include "boardconfig.h"
// #include "physics/tilephysics.h"

class ActMan : public Application {
public:
  ActMan() : Application("ActMan") {}
  ActMan(const int32_t argc, const char** argv) {
    this->setup(argc, argv);
  }

  virtual void parse(std::vector<std::string> argv) override;
  virtual void configure() override;
  virtual const std::string usage() const override;
  virtual void reset() override;
  virtual const std::string print_help() const override;
protected:
  virtual void on_close() override;

  entt::entity actman;
  BoardConfig config;
  std::filesystem::path config_file;
  std::filesystem::path output_file;
private:
  void create_board();
  void spawn_wall(const uint32_t x, const uint32_t y);
  void spawn_nugget(const uint32_t x, const uint32_t y);
  void spawn_bar(const uint32_t x, const uint32_t y);
  void spawn_diamond(const uint32_t x, const uint32_t y);
  void spawn_actman(const uint32_t x, const uint32_t y);
  void spawn_punky(const uint32_t x, const uint32_t y);
  void spawn_bunky(const uint32_t x, const uint32_t y);
  void spawn_dunky(const uint32_t x, const uint32_t y);
  void spawn_runky(const uint32_t x, const uint32_t y);
};


#endif // ACTMAN_H
