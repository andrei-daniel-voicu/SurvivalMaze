#pragma once
#include "transform.h"

class Collider {
 public:
  Collider() : Collider(glm::vec3{0.f, 0.f, 0.f}, glm::vec3{1.f, 1.f, 1.f}){};
  Collider(const glm::vec3&, const glm::vec3&);
  bool CheckCollision(const glm::vec3&, const glm::vec3&, const glm::vec3&,
                      const glm::vec3&);
  virtual void OnCollisionEnter(const Collider&);

  void setColPosition(const glm::vec3&);
  void setColScale(const glm::vec3&);

  glm::vec3 getColPosition() const;
  glm::vec3 getColScale() const;

 private:
  glm::vec3 center;
  glm::vec3 scale;
};