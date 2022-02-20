#include "drawable.h"

Drawable::Drawable(Mesh* mesh, Shader* shader) {
  this->mesh = mesh;
  this->shader = shader;
}

Drawable::~Drawable() {}

void Drawable::Draw(const glm::mat4&, const glm::mat4&) {}

void Drawable::RenderMesh(const glm::mat4& modelMatrix,
                          const glm::mat4& viewMatrix,
                          const glm::mat4& projMatrix, const glm::vec3& color) {
  if (!mesh || !shader || !shader->GetProgramID()) return;

  // Render an object using the specified shader and the specified position
  glUseProgram(shader->program);

  int location = glGetUniformLocation(shader->program, "object_color");
  glUniform3fv(location, 1, glm::value_ptr(color));

  // Bind model matrix
  GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
  glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE,
                     glm::value_ptr(modelMatrix));

  // Bind view matrix
  int loc_view_matrix = glGetUniformLocation(shader->program, "View");
  glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

  // Bind projection matrix
  int loc_projection_matrix =
      glGetUniformLocation(shader->program, "Projection");
  glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE,
                     glm::value_ptr(projMatrix));

  // Draw the object
  glBindVertexArray(mesh->GetBuffers()->m_VAO);
  glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()),
                 GL_UNSIGNED_INT, 0);
}

void Drawable::RenderMesh(float time, int animate, const glm::mat4& modelMatrix,
                          const glm::mat4& viewMatrix,
                          const glm::mat4& projMatrix, const glm::vec3& color) {
  if (!mesh || !shader || !shader->GetProgramID()) return;

  // Render an object using the specified shader and the specified position
  glUseProgram(shader->program);

  // color
  int loc_color = glGetUniformLocation(shader->program, "object_color");
  glUniform3fv(loc_color, 1, glm::value_ptr(color));

  // animate
  int loc_animate = glGetUniformLocation(shader->program, "animate");
  glUniform1i(loc_animate, animate);

  // time
  int loc_time = glGetUniformLocation(shader->program, "time");
  glUniform1f(loc_time, time);

  // Bind model matrix
  GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
  glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE,
                     glm::value_ptr(modelMatrix));

  // Bind view matrix
  int loc_view_matrix = glGetUniformLocation(shader->program, "View");
  glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

  // Bind projection matrix
  int loc_projection_matrix =
      glGetUniformLocation(shader->program, "Projection");
  glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE,
                     glm::value_ptr(projMatrix));

  // Draw the object
  glBindVertexArray(mesh->GetBuffers()->m_VAO);
  glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()),
                 GL_UNSIGNED_INT, 0);
}