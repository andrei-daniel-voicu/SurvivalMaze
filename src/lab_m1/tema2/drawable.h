#pragma once
#include "core/gpu/mesh.h"
#include "core/gpu/shader.h"
#include "glm/glm.hpp"

class Drawable {
 public:
  Drawable(Mesh* mesh, Shader* shader);
  ~Drawable();

  Mesh* mesh;
  Shader* shader;
  virtual void Draw(const glm::mat4&, const glm::mat4&);

  void RenderMesh(const glm::mat4&, const glm::mat4&, const glm::mat4&,
                  const glm::vec3&);
  void RenderMesh(float, int, const glm::mat4&, const glm::mat4&,
                  const glm::mat4&, const glm::vec3&);
};