#include <VBE/VBE.hpp>
#include <VBE/graphics/Image.hpp>
#include <iostream>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <Box2D/Box2D.h>

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


b2World world(b2Vec2(0.0f, -10.0f));

const int CHUNK_SIZE = 256;
const float CHUNK_RESOLUTION = 0.1f;
const float CHUNK_DEEP = 100;

float calcHeight(int x) {
    float res = 0;
    res += sin(x*0.04);
    res += sin(x*0.03732);
    res += sin(x*0.083)*0.4;
    return res;
}

class GroundChunk {
public:
    vector<float> heights;
    Mesh mesh;
    int pos;

    GroundChunk(int pos) {
        this->pos = pos;

        heights.resize(CHUNK_SIZE+1);
        for(int i = 0 ; i < CHUNK_SIZE+1; i++) {
            float x = i + pos * CHUNK_SIZE;
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

        chain.SetPrevVertex(b2Vec2(3.0f, 1.0f));
        chain.SetNextVertex(b2Vec2(-2.0f, 0.0f));
    }
};

const int GROUND_LEN = 8;
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

    void draw(ShaderProgram& s, mat4f projection, vec2f cam) {
        for(int i = 0; i < GROUND_LEN; i++) {
            s.uniform("mvp")->set(glm::translate(projection, vec3f(chunks[i]->pos*CHUNK_SIZE*CHUNK_RESOLUTION-cam.x, -cam.y, 0.0f)));
            chunks[i]->mesh.draw(s);
        }
    }
};

int main() {
	findAssetPath();

	// Create screen
	ContextSettings settings;
	settings.versionMajor = 3;
	settings.versionMinor = 3;
	Window window (Window::DisplayMode::createWindowedMode(800, 600), settings);

	MeshIndexed quad = createQuad();
	ShaderProgram quadShader(
				Storage::openAsset("quad.vert"),
				Storage::openAsset("quad2.frag"));

	Mesh cube = createCube();
	ShaderProgram cubeShader(
				Storage::openAsset("cube.vert"),
				Storage::openAsset("cube.frag"));

	Texture2D awesome = Texture2D::load(Storage::openAsset("awesomeface.png"));

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Ground g;

	while(true) {
		window.update();

		if(Keyboard::pressed(Keyboard::Escape) || window.isClosing())
			break;

		if(Keyboard::pressed(Keyboard::A))
			cout<<"Pressed A!"<<endl;
		if(Keyboard::justPressed(Keyboard::A))
			cout<<"Just Pressed A!"<<endl;
		if(Keyboard::justReleased(Keyboard::A))
			cout<<"Just Released A!"<<endl;

		if(Keyboard::pressed(Keyboard::Q))
			window.setDisplayMode(Window::getFullscreenModes()[0]);
		if(Keyboard::pressed(Keyboard::W))
			window.setDisplayMode(Window::DisplayMode::createWindowedMode(800, 600));
		if(Keyboard::pressed(Keyboard::E))
			window.setDisplayMode(Window::getFullscreenModes()[12]);

		// Set viewport
		glViewport(0, 0, window.getSize().x, window.getSize().y);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Projection matrix.
		float aspect = float(window.getSize().x)/window.getSize().y;
        float zoom = 8.0f;
        mat4f projection = glm::ortho(-zoom*aspect, zoom*aspect, -zoom, zoom);

		// View matrix.
//		mat4f view = glm::lookAt(vec3f(1.0, 1.0, 1.0)*3.0f, vec3f(0, 0, 0), vec3f(0, 1, 0));

		// Model matrix.
        float t = Clock::getSeconds()*50;
        mat4f model = glm::rotate(mat4f(1.0f), t*2.0f, vec3f(0.0, 1.0, 0.0));
        model = glm::scale(model, vec3f(0.5f));

		// Normal matrix
		mat3f normal = glm::inverse(glm::transpose(mat3f(model)));

        // Draw crazy awesome cube. :)
        cubeShader.uniform("mvp")->set(projection*model);
        cubeShader.uniform("norm")->set(normal);
        cubeShader.uniform("tex")->set(awesome);
        cube.draw(cubeShader);

        // Draw crazy awesome cube. :)
        g.load(t);
        g.draw(quadShader, projection, vec2f(t, 0));


		window.swapBuffers();
	}
}
