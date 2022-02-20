#include "obstacle.h"

void Obstacle::Draw(const glm::mat4& view, const glm::mat4& proj) {
  glm::mat4 modelMatrix = glm::mat4(1);
  modelMatrix = glm::translate(modelMatrix, getPosition());
  modelMatrix = glm::scale(modelMatrix, getScale());
  if (type == 0)
    RenderMesh(modelMatrix, view, proj, glm::vec3{1.000f, 0.843f, 0.f});
  else
    RenderMesh(modelMatrix, view, proj, glm::vec3{0.741f, 0.718f, 0.420f});
}

void Obstacle::setPosition(const glm::vec3& pos) {
  Transform::setPosition(pos);
  setColPosition(pos);
}

void Obstacle::setScale(const glm::vec3& sc) {
  Transform::setScale(sc);
  setColScale(sc);
}
