#pragma once
#include "collider.h"
#include "object.h"

class Bullet : public Object, public Collider {
 public:
  Bullet(const glm::vec3&, const glm::vec3&, float, Mesh*, Shader*);
  void Update(float) override;
  void Draw(const glm::mat4&, const glm::mat4&) override;
  void setPosition(const glm::vec3&) override;
  void setScale(const glm::vec3&) override;
  void OnCollisionEnter(const Collider&) override;

  float distanceTraveled;

 private:
  float speed;
  glm::vec3 dir;
};