#include "SkyBox.h"

#include <vector>

using namespace mutiny::engine;

ref<SkyBox> SkyBox::create(ref<GameObject> playerGo)
{
  ref<GameObject> skyboxGo = GameObject::create("SkyBox");
  ref<SkyBox> skybox = skyboxGo->addComponent<SkyBox>();
  skybox->playerGo = playerGo;

  return skybox;
}

void SkyBox::onAwake()
{
  Debug::log("SkyBox awoken");

  std::vector<Vector3> vertices;
  vertices.push_back(Vector3(-100, -100, 50));
  vertices.push_back(Vector3(-100, 100, 50));
  vertices.push_back(Vector3(100, 100, 50));
  vertices.push_back(Vector3(100, 100, 50));
  vertices.push_back(Vector3(100, -100, 50));
  vertices.push_back(Vector3(-100, -100, 50));

  std::vector<Vector2> uv;
  uv.push_back(Vector2(0, 1));
  uv.push_back(Vector2(0, 0));
  uv.push_back(Vector2(1, 0));
  uv.push_back(Vector2(1, 0));
  uv.push_back(Vector2(1, 1));
  uv.push_back(Vector2(0, 1));

  std::vector<int> triangles;
  triangles.push_back(0);
  triangles.push_back(1);
  triangles.push_back(2);
  triangles.push_back(3);
  triangles.push_back(4);
  triangles.push_back(5);

  ref<Mesh> mesh = new Mesh();
  mesh->setVertices(vertices);
  mesh->setUv(uv);
  mesh->setTriangles(triangles, 0);

  ref<MeshFilter> meshFilter = getGameObject()->addComponent<MeshFilter>();
  meshFilter->setMesh(mesh);

  ref<Texture2d> texture = Resources::load<Texture2d>("textures/clouds");

  //Texture2d* texture = new Texture2d(1, 1);
  //texture->setPixel(0, 0, Color(1, 0, 0));
  //texture->apply();

  material = Material::create(Resources::load<Shader>("shaders/Internal-SkyboxTexture"));
  material->setMainTexture(texture);

  ref<MeshRenderer> meshRenderer = getGameObject()->addComponent<MeshRenderer>();
  meshRenderer->setMaterial(material.get());

  //getGameObject()->getTransform()->translate(Vector3(0, 0, 10));
  //getGameObject()->getTransform()->rotate(Vector3(0, 180, 0));
}

void SkyBox::onUpdate()
{
  //getGameObject()->getTransform()->rotate(Vector3(0, 100, 0) * Time::getDeltaTime());
  //getGameObject()->getTransform()->translate(Vector3(0, Time::getDeltaTime(), 0));
  Vector3 pos = getGameObject()->getTransform()->getPosition();
  //pos.x = playerGo->getTransform()->getPosition().x;
  getGameObject()->getTransform()->setPosition(pos);
}
