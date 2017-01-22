#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>
#include <vector>
#include <VBE/VBE.hpp>

using namespace std;

extern mat3f projection;
extern MeshIndexed quadMesh;
extern ShaderProgram quadShader;
extern ShaderProgram groundShader;
extern std::vector<Texture2D*> faces;
extern std::vector<std::string> facePaths;
extern Texture2D* groundTexture;
extern Texture2D* grassTexture;
extern Texture2D* bgTexture;

void loadAssets();
string findAssetPath();
void drawQuad(Texture2D& tex, vec2f pos, float radius, float roto);
int faceIndex(char c);

#endif // RESOURCES_H
