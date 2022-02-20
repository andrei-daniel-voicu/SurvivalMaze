#pragma once
#include "player.h"

class HUD : public Object {
 public:
  HUD(Mesh* mesh = nullptr, Shader* shader = nullptr)
      : Object(glm::vec3{0, 0, 0}, glm::vec3{0, 0, 0}, glm::vec3{1, 1, 1}, mesh,
               shader){};
  void Update(float) override;
  void Draw(const glm::mat4&, const glm::mat4&) override;

  float initialHpScale;
  float initialTimeScale;
  float timeLeft;
  float initialTime;
  Player* player;
};