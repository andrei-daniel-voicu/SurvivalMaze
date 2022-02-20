#include "player.h"

#include "enemy.h"

Player::~Player() {}

void Player::Update(float deltaTime) {
  if (passed > 0) passed -= deltaTime;
  bullets.remove_if(
      [](const Bullet& bullet) { return bullet.isDestroy() == true; });

  for (Bullet& bullet : bullets) {
    bullet.Update(deltaTime);
    if (bullet.distanceTraveled >= travelDistance) {
      bullet.Destroy();
    }
  }
  timeSinceFired += deltaTime;
}

void Player::Draw(const glm::mat4& view, const glm::mat4& proj) {
  glm::vec3 bodyPos = getPosition();

  glm::mat4 bodyMatrix;
  glm::mat4 modelMatrix = glm::mat4(1);

  // body
  {
    modelMatrix = glm::translate(modelMatrix, bodyPos);
    modelMatrix = glm::rotate(modelMatrix, getRotation().y, glm::vec3{0, 1, 0});
    bodyMatrix = modelMatrix;
    modelMatrix = glm::scale(modelMatrix, bodyScale);
    RenderMesh(modelMatrix, view, proj,
               glm::vec3{72 / 255.f, 118 / 255.f, 117 / 255.f});
  }
  // left arm
  {
    modelMatrix = glm::translate(
        bodyMatrix, glm::vec3{-bodyScale.x / 2 - armScale.x / 2,
                              bodyScale.y / 2 - armScale.y / 2, 0});
    modelMatrix = glm::scale(modelMatrix, armScale);
    RenderMesh(modelMatrix, view, proj,
               glm::vec3{72 / 255.f, 118 / 255.f, 117 / 255.f});
  }
  // right arm
  {
    modelMatrix = glm::translate(
        bodyMatrix, glm::vec3{bodyScale.x / 2 + armScale.x / 2,
                              bodyScale.y / 2 - armScale.y / 2, 0});
    modelMatrix = glm::scale(modelMatrix, armScale);
    RenderMesh(modelMatrix, view, proj,
               glm::vec3{72 / 255.f, 118 / 255.f, 117 / 255.f});
  }
  // left hand
  {
    modelMatrix = glm::translate(
        bodyMatrix, glm::vec3{-bodyScale.x / 2 - armScale.x / 2,
                              bodyScale.y / 2 - armScale.y / 2, 0});
    modelMatrix = glm::translate(
        modelMatrix, glm::vec3{0, -armScale.y / 2 - handScale.y / 2, 0});
    modelMatrix = glm::scale(modelMatrix, handScale);
    RenderMesh(modelMatrix, view, proj,
               glm::vec3{224 / 255.f, 207 / 255.f, 182 / 255.f});
  }
  // right hand
  {
    modelMatrix = glm::translate(
        bodyMatrix, glm::vec3{bodyScale.x / 2 + armScale.x / 2,
                              bodyScale.y / 2 - armScale.y / 2, 0});
    modelMatrix = glm::translate(
        modelMatrix, glm::vec3{0, -armScale.y / 2 - handScale.y / 2, 0});
    modelMatrix = glm::scale(modelMatrix, handScale);
    RenderMesh(modelMatrix, view, proj,
               glm::vec3{224 / 255.f, 207 / 255.f, 182 / 255.f});
  }
  // head
  {
    modelMatrix = glm::translate(
        bodyMatrix, glm::vec3{0, bodyScale.y / 2 + headScale.y / 2, 0});
    modelMatrix = glm::scale(modelMatrix, headScale);
    RenderMesh(modelMatrix, view, proj,
               glm::vec3{224 / 255.f, 207 / 255.f, 182 / 255.f});
  }
  // left leg
  {
    modelMatrix = glm::translate(
        bodyMatrix, glm::vec3{-bodyScale.x / 2 + legScale.x / 2,
                              -bodyScale.y / 2 - legScale.y / 2, 0});
    modelMatrix = glm::scale(modelMatrix, legScale);
    RenderMesh(modelMatrix, view, proj,
               glm::vec3{56 / 255.f, 65 / 255.f, 149 / 255.f});
  }
  // right leg
  {
    modelMatrix = glm::translate(
        bodyMatrix, glm::vec3{bodyScale.x / 2 - legScale.x / 2,
                              -bodyScale.y / 2 - legScale.y / 2, 0});
    modelMatrix = glm::scale(modelMatrix, legScale);
    RenderMesh(modelMatrix, view, proj,
               glm::vec3{56 / 255.f, 65 / 255.f, 149 / 255.f});
  }
}

void Player::SpawnBullet() {
  if (timeSinceFired >= fireRate) {
    timeSinceFired = 0;
    glm::vec3 offset = *cameraPos + *forward;
    bullets.push_back(
        Bullet(offset, *forward, bulletSpeed, bulletMesh, bulletShader));
  }
}

void Player::setPosition(const glm::vec3& pos) {
  Transform::setPosition(pos);
  setColPosition(pos);
}

void Player::setScale(const glm::vec3& sc) {
  Transform::setScale(sc);
  setColScale(glm::vec3{bodyScale.x + armScale.x * 2 + 0.1f,
                        bodyScale.y + legScale.y, bodyScale.z + 1.f});
}

void Player::OnCollisionEnter(const Collider& col) {
  if (dynamic_cast<const Enemy*>(&col) != nullptr) {
    if (passed <= 0) {
      hp--;
      passed = invincibility;
    }
  }
}

glm::vec3 Player::tryMove(const std::vector<Obstacle>& obstacles,
                          const glm::vec3& dir) {
  glm::vec3 newPos = getColPosition() + dir;
  glm::vec3 newDir = dir;
  glm::vec3 colScale = getColScale();

  for (const Obstacle& obs : obstacles) {
    if (CheckCollision(obs.getColPosition(), obs.getColScale(), newPos,
                       colScale)) {
      glm::vec3 obsPos = obs.getColPosition();
      glm::vec3 obsScale = obs.getColScale();
      if (newPos.x - colScale.x / 2 <= obsPos.x - obsScale.x / 2) {
        newPos.x -= dir.x;
        newDir.x = 0;
      }
      if (newPos.x + colScale.x / 2 >= obsPos.x + obsScale.x / 2) {
        newPos.x -= dir.x;
        newDir.x = 0;
      }
      if (newPos.y + colScale.y / 2 >= obsPos.y + obsScale.y / 2) {
        newPos.y -= dir.y;
        newDir.y = 0;
      }
      if (newPos.y - colScale.y / 2 <= obsPos.y + obsScale.y / 2) {
        newPos.y -= dir.y;
        newDir.y = 0;
      }
      if (newPos.z + colScale.z / 2 >= obsPos.z + obsScale.z / 2) {
        newPos.z -= dir.z;
        newDir.z = 0;
      }
      if (newPos.z - colScale.z / 2 <= obsPos.z + obsScale.z / 2) {
        newPos.z -= dir.z;
        newDir.z = 0;
      }
    }
  }
  return newDir;
}
