#pragma once
#include "bullet.h"
#include "object.h"
#include "obstacle.h"

class Player : public Object, public Collider {
 public:
  Player(Mesh* mesh = nullptr, Shader* shader = nullptr)
      : Object(glm::vec3{0, 0, 0}, glm::vec3{0, 0, 0}, glm::vec3{1, 1, 1}, mesh,
               shader) {
    timeSinceFired = 0;
    setScale(glm::vec3{1, 1, 1});
  }
  ~Player();
  void Update(float) override;
  void Draw(const glm::mat4&, const glm::mat4&) override;
  void setPosition(const glm::vec3&) override;
  void setScale(const glm::vec3&) override;
  void OnCollisionEnter(const Collider&) override;

  glm::vec3 tryMove(const std::vector<Obstacle>&, const glm::vec3&);

  void SpawnBullet();

  float speed;
  int hp;
  int initialHp;
  float bulletSpeed;
  float travelDistance;
  float fireRate;
  Mesh* bulletMesh;
  Shader* bulletShader;
  glm::vec3* forward;
  glm::vec3* cameraPos;
  std::list<Bullet> bullets;

  glm::vec3 bodyScale = glm::vec3{0.6f, 0.9f, 0.4f};
  glm::vec3 headScale = glm::vec3{0.35f, 0.35f, 0.35f};
  glm::vec3 armScale = glm::vec3{bodyScale.x / 2, bodyScale.y / 2, bodyScale.z};
  glm::vec3 legScale =
      glm::vec3{bodyScale.x / 2 - 0.1f, 3 * bodyScale.y / 4, bodyScale.z};
  glm::vec3 handScale = glm::vec3{armScale.x, armScale.x, armScale.z};

 private:
  float timeSinceFired = fireRate;
  float invincibility = 0.06f;
  float passed = invincibility;
};