#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>
#include <vector>
#include <VBE/VBE.hpp>
#include <VBE/dependencies/glm/gtx/matrix_transform_2d.hpp>

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
extern Texture2D* winnerTexture;

void loadAssets();
string findAssetPath();
void drawQuad(Texture2D& tex, vec2f pos, float radius, float roto);
int faceIndex(char c);

#endif // RESOURCES_H
