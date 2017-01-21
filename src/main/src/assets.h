#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>
#include <vector>
#include <VBE/VBE.hpp>

using namespace std;

extern mat4f projection;
extern MeshIndexed quadMesh;
extern ShaderProgram quadShader;
extern ShaderProgram groundShader;
extern std::vector<Texture2D*> faces;
extern std::vector<std::string> facePaths;

void loadAssets();
string findAssetPath();
void drawQuad(Texture2D& tex, vec2f pos, float radius, float roto);
int faceIndex(char c);

#endif // RESOURCES_H
