#ifndef COMPONENTS_TILE_MAP_H
#define COMPONENTS_TILE_MAP_H

#include "components/transform.h"
#include "components/tilecollider.h"

struct TileMap {
  TileMap() = default;
  TileMap(const TileMap&) = default;
  TileMap(const uint32_t rows, const uint32_t cols, const uint32_t cell_width = 1, const uint32_t cell_height = 1) : rows(rows), cols(cols), cell_width(cell_width), cell_height(cell_height) {}

  uint32_t cell_width;
  uint32_t cell_height;
  uint32_t cols;
  uint32_t rows;

  uint32_t distance(const Transform& lhs, const Transform& rhs) const {
    return abs(lhs.x - rhs.x) + (lhs.y - rhs.y);
  }

  std::vector<Transform> get_neighbors(Transform& transform) const {
    return {{transform.x, transform.y - 1, 0, 0},
            {transform.x + 1, transform.y, 0, 0},
            {transform.x, transform.y + 1, 0, 0},
            {transform.x - 1, transform.y, 0, 0}};
  }

  std::vector<Entity> get_entities(Scene& scene, const Transform& transform) const {
    return scene.filtered_view<Transform>([transform](Entity entity){
      auto& t = entity.get_component<Transform>();
      return t.x == transform.x && t.y == transform.y;
    });
  }
};

#endif // COMPONENTS_TILE_MAP_H
