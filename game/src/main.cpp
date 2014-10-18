#include <VBE/VBE.hpp>
#include <iostream>
#include <memory>

using namespace std;


Mesh* createQuad() {
	// Create the quad mesh. Example of not indexed mesh.
	Vertex::Format format({
		Vertex::Element(Vertex::Element(Vertex::Attribute::Position, Vertex::Element::Float, 2))
	});

	vector<vec2f> data = {
		vec2f(1, -1), vec2f(1, 1), vec2f(-1, 1),
		vec2f(-1, 1), vec2f(-1, -1), vec2f(1, -1)
	};

	Mesh* quad = Mesh::loadEmpty(format);
	quad->setVertexData(&data[0], 6);
	quad->setPrimitiveType(Mesh::TRIANGLES);

	return quad;
}

Mesh* createCube() {
	// Create cube mesh. Example of indexed mesh.

	Vertex::Format format({
		Vertex::Element(Vertex::Element(Vertex::Attribute::Position, Vertex::Element::Float, 3))
	});

	vector<vec3f> cubeVertices = {
		vec3f(-1.0, -1.0, 1.0),
		vec3f(1.0, -1.0, 1.0),
		vec3f(-1.0, 1.0, 1.0),
		vec3f(1.0, 1.0, 1.0),
		vec3f(-1.0, -1.0, -1.0),
		vec3f(1.0, -1.0, -1.0),
		vec3f(-1.0, 1.0, -1.0),
		vec3f(1.0, 1.0, -1.0),
	};

	vector<unsigned int> cubeIndices = {
		0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
	};

	Mesh* cube = Mesh::loadEmpty(format, Mesh::Indexed);
	cube->setPrimitiveType(Mesh::TRIANGLE_STRIP);
	cube->setVertexData(&cubeVertices[0], cubeVertices.size());
	cube->setVertexIndices(&cubeIndices[0], cubeIndices.size());

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

	unique_ptr<Mesh> quad(createQuad());
	unique_ptr<ShaderProgram> quadShader(ShaderProgram::loadFromFile("assets/quad.vert", "assets/quad.frag"));
	unique_ptr<Mesh> cube(createCube());
	unique_ptr<ShaderProgram> cubeShader(ShaderProgram::loadFromFile("assets/cube.vert", "assets/cube.frag"));

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
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw fullscreen quad with fancy shader.
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		quadShader->uniform("t")->set(Clock::getSeconds());
		quadShader->uniform("resolution")->set(vec2f(window.getSize()));
		quad->draw(quadShader.get());

		// Draw demoscene-ish crazy wireframe cube. :)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Projection matrix.
		float aspect = float(window.getSize().x)/window.getSize().y;
		mat4f mvp = glm::perspective(60.0f, aspect, 0.01f, 100.0f);

		// Modelview matrix. (We multiply it directly into the MVP)
		float t = Clock::getSeconds();
		float zoom = 4.0f;
		mvp *= glm::lookAt(vec3f(sin(t), 1.0f, cos(t))*zoom, vec3f(0, 0, 0), vec3f(0, 1, 0));

		// Draw it! :D
		cubeShader->uniform("mvp")->set(mvp);
		cube->draw(cubeShader.get());

		window.swapBuffers();
	}
}
