#include "engine.h"
#include <time.h>
#include <iostream>
#include <glm/ext/matrix_transform.hpp>

namespace Sefirah
{
	void Engine::PopulateWorld()
	{
		srand(time(NULL));

		for (int i = 0; i < startingPopulation; i++)
		{
			Organism* organism = new Organism(geneCount);
			world->AddOrganism(organism);
			world->RandomlyPlaceOrganism(organism);
		}
	}

	void Engine::UpdateOrganisms()
	{
		srand(time(NULL));

		std::vector<Organism*> os = world->GetOrganisms();
		for (int i = 0; i < os.size(); i++)
		{
			Organism* o = os[i];

			// First, we need to check if the creature is starving.
			int satiation = o->GetSatiation();

			if (satiation == 0)
			{
				world->KillOrganism(o);
				continue;
			}
			else if (satiation > 100)
			{
				world->ReproduceOrganism(o);
				o->Starve(100);
			}

			// If not, then we need to run the creature's little brain.
			neural->RunBrain(o, world, maxRecurseDepth);
		}
	}

	void Engine::RenderOrganisms()
	{
		std::vector<Organism*> os = world->GetOrganisms();
		for (int i = 0; i < os.size(); i++)
		{
			Organism* o = os[i];
			renderer->RenderPoint(o->GetPosition(), o->GetColor());
		}
	}

	void Engine::Update()
	{
		while (!glfwWindowShouldClose(window))
		{
			float deltaTime = glfwGetTime() - lastTime;
			lastTime = glfwGetTime();

			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPosition.x += 500 * deltaTime;
			else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPosition.x -= 500 * deltaTime;
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPosition.y += 500 * deltaTime;
			else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPosition.y -= 500 * deltaTime;
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) zoom += 2 * deltaTime;
			else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) zoom -= 2 * deltaTime;

			int w, h;
			glfwGetWindowSize(window, &w, &h);
			windowWidth = w, windowHeight = h;
			glViewport(0, 0, windowWidth, windowHeight);

			renderer->SetView(glm::lookAt(cameraPosition, cameraPosition + glm::vec3(0, 0, -1.0f), glm::vec3(0, 1, 0)));
			renderer->SetProjection(windowWidth, windowHeight, zoom, 0.01f, 10.0f);

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			UpdateOrganisms();
			RenderOrganisms();
			renderer->Render();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	void Engine::Terminate()
	{
		world->Terminate();
		delete renderer;
		delete neural;
		delete this;
	}

	Engine::Engine(unsigned int w, unsigned int h, std::string t, int startingPopulation, int geneCount)
	{
		this->startingPopulation = startingPopulation;
		this->geneCount = geneCount;

		// Window Details
		this->windowWidth = w;
		this->windowHeight = h;
		this->windowTitle = t;

		// Camera Details
		this->cameraPosition = glm::vec3(0, 0, 0);
		this->zoom = 1.0f;

		// Initiate OpenGL-Related Stuff
		if (!glfwInit())
		{
			std::cout << "Failed to initialize GLFW." << std::endl;
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHintString(GLFW_X11_CLASS_NAME, "OpenGL");
		glfwWindowHintString(GLFW_X11_INSTANCE_NAME, "OpenGL");

		window = glfwCreateWindow(windowWidth, windowHeight, t.c_str(), NULL, NULL);

		if (!window)
		{
			glfwTerminate();
			std::cout << "Failed to create Opengl Window." << std::endl;
			return;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Setup
		world = new World();
		neural = new Neural();
		renderer = new Renderer();

		PopulateWorld();
	}
}