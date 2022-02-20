#include "lab_m1/tema2/tema2.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace m1;
using namespace std;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Tema2::Tema2() {}

Tema2::~Tema2() { delete camera; }

void Tema2::Init() {
  camera = new implemented::Camera();
  {
    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(
        PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"),
        "box.obj");
    AddMeshToList(mesh);
  }
  {
    Mesh* mesh = new Mesh("sphere");
    mesh->LoadMesh(
        PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"),
        "sphere.obj");
    AddMeshToList(mesh);
  }
  {
    Mesh* mesh = new Mesh("rabbit");
    mesh->LoadMesh(
        PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "animals"),
        "bunny.obj");
    AddMeshToList(mesh);
  }
  {
    Shader* shader = new Shader("ColorShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2",
                                "shaders", "ColorVertexShader.glsl"),
                      GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2",
                                "shaders", "ColorFragmentShader.glsl"),
                      GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
  }
  {
    Shader* shader = new Shader("DisplacementShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2",
                                "shaders", "DisplacementVertexShader.glsl"),
                      GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2",
                                "shaders", "DisplacementFragmentShader.glsl"),
                      GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
  }
  ReadMap();
  GenerateMaze();

  fovY = RADIANS(60);
  near = 0.01f;
  far = 200.0f;
  left = -window->GetResolution().x / 400;
  right = window->GetResolution().x / 400;
  bottom = -window->GetResolution().y / 400;
  top = window->GetResolution().y / 400;
  projectionMatrix =
      glm::perspective(fovY, window->props.aspectRatio, near, far);
  orthoMatrix = glm::ortho(left, right, bottom, top, near, far);

  {
    player.mesh = meshes["box"];
    player.shader = shaders["ColorShader"];

    player.bulletSpeed = 15.f;
    player.travelDistance = 10.f;
    player.fireRate = 0.5f;
    player.bulletMesh = meshes["sphere"];
    player.bulletShader = shaders["ColorShader"];

    player.speed = 5.f;
    player.hp = 10;
    player.initialHp = player.hp;

    player.forward = &camera->forward;
    player.cameraPos = &camera->position;

    player.setPosition(
        glm::vec3(startPosX * cellScaleX, 0, startPosY * cellScaleY));
  }
  {
    hud.mesh = meshes["box"];
    hud.shader = shaders["ColorShader"];
    hud.player = &player;

    hud.setScale(glm::vec3{1.2, 0.15f, 0});
    hud.setPosition(player.getPosition());
    hud.initialHpScale = hud.getScale().x;
    hud.initialTimeScale = hud.getScale().x;
    hud.initialTime = 50.f;
    hud.timeLeft = hud.initialTime;
  }
  {
    window->SetSize(1500, 800);
    window->CenterWindow();
    window->HidePointer();
    glm::vec3 cameraPos = camera->position - camera->GetTargetPosition() +
                          player.getPosition() + glm::vec3(0, 2.f, 0);
    camera->distanceToTarget = 2.5f;
    camera->Set(cameraPos, player.getPosition(), glm::vec3_up);
    sensivityOX = 0.001f;
    sensivityOY = 0.001f;

    attackMode = false;
  }
}

void Tema2::FrameStart() {
  // Clears the color buffer (using the previously set color) and depth buffer
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::ivec2 resolution = window->GetResolution();
  // Sets the screen area where to draw
  glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds) {
  if (player.hp <= 0 || hud.timeLeft <= 0) {
    window->Close();
    std::cout << "\nGame Over!\n";
  }

  glm::vec3 playerPos = player.getPosition();
  if (playerPos.x >= endPosX * cellScaleX - cellScaleX / 2 &&
      playerPos.x <= endPosX * cellScaleX + cellScaleX / 2 &&
      playerPos.z >= endPosY * cellScaleY - cellScaleY / 2 &&
      playerPos.z <= endPosY * cellScaleY + cellScaleY / 2) {
    window->Close();
    std::cout << "\nCONGRATULATIONS! YOU FOUND THE EXIT!\n";
  }

  // call updates
  player.Update(deltaTimeSeconds);
  std::for_each(enemies.begin(), enemies.end(),
                [&](Enemy& enemy) { enemy.Update(deltaTimeSeconds); });
  hud.Update(deltaTimeSeconds);

  // check collisions
  CheckCollisions();

  // remove entities
  enemies.remove_if(
      [](const Enemy& enemy) { return enemy.isDestroy() == true; });

  // draw scene
  DrawScene();
}

