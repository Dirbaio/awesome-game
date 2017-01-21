#include <VBE/VBE.hpp>
#include <VBE/graphics/Image.hpp>
#include <iostream>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <Box2D/Box2D.h>
#include "websocketinput.h"

using namespace std;

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


mat4f projection;
MeshIndexed quadMesh;
ShaderProgram quadShader;

void drawQuad(Texture2D& tex, vec2f pos, float radius, float roto) {
    quadShader.uniform("u_tex")->set(tex);
    mat4f lol = projection;
    lol = glm::translate(lol, vec3f(pos.x, pos.y, 0));
    lol = glm::scale(lol, vec3f(radius, radius, radius));
    lol = glm::rotate(lol, roto, vec3f(0, 0, 1));
    quadShader.uniform("mvp")->set(lol);
    quadMesh.draw(quadShader);
}

Mesh createCube() {
	// Create cube mesh. Example of non-indexed mesh, with multiple attributes.

	Vertex::Format format({
							  Vertex::Attribute("a_position", Vertex::Attribute::Float, 3),
							  Vertex::Attribute("a_normal", Vertex::Attribute::Float, 3),
							  Vertex::Attribute("a_texcoord", Vertex::Attribute::Float, 2)
						  });

	struct vtx {
			vec3f position;
			vec3f normal;
			vec2f texcoord;
	};

	vector<vtx> cubeVertices = {
		vtx{vec3f(-1.0,  1.0,  1.0), vec3f(-1.0, 0.0, 0.0), vec2f(1.0, 0.0)},
		vtx{vec3f(-1.0,  1.0, -1.0), vec3f(-1.0, 0.0, 0.0), vec2f(0.0, 0.0)},
		vtx{vec3f(-1.0, -1.0, -1.0), vec3f(-1.0, 0.0, 0.0), vec2f(0.0, 1.0)},
		vtx{vec3f(-1.0, -1.0, -1.0), vec3f(-1.0, 0.0, 0.0), vec2f(0.0, 1.0)},
		vtx{vec3f(-1.0, -1.0,  1.0), vec3f(-1.0, 0.0, 0.0), vec2f(1.0, 1.0)},
		vtx{vec3f(-1.0,  1.0,  1.0), vec3f(-1.0, 0.0, 0.0), vec2f(1.0, 0.0)},

		vtx{vec3f( 1.0, -1.0, -1.0), vec3f( 1.0, 0.0, 0.0), vec2f(1.0, 1.0)},
		vtx{vec3f( 1.0,  1.0, -1.0), vec3f( 1.0, 0.0, 0.0), vec2f(1.0, 0.0)},
		vtx{vec3f( 1.0,  1.0,  1.0), vec3f( 1.0, 0.0, 0.0), vec2f(0.0, 0.0)},
		vtx{vec3f( 1.0,  1.0,  1.0), vec3f( 1.0, 0.0, 0.0), vec2f(0.0, 0.0)},
		vtx{vec3f( 1.0, -1.0,  1.0), vec3f( 1.0, 0.0, 0.0), vec2f(0.0, 1.0)},
		vtx{vec3f( 1.0, -1.0, -1.0), vec3f( 1.0, 0.0, 0.0), vec2f(1.0, 1.0)},

		vtx{vec3f(-1.0, -1.0, -1.0), vec3f(0.0, 0.0, -1.0), vec2f(1.0, 1.0)},
		vtx{vec3f(-1.0,  1.0, -1.0), vec3f(0.0, 0.0, -1.0), vec2f(1.0, 0.0)},
		vtx{vec3f( 1.0,  1.0, -1.0), vec3f(0.0, 0.0, -1.0), vec2f(0.0, 0.0)},
		vtx{vec3f( 1.0,  1.0, -1.0), vec3f(0.0, 0.0, -1.0), vec2f(0.0, 0.0)},
		vtx{vec3f( 1.0, -1.0, -1.0), vec3f(0.0, 0.0, -1.0), vec2f(0.0, 1.0)},
		vtx{vec3f(-1.0, -1.0, -1.0), vec3f(0.0, 0.0, -1.0), vec2f(1.0, 1.0)},

		vtx{vec3f( 1.0,  1.0,  1.0), vec3f(0.0, 0.0,  1.0), vec2f(1.0, 0.0)},
		vtx{vec3f(-1.0,  1.0,  1.0), vec3f(0.0, 0.0,  1.0), vec2f(0.0, 0.0)},
		vtx{vec3f(-1.0, -1.0,  1.0), vec3f(0.0, 0.0,  1.0), vec2f(0.0, 1.0)},
		vtx{vec3f(-1.0, -1.0,  1.0), vec3f(0.0, 0.0,  1.0), vec2f(0.0, 1.0)},
		vtx{vec3f( 1.0, -1.0,  1.0), vec3f(0.0, 0.0,  1.0), vec2f(1.0, 1.0)},
		vtx{vec3f( 1.0,  1.0,  1.0), vec3f(0.0, 0.0,  1.0), vec2f(1.0, 0.0)},

		vtx{vec3f( 1.0, -1.0,  1.0), vec3f(0.0, -1.0, 0.0), vec2f(1.0, 0.0)},
		vtx{vec3f(-1.0, -1.0,  1.0), vec3f(0.0, -1.0, 0.0), vec2f(0.0, 0.0)},
		vtx{vec3f(-1.0, -1.0, -1.0), vec3f(0.0, -1.0, 0.0), vec2f(0.0, 1.0)},
		vtx{vec3f(-1.0, -1.0, -1.0), vec3f(0.0, -1.0, 0.0), vec2f(0.0, 1.0)},
		vtx{vec3f( 1.0, -1.0, -1.0), vec3f(0.0, -1.0, 0.0), vec2f(1.0, 1.0)},
		vtx{vec3f( 1.0, -1.0,  1.0), vec3f(0.0, -1.0, 0.0), vec2f(1.0, 0.0)},

		vtx{vec3f(-1.0,  1.0, -1.0), vec3f(0.0,  1.0, 0.0), vec2f(1.0, 1.0)},
		vtx{vec3f(-1.0,  1.0,  1.0), vec3f(0.0,  1.0, 0.0), vec2f(1.0, 0.0)},
		vtx{vec3f( 1.0,  1.0,  1.0), vec3f(0.0,  1.0, 0.0), vec2f(0.0, 0.0)},
		vtx{vec3f( 1.0,  1.0,  1.0), vec3f(0.0,  1.0, 0.0), vec2f(0.0, 0.0)},
		vtx{vec3f( 1.0,  1.0, -1.0), vec3f(0.0,  1.0, 0.0), vec2f(0.0, 1.0)},
		vtx{vec3f(-1.0,  1.0, -1.0), vec3f(0.0,  1.0, 0.0), vec2f(1.0, 1.0)},
	};

	Mesh cube(format);
	cube.setPrimitiveType(Mesh::TRIANGLES);
	cube.setVertexData(&cubeVertices[0], cubeVertices.size());

	return cube;
}

