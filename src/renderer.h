#ifndef RENDERER_H
#define RENDERER_H

#include "shader.h"

#include <vector>
#include <array>

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace Sefirah
{
	struct Vertex
	{
		// Location
		float x;
		float y;
		float z;

		// Color
		float r;
		float g;
		float b;
		float a;
	};

	struct Triangle
	{
		Vertex topLeft;
		Vertex bottomRight;
		Vertex bottomLeft;
	};

	struct Batch
	{
		static constexpr int MAX_TRIS = 20000;

		std::array<Triangle, MAX_TRIS> buffer;
		int index = 0;
	};

	class Renderer
	{
	private:
		std::vector<Batch>	batches;
		Shader				shader;

		glm::mat4		view;
		glm::mat4		projection;

		GLuint VAO;
		GLuint VBO;


		void Flush(const Batch& batch);
		void ResetBuffers();

		int tris = 0;

	public:
		float tileSize = 8.0f;

		void SetView(glm::mat4 view) { this->view = view; }
		void SetProjection(int width, int height, float zoom, float nearClip, float farClip)
		{
			float halfWidth = (width / 2.0f) * zoom;
			float halfHeight = (height / 2.0f) * zoom;

			this->projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearClip, farClip);
		}

		void RenderPoint(glm::vec2 pos, glm::vec4 color);

		void Render();

		Renderer();
	};
}

#endif