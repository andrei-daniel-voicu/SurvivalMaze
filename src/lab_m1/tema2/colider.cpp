#include "collider.h"

Collider::Collider(const glm::vec3& center, const glm::vec3& scale) {
  this->center = center;
  this->scale = scale;
}

bool Collider::CheckCollision(const glm::vec3& pos1, const glm::vec3& scale1,
                              const glm::vec3& pos2, const glm::vec3& scale2) {
  // check the x axis
  if (abs(pos1.x - pos2.x) >= scale1.x / 2 + scale2.x / 2) return false;

  // check the y axis
  if (abs(pos1.y - pos2.y) >= scale1.y / 2 + scale2.y / 2) return false;

  // check the z axis
  if (abs(pos1.z - pos2.z) >= scale1.z / 2 + scale2.z / 2) return false;

  return true;
}

void Collider::OnCollisionEnter(const Collider&) {}

void Collider::setColPosition(const glm::vec3& pos) { center = pos; }

void Collider::setColScale(const glm::vec3& sc) { scale = sc; }

glm::vec3 Collider::getColPosition() const { return center; }

glm::vec3 Collider::getColScale() const { return scale; }