bool isDir(const char* path) {
	struct stat info;

	if(stat(path, &info) != 0 )
		return false;

	return (info.st_mode & S_IFDIR) != 0;
}

void findAssetPath() {
	if(isDir("assets/")) {
		Storage::setAssetPath("assets/");
		return;
	}
	if(isDir("../assets/")) {
		Storage::setAssetPath("../assets/");
		return;
	}
	if(isDir("../../assets/")) {
		Storage::setAssetPath("../../assets/");
		return;
	}
	if(isDir("../../../assets/")) {
		Storage::setAssetPath("../../../assets/");
		return;
	}
	if(isDir("../../../../assets/")) {
		Storage::setAssetPath("../../../../assets/");
		return;
	}

	VBE_ASSERT(false, "Can't find assets folder!");
}

b2World world(b2Vec2(0.0f, -50.0f));

const int CHUNK_SIZE = 256;
const float CHUNK_RESOLUTION = 0.1f;
const float CHUNK_DEEP = 100;

float calcHeight(int x) {
    float res = 0;
    res += sin(x*0.0025)*20;
    res += sin(x*0.01)*3;
    return res;
}

class GroundChunk {
public:
    vector<float> heights;
    Mesh mesh;
    int pos;
    b2Body* body;

    GroundChunk(int pos) {
        this->pos = pos;

        heights.resize(CHUNK_SIZE+1);
        for(int i = 0 ; i < CHUNK_SIZE+1; i++) {
            int x = i + pos * CHUNK_SIZE;
            heights[i] = calcHeight(x);
        }

        vector<vec2f> data;
        for(int i = 0 ; i < CHUNK_SIZE; i++) {
            data.push_back(vec2f((i+0)*CHUNK_RESOLUTION, heights[i]-CHUNK_DEEP));
            data.push_back(vec2f((i+1)*CHUNK_RESOLUTION, heights[i+1]-CHUNK_DEEP));
            data.push_back(vec2f((i+1)*CHUNK_RESOLUTION, heights[i+1]));

            data.push_back(vec2f((i+0)*CHUNK_RESOLUTION, heights[i]-CHUNK_DEEP));
            data.push_back(vec2f((i+0)*CHUNK_RESOLUTION, heights[i]));
            data.push_back(vec2f((i+1)*CHUNK_RESOLUTION, heights[i+1]));
        }

        Vertex::Format format({
            Vertex::Attribute("a_position", Vertex::Attribute::Float, 2)
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

        b2BodyDef def;
        def.type = b2_staticBody;
        def.position.Set(0.0f, 0.0f);
        body = world.CreateBody(&def);
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &chain;
        fixtureDef.friction = 0.5;
        body->CreateFixture(&fixtureDef);
    }

    ~GroundChunk() {
        world.DestroyBody(body);
    }
};

const int GROUND_LEN = 8;
ShaderProgram groundShader;

class Ground {
public:
    vector<GroundChunk*> chunks;

    Ground() {
        chunks.resize(GROUND_LEN);
        load(0);
    }

    void load(int x) {
        x = int(x / CHUNK_RESOLUTION / CHUNK_SIZE) - GROUND_LEN/2;
        for(int i = 0; i < GROUND_LEN; i++) {
            int pos = i + x;
            int idx = ((pos % GROUND_LEN) + GROUND_LEN) % GROUND_LEN;
            if(!chunks[idx] || chunks[idx]->pos != pos) {
                if(!chunks[idx])
                    delete chunks[idx];
                chunks[idx] = new GroundChunk(pos);
            }
        }
    }

    void draw() {
        for(int i = 0; i < GROUND_LEN; i++) {
            groundShader.uniform("mvp")->set(glm::translate(projection, vec3f(chunks[i]->pos*CHUNK_SIZE*CHUNK_RESOLUTION, 0.0f, 0.0f)));
            chunks[i]->mesh.draw(groundShader);
        }
    }
};

Texture2D* awesome;

class Player {
public:
    b2Body* body;

	Player(float x, float y) {

        b2CircleShape circle;
        circle.m_radius = 1.0f;

        b2BodyDef def;
        def.type = b2_dynamicBody;
		def.position.Set(x, y);
        body = world.CreateBody(&def);
        b2FixtureDef fixtureDef;
        fixtureDef.friction = 0.1;
        fixtureDef.shape = &circle;
        fixtureDef.density = 1;
        body->CreateFixture(&fixtureDef);
    }

    vec2f getPosition() {
        return vec2f(body->GetPosition().x, body->GetPosition().y);
    }

    void draw() {
        drawQuad(*awesome, getPosition(), 1.0f, body->GetAngle());
    }
};

const float DOWN_FORCE = 1000.0f;
const float UP_FORCE = 1.0f;
const float SIDES_FORCE = 5.0f;

int main() {
	findAssetPath();

	WebSocketInput input;

	// Create screen
	ContextSettings settings;
	settings.versionMajor = 3;
    settings.versionMinor = 3;
	Window window (Window::DisplayMode::createWindowedMode(800, 600), settings);

    groundShader = ShaderProgram(Storage::openAsset("ground.vert"), Storage::openAsset("ground.frag"));
    quadShader = ShaderProgram(Storage::openAsset("quad.vert"), Storage::openAsset("quad.frag"));
    quadMesh = createQuad();

    awesome = new Texture2D(Texture2D::load(Storage::openAsset("awesomeface.png")));

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Ground g;
	map<char, Player*> players;

	while(true) {
		window.update();

		if(Keyboard::pressed(Keyboard::Escape) || window.isClosing())
			break;

		vec3f center(0,0,0);
		if (players.size() != 0) {
			for (auto p : players) {
				auto pos = p.second->getPosition();
				center.x += pos.x;
				center.y += pos.y;
			}
			center /= players.size();
		}

		if(Keyboard::justPressed(Keyboard::Return)){
            if (players.find('z') == players.end()) {
				players['z'] = new Player(center.x, center.y+10.f);
			}
		}

		for (char c : input.connectedPlayers()){
			players[c] = new Player(center.x, center.y+10.f);
		}

		for (char c : input.disconnectedPlayers()){
			delete players[c];
			players.erase(c);
		}

		for (auto p : players) {
			WebSocketInput::PlayerState s = input.getPlayerState(p.first);
			if (s == WebSocketInput::DOWN) {
                p.second->body->ApplyForceToCenter(b2Vec2(0, -DOWN_FORCE), true);
			} else if (s == WebSocketInput::UP) {
                p.second->body->ApplyForceToCenter(b2Vec2(0, UP_FORCE), true);
			}
		}

		if(Keyboard::pressed(Keyboard::Q))
			window.setDisplayMode(Window::getFullscreenModes()[0]);
		if(Keyboard::pressed(Keyboard::W))
			window.setDisplayMode(Window::DisplayMode::createWindowedMode(800, 600));
		if(Keyboard::pressed(Keyboard::E))
			window.setDisplayMode(Window::getFullscreenModes()[12]);

        if(Keyboard::pressed(Keyboard::Right))
            if (players.find('z') != players.end()) players['z']->body->ApplyForceToCenter(b2Vec2(SIDES_FORCE, 0), true);
        if(Keyboard::pressed(Keyboard::Left))
            if (players.find('z') != players.end()) players['z']->body->ApplyForceToCenter(b2Vec2(SIDES_FORCE, 0), true);
        if(Keyboard::pressed(Keyboard::UP))
            if (players.find('z') != players.end()) players['z']->body->ApplyForceToCenter(b2Vec2(0, UP_FORCE), true);
        if(Keyboard::pressed(Keyboard::Down))
            if (players.find('z') != players.end()) players['z']->body->ApplyForceToCenter(b2Vec2(0, -DOWN_FORCE), true);

        float32 timeStep = 1.0f / 60.f;
        int32 velocityIterations = 10;
        int32 positionIterations = 8;
        world.Step(timeStep, velocityIterations, positionIterations);

		// Set viewport
		glViewport(0, 0, window.getSize().x, window.getSize().y);

        glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Projection matrix.
		float aspect = float(window.getSize().x)/window.getSize().y;
        float zoom = 20.0f;
        projection = glm::ortho(-zoom*aspect, zoom*aspect, -zoom, zoom);
		projection = glm::translate(projection, -center);

		g.load(center.x);
        g.draw();

		for (auto p : players) {
			p.second->draw();
		}

		window.swapBuffers();
	}
}
