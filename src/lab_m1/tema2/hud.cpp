#include "hud.h"

void HUD::Update(float deltaTime) {
  setScale(glm::vec3{player->hp * initialHpScale / player->initialHp,
                     getScale().y, getScale().z});
  timeLeft -= deltaTime;
}

void HUD::Draw(const glm::mat4& view, const glm::mat4& proj) {
  glm::mat4 modelMatrix = glm::mat4(1);
  modelMatrix = glm::translate(modelMatrix, player->getPosition());
  modelMatrix =
      glm::rotate(modelMatrix, player->getRotation().y, glm::vec3{0, 1, 0});
  modelMatrix = glm::translate(
      modelMatrix,
      glm::vec3{0, player->bodyScale.y / 2 + player->headScale.y / 2, 0});

  modelMatrix = glm::translate(modelMatrix, glm::vec3{-1.f, 0, 0});
  modelMatrix = glm::translate(
      modelMatrix, glm::vec3{(initialHpScale - getScale().x) / 2, 0, 0});
  modelMatrix = glm::scale(modelMatrix, getScale());
  RenderMesh(modelMatrix, view, proj,
             glm::vec3(203 / 255.f, 3 / 255.f, 3 / 255.f));
  modelMatrix =
      glm::scale(modelMatrix, glm::vec3{1 / getScale().x, 1 / getScale().y, 0});
  modelMatrix = glm::translate(
      modelMatrix, glm::vec3{-(initialHpScale - getScale().x) / 2, 0, 0});
  modelMatrix = glm::scale(modelMatrix, glm::vec3{1.2, 0.15f, 0});
  mesh->SetDrawMode(GL_LINES);
  RenderMesh(modelMatrix, view, proj,
             glm::vec3(203 / 255.f, 3 / 255.f, 3 / 255.f));
  mesh->SetDrawMode(GL_TRIANGLES);

  modelMatrix = glm::mat4(1);
  modelMatrix = glm::translate(modelMatrix, player->getPosition());
  modelMatrix =
      glm::rotate(modelMatrix, player->getRotation().y, glm::vec3{0, 1, 0});
  modelMatrix = glm::translate(
      modelMatrix,
      glm::vec3{0, player->bodyScale.y / 2 + player->headScale.y / 2, 0});

  modelMatrix = glm::translate(modelMatrix, glm::vec3{-1.f, -getScale().y, 0});
  modelMatrix = glm::translate(
      modelMatrix,
      glm::vec3{
          (initialTimeScale - timeLeft * initialTimeScale / initialTime) / 2, 0,
          0});
  modelMatrix = glm::scale(modelMatrix,
                           glm::vec3{timeLeft * initialTimeScale / initialTime,
                                     getScale().y, getScale().z});
  RenderMesh(modelMatrix, view, proj, glm::vec3(0.824f, 0.412f, 0.118f));
  modelMatrix = glm::scale(
      modelMatrix, glm::vec3{1.f / (timeLeft * initialTimeScale / initialTime),
                             1 / getScale().y, 0});
  modelMatrix = glm::translate(
      modelMatrix,
      glm::vec3{
          -(initialTimeScale - timeLeft * initialTimeScale / initialTime) / 2,
          0, 0});
  modelMatrix = glm::scale(modelMatrix, glm::vec3{1.2, 0.15f, 0});
  mesh->SetDrawMode(GL_LINES);
  RenderMesh(modelMatrix, view, proj, glm::vec3(0.824f, 0.412f, 0.118f));
  mesh->SetDrawMode(GL_TRIANGLES);
}
