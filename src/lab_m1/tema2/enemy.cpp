#include "enemy.h"

#include "bullet.h"

Enemy::~Enemy() {}

void Enemy::Update(float deltaTime) {
  if (passed >= animDuration) {
    Destroy();
    return;
  }
  if (animate) {
    passed += deltaTime;
    return;
  }
  if (glm::distance(getPosition(), corners[static_cast<int>(nextDir)]) <=
      0.7f) {
    currDir = nextDir;
    nextDir = static_cast<Direction>((static_cast<int>(currDir) + 1) % 4);
  } else {
    glm::vec3 dir =
        currDir == Direction::CENTER
            ? glm::normalize(corners[static_cast<int>(nextDir)] - getPosition())
            : glm::normalize(corners[static_cast<int>(nextDir)] -
                             corners[static_cast<int>(currDir)]);
    glm::vec3 pos = getPosition() + dir * speed * deltaTime;
    setPosition(pos);
  }
}

void Enemy::Draw(const glm::mat4& view, const glm::mat4& proj) {
  glm::mat4 modelMatrix = glm::mat4(1);
  modelMatrix = glm::translate(modelMatrix, getPosition());
  modelMatrix = glm::scale(modelMatrix, getScale());
  RenderMesh(Engine::GetElapsedTime(), animate, modelMatrix, view, proj,
             glm::vec3{0.545f, 0.271f, 0.075f});
}

void Enemy::setPosition(const glm::vec3& pos) {
  Transform::setPosition(pos);
  setColPosition(pos);
}

void Enemy::setScale(const glm::vec3& sc) {
  Transform::setScale(sc);
  setColScale(glm::vec3{sc.x + 1.9f, sc.y + 1.9f, sc.z + 1.9f});
}

void Enemy::OnCollisionEnter(const Collider& col) {
  if (dynamic_cast<const Bullet*>(&col) != nullptr) {
    // Destroy();
    animate = true;
  }
}
