#pragma once

#include "components/simple_scene.h"
#include "enemy.h"
#include "hud.h"
#include "lab_camera.h"
#include "obstacle.h"
#include "player.h"

namespace m1 {
class Tema2 : public gfxc::SimpleScene {
 public:
  Tema2();
  ~Tema2();

  void Init() override;

 private:
  void FrameStart() override;
  void Update(float deltaTimeSeconds) override;
  void FrameEnd() override;

  void OnInputUpdate(float deltaTime, int mods) override;
  void OnKeyPress(int key, int mods) override;
  void OnKeyRelease(int key, int mods) override;
  void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
  void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
  void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
  void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
  void OnWindowResize(int width, int height) override;

 protected:
  implemented::Camera* camera;
  glm::mat4 projectionMatrix;
  glm::mat4 orthoMatrix;

  float fovY, near, far;
  float left, right, bottom, top;

 private:
  void ReadMap();
  void GenerateMaze();
  void CheckCollisions();
  void DrawScene();

  Player player;
  std::list<Enemy> enemies;
  HUD hud;

  bool attackMode;
  float sensivityOX, sensivityOY;

  int mapSizeX, mapSizeY;
  int lastMap;
  int noMaps;
  std::vector<std::vector<int>> map;
  float startPosX, startPosY, endPosX, endPosY;

  int playerPosX, playerPosY;

  float cellScaleY = 3.f, cellScaleX = 3.f;
  float floorHeight = 0.1f, wallHeight = 10.f;
  float labY;
  float angleOX = 0;
  std::vector<Obstacle> maze;
};
}  // namespace m1
