#pragma once
#include "collider.h"
#include "object.h"

class Enemy : public Object, public Collider {
 public:
  enum class Direction {
    CENTER = -1,
    TOP_LEFT_CORNER,
    BOTTOM_LEFT_CORNER,
    BOTTOM_RIGHT_CORNER,
    TOP_RIGHT_CORNER
  };

  Enemy(const glm::vec2& cellScale, float speed, const glm::vec3& pos,
        Mesh* mesh = nullptr, Shader* shader = nullptr)
      : Object(glm::vec3{0, 0, 0}, glm::vec3{0, 0, 0}, glm::vec3{1, 1, 1}, mesh,
               shader) {
    setScale(glm::vec3{0.03f, 0.03f, 0.03f});
    setPosition(glm::vec3{pos.x, pos.y + 0.5f, pos.z});
    this->speed = speed;
    corners[0] = glm::vec3(getPosition().x - cellScale.x / 2, getPosition().y,
                           getPosition().z - cellScale.y / 2);
    corners[1] = glm::vec3(getPosition().x - cellScale.x / 2, getPosition().y,
                           getPosition().z + cellScale.y / 2);
    corners[2] = glm::vec3(getPosition().x + cellScale.x / 2, getPosition().y,
                           getPosition().z + cellScale.y / 2);
    corners[3] = glm::vec3(getPosition().x + cellScale.x / 2, getPosition().y,
                           getPosition().z - cellScale.y / 2);
    currDir = Direction::CENTER;
    nextDir = Direction::TOP_LEFT_CORNER;
    animate = false;
  }
  ~Enemy();
  void Update(float) override;
  void Draw(const glm::mat4&, const glm::mat4&) override;
  void setPosition(const glm::vec3&) override;
  void setScale(const glm::vec3&) override;
  void OnCollisionEnter(const Collider&) override;
  int animate;

 private:
  float speed;
  float animDuration = 2.f;
  float passed = 0;

  glm::vec3 corners[4];
  Direction currDir;
  Direction nextDir;
};