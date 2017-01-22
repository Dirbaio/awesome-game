#include "groundactor.h"
#include "assets.h"
#include "perlinnoise.h"

const int CHUNK_SIZE = 256;
const float CHUNK_RESOLUTION = 0.1f;
const float CHUNK_DEEP = 500;
const int GROUND_LEN = 32;


class GroundChunk {
public:
    vector<float> heights;
    Mesh mesh;
    int pos;
    b2Fixture* fixture;
    b2Body* groundBody;
    PerlinNoise* pn;

    float calcHeight(int x) {
        float wave = 0;
        wave += pn->noise(x*0.0033f, 0.5, 0.5)*28.f;
        //wave += sin(x*0.008);
        wave += x*0.0025; //it's actually uphill XD
        return wave;
    }

    GroundChunk(b2Body* _groundBody, int _pos, PerlinNoise* _pn)
        : pos(_pos)
        , groundBody(_groundBody)
        , pn(_pn)
    {

        heights.resize(CHUNK_SIZE+1);
        for(int i = 0 ; i < CHUNK_SIZE+1; i++) {
            int x = i + pos * CHUNK_SIZE;
            heights[i] = calcHeight(x);
        }

        vector<vec3f> data;
        for(int i = 0 ; i < CHUNK_SIZE; i++) {
            data.push_back(vec3f((i+0)*CHUNK_RESOLUTION, heights[i]-CHUNK_DEEP, 1));
            data.push_back(vec3f((i+1)*CHUNK_RESOLUTION, heights[i+1]-CHUNK_DEEP, 1));
            data.push_back(vec3f((i+1)*CHUNK_RESOLUTION, heights[i+1], 0));

            data.push_back(vec3f((i+0)*CHUNK_RESOLUTION, heights[i]-CHUNK_DEEP, 1));
            data.push_back(vec3f((i+0)*CHUNK_RESOLUTION, heights[i], 0));
            data.push_back(vec3f((i+1)*CHUNK_RESOLUTION, heights[i+1], 0));
        }

        Vertex::Format format({
                                  Vertex::Attribute("a_position", Vertex::Attribute::Float, 3)
                              });
        mesh = Mesh(format);
        mesh.setVertexData(&data[0], data.size());
        mesh.setPrimitiveType(Mesh::TRIANGLES);

        vector<b2Vec2> v;
        for(int i = 0 ; i < CHUNK_SIZE+1; i++) {
            v.push_back(b2Vec2((pos*CHUNK_SIZE+i)*CHUNK_RESOLUTION, heights[i]));
        }
        b2ChainShape chain;
        chain.CreateChain(&v[0], v.size());
        chain.SetPrevVertex(b2Vec2((pos*CHUNK_SIZE-1)*CHUNK_RESOLUTION, calcHeight(pos*CHUNK_SIZE-1)));
        chain.SetPrevVertex(b2Vec2(((pos+1)*CHUNK_SIZE+1)*CHUNK_RESOLUTION, calcHeight((pos+1)*CHUNK_SIZE+1)));

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &chain;
        fixtureDef.friction = 0.5;
        fixture = groundBody->CreateFixture(&fixtureDef);
    }

    ~GroundChunk() {
        groundBody->DestroyFixture(fixture);
    }
};

GroundActor::GroundActor(GameScene* scene)
    : Actor(scene)
    , pn(time(NULL))
{
    b2BodyDef def;
    def.type = b2_staticBody;
    def.position.Set(0.0f, 0.0f);
    setBody(scene->world.CreateBody(&def));

    chunks.resize(GROUND_LEN);
    load(0);
}

GroundActor::~GroundActor() {
    for (GroundChunk * chunk : chunks) {
        delete chunk;
    }
}

void GroundActor::load(int x) {
    x = int(x / CHUNK_RESOLUTION / CHUNK_SIZE) - GROUND_LEN/2;
    for(int i = 0; i < GROUND_LEN; i++) {
        int pos = i + x;
        int idx = ((pos % GROUND_LEN) + GROUND_LEN) % GROUND_LEN;
        if(!chunks[idx] || chunks[idx]->pos != pos) {
            if(chunks[idx])
                delete chunks[idx];
            chunks[idx] = new GroundChunk(this->body, pos, &pn);
        }
    }
}

void GroundActor::draw() {
    for(int i = 0; i < GROUND_LEN; i++) {
        groundShader.uniform("m")->set(glm::translate(mat4f(1.f), vec3f(chunks[i]->pos*CHUNK_SIZE*CHUNK_RESOLUTION, 0.0f, 0.0f)));
        groundShader.uniform("mvp")->set(glm::translate(projection, vec3f(chunks[i]->pos*CHUNK_SIZE*CHUNK_RESOLUTION, 0.0f, 0.0f)));
        groundShader.uniform("groundTexture")->set(groundTexture);
        groundShader.uniform("grassTexture")->set(grassTexture);
        chunks[i]->mesh.draw(groundShader);
    }
}

