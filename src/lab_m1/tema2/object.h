#pragma once
#include "components/simple_scene.h"
#include "drawable.h"
#include "transform.h";

class Object : public Transform, public Drawable {
 public:
  Object(Mesh* mesh, Shader* shader);
  Object(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sc,
         Mesh* mesh = nullptr, Shader* shader = nullptr)
      : Transform(pos, rot, sc), Drawable(mesh, shader) {
    toDestroy = false;
  }
  ~Object();

  virtual void Update(float);
  void Destroy();
  bool isDestroy() const;

 private:
  bool toDestroy;
};