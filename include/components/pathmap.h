#ifndef PATH_MAP_H
#define PATH_MAP_H

#include <list>
#include <vector>
#include <queue>
#include <functional>

#include "hadron/hadron.h"

struct PathMap {
  struct Node {

    void add_neighbor(PathMap::Node* neighbor) {
      if(neighbor == nullptr) {
        Log::get_core_logger()->error("neighbor is null");
        return;
      }
      this->neighbors.push_back(neighbor);
    }

    void add_neighbors(const std::vector<PathMap::Node*>& neighbors) {
      for(auto neighbor : neighbors) {
        this->add_neighbor(neighbor);
      }
    }

    glm::vec3 transform; // Necessary for empty locations
    std::list<PathMap::Node*> neighbors;
    bool visited;
  };

  Node* get(const glm::vec3 transform) {
    // Log::get_core_logger()->trace("({}, {})", transform.x, transform.y);
    for(auto& node : this->nodes) {
      // Log::get_core_logger()->trace("({}, {})", node.transform.x, node.transform.y);
      if(node.transform.x == transform.x && node.transform.y == transform.y) {
        return &node;
      }
    }
    return nullptr;
  }

  void add_node(const glm::vec3 transform) {
    nodes.push_back(PathMap::Node());
    nodes.back().transform = transform;
  }

  std::queue<PathMap::Node*> bfs(
    Scene& scene,
    glm::vec3 start,
    std::function<bool(Scene& scene, PathMap::Node*)> target_fn,
    std::function<bool(Scene& scene, PathMap::Node*)> avoid_fn = [](Scene& scene, PathMap::Node*) -> bool {return false;}
  ) {
    return this->bfs(scene, this->get(start), target_fn, avoid_fn);
  }

  std::queue<PathMap::Node*> bfs(
    Scene& scene,
    PathMap::Node* start,
    std::function<bool(Scene& scene, PathMap::Node*)> target_fn,
    std::function<bool(Scene& scene, PathMap::Node*)> avoid_fn = [](Scene& scene, PathMap::Node*) -> bool {return false;}
  ) {
    if(start == nullptr) {
      Log::get_core_logger()->error("Start node doesn't exist");
      return std::queue<PathMap::Node*>();
    }
    for(auto& node : this->nodes) {
      Log::get_client_logger()->trace("node ({}, {})({:p}) visited: {}", node.transform.x, node.transform.y, (void*)(&node), node.visited);
      node.visited = false;
    }
    std::queue<std::queue<PathMap::Node*>> queue;
    start->visited = true;

    std::queue<PathMap::Node*> path;
    path.push(start);
    queue.push(path);

    while(!queue.empty()) {
      path = queue.front();
      PathMap::Node* n = path.back();
      queue.pop();
      Log::get_core_logger()->trace("node: ({}, {})", n->transform.x, n->transform.y);

      for(auto neighbor : n->neighbors) {
        Log::get_core_logger()->trace("Checking ({}, {})({:p})", neighbor->transform.x, neighbor->transform.y, (void*)neighbor);
        if(!neighbor->visited) {
          neighbor->visited = true;

          std::queue<PathMap::Node*> new_path(path);
          new_path.push(neighbor);

          if(!avoid_fn(scene, neighbor)) { // Safe?
            if(target_fn(scene, neighbor)) { // Target?
              Log::get_core_logger()->trace("Found path!");
              return new_path; // Found
            }
            queue.push(new_path);
            Log::get_core_logger()->trace("path to ({}, {})", neighbor->transform.x, neighbor->transform.y);
          }
        } else {
          Log::get_core_logger()->trace("Already checked");
        }
      }
    }
    Log::get_core_logger()->trace("Couldn't find path!");
    return std::queue<PathMap::Node*>(); // No Path for input
  }

  std::list<PathMap::Node> nodes;
};

#endif // PATH_MAP_H
