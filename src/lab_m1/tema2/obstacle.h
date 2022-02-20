#pragma once
#include "collider.h"
#include "object.h"

class Obstacle : public Object, public Collider {
 public:
  Obstacle(bool t, const glm::vec3& pos, const glm::vec3& rot,
           const glm::vec3& sc, Mesh* mesh, Shader* shader)
      : Object(pos, rot, sc, mesh, shader) {
    this->type = t;
    setScale(sc);
    setPosition(pos);
  };

  void Draw(const glm::mat4&, const glm::mat4&) override;
  void setPosition(const glm::vec3&) override;
  void setScale(const glm::vec3&) override;

 private:
  bool type;
};