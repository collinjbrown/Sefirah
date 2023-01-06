#ifndef ENGINE_H
#define ENGINE_H

#include "neural.h"
#include "renderer.h"

#include <GLFW/glfw3.h>

namespace Sefirah
{
	class Engine
	{
	private:
		GLFWwindow*		window;
		std::string		windowTitle;
		unsigned int	windowWidth;
		unsigned int	windowHeight;

		glm::vec3		cameraPosition;
		float			zoom;

		World*			world;
		Neural*			neural;
		Renderer*		renderer;

		int				startingPopulation;
		int				geneCount;
		float			evolveChance = 1.0f;
		int				maxRecurseDepth = 10;

		float			lastTime = 0.0f;

	public:
		void			PopulateWorld();
		void			UpdateOrganisms();
		void			RenderOrganisms();
		void			Update();
		void			Terminate();

		Engine(unsigned int w, unsigned int h, std::string t, int startingPopulation, int geneCount);
	};
}

#endif