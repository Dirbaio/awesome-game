#ifndef RESOURCES_H
#define RESOURCES_H

#include<string>
#include <VBE/VBE.hpp>

using namespace std;

extern mat4f projection;
extern MeshIndexed quadMesh;
extern ShaderProgram quadShader;
extern ShaderProgram groundShader;
extern Texture2D* awesome;

void loadAssets();
string findAssetPath();
void drawQuad(Texture2D& tex, vec2f pos, float radius, float roto);

#endif // RESOURCES_H
