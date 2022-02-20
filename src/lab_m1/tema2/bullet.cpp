#include "bullet.h"

#include <iostream>

Bullet::Bullet(const glm::vec3& pos, const glm::vec3& dir, float speed,
               Mesh* mesh, Shader* shader)
    : Object(mesh, shader) {
  this->speed = speed;
  this->distanceTraveled = 0;
  this->dir = dir;

  setPosition(pos);
  setScale(glm::vec3{0.5f, 0.5f, 0.5f});
}

void Bullet::Update(float deltaTime) {
  glm::vec3 oldPos = getPosition();
  glm::vec3 newPos = oldPos + dir * deltaTime * speed;
  setPosition(newPos);
  distanceTraveled += glm::distance(newPos, oldPos);
}

void Bullet::Draw(const glm::mat4& view, const glm::mat4& proj) {
  glm::mat4 modelMatrix = glm::mat4(1);
  modelMatrix = glm::translate(modelMatrix, getPosition());
  modelMatrix = glm::scale(modelMatrix, getScale());
  RenderMesh(modelMatrix, view, proj,
             glm::vec3{224 / 255.f, 207 / 255.f, 182 / 255.f});
}

void Bullet::setPosition(const glm::vec3& pos) {
  Transform::setPosition(pos);
  setColPosition(pos);
}

void Bullet::setScale(const glm::vec3& sc) {
  Transform::setScale(sc);
  setColScale(glm::vec3{sc.x, sc.y, sc.z});
}

void Bullet::OnCollisionEnter(const Collider& col) { Destroy(); }