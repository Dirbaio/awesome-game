#include "assets.h"
#include <sys/types.h>
#include <sys/stat.h>
#include<vector>
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
using namespace std;

mat3f projection;
MeshIndexed quadMesh;
ShaderProgram quadShader;
ShaderProgram groundShader;
std::vector<Texture2D*> faces;
std::vector<std::string> facePaths = {
    {"A.png"},
    {"B.png"},
    {"C.png"},
    {"D.png"},
    {"E.png"},
    {"F.png"},
    {"G.png"},
    {"H.png"},
    {"I.png"},
    {"J.png"}
};
Texture2D* groundTexture;
Texture2D* grassTexture;
Texture2D* bgTexture;

bool isDir(const char* path) {
    struct stat info;

    if(stat(path, &info) != 0 )
        return false;

    return (info.st_mode & S_IFDIR) != 0;
}


string findAssetPath() {
    if(isDir("assets/"))
        return "assets/";
    if(isDir("../assets/"))
        return "../assets/";
    if(isDir("../../assets/"))
        return "../../assets/";
    if(isDir("../../../assets/"))
        return "../../../assets/";
    if(isDir("../../../../assets/"))
        return "../../../../assets/";

    VBE_ASSERT(false, "Can't find assets folder!");
    return "";
}

MeshIndexed createQuad() {
    // Create the quad mesh. Example of indexed mesh.

    Vertex::Format format({
                              Vertex::Attribute("a_position", Vertex::Attribute::Float, 2)
                          });

    vector<vec2f> data = {
        vec2f(1, -1),
        vec2f(1, 1),
        vec2f(-1, 1),
        vec2f(-1, -1)
    };

    vector<unsigned int> indices = {
        0, 1, 2, 3, 0, 2
    };

    MeshIndexed quad(format);
    quad.setVertexData(&data[0], data.size());
    quad.setIndexData(&indices[0], indices.size());
    quad.setPrimitiveType(Mesh::TRIANGLES);

    return quad;
}

void drawQuad(Texture2D& tex, vec2f pos, float radius, float roto) {
    quadShader.uniform("u_tex")->set(tex);
    mat3f lol = projection;
    lol = glm::translate(lol, pos);
    lol = glm::scale(lol, vec2f(radius));
    lol = glm::rotate(lol, roto);
    quadShader.uniform("mvp")->set(lol);
    quadMesh.draw(quadShader);
}

void loadAssets() {
    Storage::setAssetPath(findAssetPath());

    groundShader = ShaderProgram(Storage::openAsset("ground.vert"), Storage::openAsset("ground.frag"));
    quadShader = ShaderProgram(Storage::openAsset("quad.vert"), Storage::openAsset("quad.frag"));
    quadMesh = createQuad();
    groundTexture = new Texture2D(Texture2D::load(Storage::openAsset("ground.png")));
    groundTexture->setWrap(GL_REPEAT);
    grassTexture = new Texture2D(Texture2D::load(Storage::openAsset("grass.png")));
    grassTexture->setWrap(GL_REPEAT);
    bgTexture = new Texture2D(Texture2D::load(Storage::openAsset("bg.png")));
    bgTexture->setWrap(GL_REPEAT);

    shuffle(facePaths.begin(), facePaths.end(), std::default_random_engine(time(NULL)));
    for (const string& s : facePaths) {
        faces.push_back(new Texture2D(Texture2D::load(Storage::openAsset(s))));
    }
}

int faceIndex(char c) {
    return c%facePaths.size();
}
