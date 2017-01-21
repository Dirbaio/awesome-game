#ifndef RESOURCES_H
#define RESOURCES_H

#include <VBE/VBE.hpp>

extern mat4f projection;
extern MeshIndexed quadMesh;
extern ShaderProgram quadShader;
extern ShaderProgram groundShader;
extern Texture2D* awesome;

void loadAssets();
void drawQuad(Texture2D& tex, vec2f pos, float radius, float roto);

#endif // RESOURCES_H