void Tema2::FrameEnd() {}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema2::OnInputUpdate(float deltaTime, int mods) {
  if (deltaTime == INFINITY) return;

  if (attackMode) return;
  glm::vec3 dir{0, 0, 0};
  glm::vec3 move{0, 0, 0};
  glm::vec3 newMove{0, 0, 0};

  if (window->KeyHold(GLFW_KEY_W) || window->KeyHold(GLFW_KEY_UP)) {
    glm::vec3 dir =
        glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z));
    move = dir * deltaTime * player.speed;
    newMove = player.tryMove(maze, move);
    if (newMove != move) return;
    player.setPosition(player.getPosition() + newMove);
    camera->MoveForward(deltaTime * player.speed);
  }

  if (window->KeyHold(GLFW_KEY_A) || window->KeyHold(GLFW_KEY_LEFT)) {
    glm::vec3 dir = -glm::normalize(camera->right);
    move = dir * deltaTime * player.speed;
    newMove = player.tryMove(maze, move);
    if (newMove != move) return;
    player.setPosition(player.getPosition() + newMove);
    camera->TranslateRight(-deltaTime * player.speed);
  }

  if (window->KeyHold(GLFW_KEY_S) || window->KeyHold(GLFW_KEY_DOWN)) {
    glm::vec3 dir =
        -glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z));
    move = dir * deltaTime * player.speed;
    newMove = player.tryMove(maze, move);
    if (newMove != move) return;
    player.setPosition(player.getPosition() + newMove);
    camera->MoveForward(-deltaTime * player.speed);
  }

  if (window->KeyHold(GLFW_KEY_D) || window->KeyHold(GLFW_KEY_RIGHT)) {
    glm::vec3 dir = glm::normalize(camera->right);
    move = dir * deltaTime * player.speed;
    newMove = player.tryMove(maze, move);
    if (newMove != move) return;
    player.setPosition(player.getPosition() + newMove);
    camera->TranslateRight(deltaTime * player.speed);
  }
}

void Tema2::OnKeyPress(int key, int mods) {
  if (key == GLFW_KEY_LEFT_CONTROL) {
    if (attackMode) {
      auto toGo =
          camera->position - camera->GetTargetPosition() + player.getPosition();
      camera->Set(toGo, player.getPosition(), glm::vec3_up);
    }
    attackMode = !attackMode;
  }
  if (attackMode && key == GLFW_KEY_SPACE) {
    player.SpawnBullet();
  }
}

