#include <VBE/VBE.hpp>
#include <iostream>
#include <memory>

using namespace std;


MeshBase* createQuad() {
	// Create the quad mesh. Example of not indexed mesh.
	Vertex::Format format({
		Vertex::Element(Vertex::Element(Vertex::Attribute::Position, Vertex::Element::Float, 2))
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

	MeshIndexed* quad = new MeshIndexed(format);
	quad->setVertexData(&data[0], data.size());
	quad->setIndexData(&indices[0], indices.size());
	quad->setPrimitiveType(Mesh::TRIANGLES);

	return quad;
}

MeshBase* createCube() {
	// Create cube mesh. Example of indexed mesh.

	Vertex::Format format({
		Vertex::Element(Vertex::Element(Vertex::Attribute::Position, Vertex::Element::Float, 3)),
		Vertex::Element(Vertex::Element(Vertex::Attribute::Normal, Vertex::Element::Float, 3)),
		Vertex::Element(Vertex::Element(Vertex::Attribute::TexCoord, Vertex::Element::Float, 2))
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

	Mesh* cube = new Mesh(format);
	cube->setPrimitiveType(Mesh::TRIANGLES);
	cube->setVertexData(&cubeVertices[0], cubeVertices.size());

	return cube;
}

void printFullscreenModes() {
	const auto& modes = Window::getFullscreenModes();
	for(int i = 0; i < modes.size(); i++) {
		cout<<i<<": "<<modes[i].getWidth()<<" "<<modes[i].getHeight()<<endl;
	}
}

int main() {
	cout<<"Hello VBE!"<<endl;

	printFullscreenModes();

	// Create screen
	Window window (Window::DisplayMode::createWindowedMode(800, 600));

	unique_ptr<MeshBase> quad(createQuad());
	unique_ptr<ShaderProgram> quadShader(ShaderProgram::loadFromFile("assets/quad.vert", "assets/quad.frag"));
	unique_ptr<MeshBase> cube(createCube());
	unique_ptr<ShaderProgram> cubeShader(ShaderProgram::loadFromFile("assets/cube.vert", "assets/cube.frag"));
	unique_ptr<Texture2D> awesome(Texture2D::createFromFile("assets/awesomeface.png"));

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while(true) {
		window.update();

		if(Keyboard::pressed(Keyboard::Escape))
			break;

		if(Keyboard::pressed(Keyboard::Q))
			window.setDisplayMode(Window::getFullscreenModes()[0]);
		if(Keyboard::pressed(Keyboard::W))
			window.setDisplayMode(Window::DisplayMode::createWindowedMode(800, 600));
		if(Keyboard::pressed(Keyboard::E))
			window.setDisplayMode(Window::getFullscreenModes()[12]);

		// Set viewport
		glViewport(0, 0, window.getSize().x, window.getSize().y);
		// Clear screen.
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw fullscreen quad with fancy shader.
		quadShader->uniform("t")->set(Clock::getSeconds());
		quadShader->uniform("resolution")->set(vec2f(window.getSize()));
		quad->draw(quadShader.get());

		// Draw crazy awesome cube. :)

		glClear(GL_DEPTH_BUFFER_BIT);
		// Projection matrix.
		float aspect = float(window.getSize().x)/window.getSize().y;
		mat4f projection = glm::perspective(60.0f, aspect, 0.01f, 100.0f);

		// View matrix.
		mat4f view = glm::lookAt(vec3f(1.0, 1.0, 1.0)*3.0f, vec3f(0, 0, 0), vec3f(0, 1, 0));

		// Model matrix.
		float t = Clock::getSeconds();
		mat4f model = glm::rotate(mat4f(1.0f), t*120.0f, vec3f(0.0, 1.0, 0.0));

		// Normal matrix
		mat3f normal = glm::inverse(glm::transpose(mat3f(model)));

		// Draw it! :D
		cubeShader->uniform("mvp")->set(projection*view*model);
		cubeShader->uniform("norm")->set(normal);
		cubeShader->uniform("tex")->set((int)awesome->getSlot());
		cube->draw(cubeShader.get());

		window.swapBuffers();
	}
}
