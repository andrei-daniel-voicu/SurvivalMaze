#include "object.h"

Object::Object(Mesh* mesh = nullptr, Shader* shader = nullptr)
    : Drawable(mesh, shader) {
  toDestroy = false;
}

Object::~Object() {}

void Object::Update(float) {}

void Object::Destroy() { toDestroy = true; }

bool Object::isDestroy() const { return toDestroy; }
