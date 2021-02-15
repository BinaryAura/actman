#include <math.h>

#include "physics/tilephysics.h"
#include "scene.h"
#include "components/tilemove.h"
#include "components/tilecollider.h"

void TilePhysics::on_update(Scene &scene, Transform& transform, Mobile& mobile, Collider& collider) {
  auto group = scene.get_group<TileCollider>(entt::get_t<Transform>{});
  group.sort<Transform>([transform](const auto& lhs, const auto& rhs)->bool{
    return abs(lhs.x - transform.x) + abs(lhs.y - transform.y) < abs(rhs.x - transform.x) + abs(rhs.y - transform.y);
  });
  auto tm = (TileMove&)mobile;
  auto coll = (TileCollider&)collider;
  for(auto entity : group) {
    auto [c, t] = group.get(entity);
    if (abs(t.x - transform.x) + abs(t.y - transform.y) > abs(tm.x - transform.x) + abs(tm.y - transform.y)) {
      break; // sorted list distance is too far now, break
    } else if (t.x == tm.x && t.y == tm.y && coll.solid && c.solid && (coll.fills_tile || c.fills_tile) ) {
      return; // Can't move to (tm.x, tm.y)
    }
  }
  transform.x = ((TileMove&)mobile).x;
  transform.y = ((TileMove&)mobile).y;
}