void Tema2::OnKeyRelease(int key, int mods) {
  // Add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
  if (!attackMode) {
    if ((angleOX >= -RADIANS(60) && angleOX <= RADIANS(60)) ||
        (angleOX >= RADIANS(60) && deltaY >= 0) ||
        (angleOX <= -RADIANS(-60) && deltaY <= 0)) {
      camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
      angleOX += -sensivityOX * deltaY;
    }

    glm::vec3 rot = player.getRotation();
    rot.y += -sensivityOY * deltaX;
    player.setRotation(rot);
    camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
  } else {
    camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
    glm::vec3 rot = player.getRotation();
    rot.y += -sensivityOY * deltaX;
    player.setRotation(rot);
    camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
  }
  window->CenterPointer();
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Tema2::OnWindowResize(int width, int height) {}

void Tema2::ReadMap() {
  std::string relativePath = "./src/lab_m1/tema2/labyrinths/";
  std::cout << relativePath + "labyrinths.txt";
  std::ifstream in(relativePath + "labyrinths.txt");

  in >> noMaps >> mapSizeY >> mapSizeX >> lastMap;
  in.close();
  map = std::vector<std::vector<int>>(mapSizeY, std::vector<int>(mapSizeX));
  srand(time(0));
  int noMap = -1;

  do {
    noMap = rand() % noMaps;
  } while (noMap == lastMap);
  std::ofstream out(relativePath + "labyrinths.txt");
  out << noMaps << " " << mapSizeX << " " << mapSizeY << " " << noMap;
  out.close();

  std::ifstream fin(relativePath + "labyrinth" + std::to_string(noMap) +
                    ".txt");
  fin >> startPosY >> startPosX >> endPosY >> endPosX;

  for (int i = 0; i < mapSizeY; i++)
    for (int j = 0; j < mapSizeX; j++) fin >> map[i][j];
  fin.close();
}

void Tema2::GenerateMaze() {
  labY = -player.bodyScale.y / 2 - 3 * player.bodyScale.y / 4;
  for (int i = 0; i < mapSizeY; i++) {
    for (int j = 0; j < mapSizeX; j++) {
      switch (map[i][j]) {
        case 0: {
          maze.push_back(Obstacle(
              0,
              glm::vec3(cellScaleX * j, labY + floorHeight / 2, cellScaleY * i),
              glm::vec3(0, 0, 0),
              glm::vec3(cellScaleX, floorHeight, cellScaleY), meshes["box"],
              shaders["ColorShader"]));
          break;
        }
        case 1: {
          maze.push_back(Obstacle(
              1,
              glm::vec3(cellScaleX * j, labY + wallHeight / 2, cellScaleY * i),
              glm::vec3(0, 0, 0), glm::vec3(cellScaleX, wallHeight, cellScaleY),
              meshes["box"], shaders["ColorShader"]));
          break;
        }
        case 2: {
          maze.push_back(Obstacle(
              0,
              glm::vec3(cellScaleX * j, labY + floorHeight / 2, cellScaleY * i),
              glm::vec3(0, 0, 0),
              glm::vec3(cellScaleX, floorHeight, cellScaleY), meshes["box"],
              shaders["ColorShader"]));

          Enemy enemy(glm::vec2{cellScaleX, cellScaleY}, 3.f,
                      glm::vec3(cellScaleX * j, labY, cellScaleY * i),
                      meshes["rabbit"], shaders["DisplacementShader"]);
          enemies.push_back(enemy);
          break;
        }
        default:
          break;
      }
    }
  }
}

void m1::Tema2::CheckCollisions() {
  // player bullet collisions
  for (Bullet& bullet : player.bullets) {
    glm::vec3 bulletPos = bullet.getColPosition();
    glm::vec3 bulletScale = bullet.getColScale();

    // bullet - enemies
    bool ok = false;
    for (Enemy& enemy : enemies) {
      if (enemy.animate) continue;
      if (bullet.CheckCollision(bulletPos, bulletScale, enemy.getColPosition(),
                                enemy.getColScale())) {
        enemy.OnCollisionEnter(bullet);
        bullet.OnCollisionEnter(enemy);
        ok = true;
        break;
      }
    }

    if (ok) continue;

    // bullet - obstacles
    for (Obstacle& obs : maze) {
      if (bullet.CheckCollision(obs.getColPosition(), obs.getColScale(),
                                bulletPos, bulletScale)) {
        obs.OnCollisionEnter(bullet);
        bullet.OnCollisionEnter(obs);
        break;
      }
    }
  }

  // enemy collision
  for (Enemy& enemy : enemies) {
    // enemy - player collision
    if (enemy.CheckCollision(player.getColPosition(), player.getColScale(),
                             enemy.getColPosition(), enemy.getColScale())) {
      enemy.OnCollisionEnter(player);
      player.OnCollisionEnter(enemy);
      break;
    }
  }
}

void Tema2::DrawScene() {
  // draw player if 3rd person camera
  if (!attackMode) player.Draw(camera->GetViewMatrix(), projectionMatrix);

  // draw bullets
  std::for_each(player.bullets.begin(), player.bullets.end(),
                [&](Bullet& bullet) {
                  bullet.Draw(camera->GetViewMatrix(), projectionMatrix);
                });

  // draw maze
  std::for_each(maze.begin(), maze.end(), [&](Obstacle& obstacle) {
    obstacle.Draw(camera->GetViewMatrix(), projectionMatrix);
  });

  // draw enemies
  std::for_each(enemies.begin(), enemies.end(), [&](Enemy& enemy) {
    enemy.Draw(camera->GetViewMatrix(), projectionMatrix);
  });

  // draw ui
  if (!attackMode) hud.Draw(camera->GetViewMatrix(), orthoMatrix);
}
