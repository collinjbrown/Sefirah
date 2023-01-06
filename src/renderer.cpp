#include "renderer.h"

#include <iostream>
#include <filesystem>
#include <glm/ext/matrix_clip_space.hpp>

namespace Sefirah
{
	void Renderer::Flush(const Batch& batch)
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, batch.index * sizeof(Triangle), &batch.buffer[0]);
		glDrawElements(GL_TRIANGLES, batch.index * 3, GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::ResetBuffers()
	{
		for (Batch& batch : batches) batch.index = 0;

		tris = 0;
	}

	void Renderer::RenderPoint(glm::vec2 pos, glm::vec4 color)
	{
		int bNum = tris / Batch::MAX_TRIS;
		if (static_cast<unsigned long long>(bNum) + 1 > batches.size())
		{
			batches.push_back(*(new Batch()));
		}

		Batch& batch = batches[bNum];

		const float right = (tileSize * pos.x) - (tileSize / 2.0f);
		const float left = (tileSize * pos.x) + (tileSize / 2.0f);
		const float bottom = (tileSize * pos.y) - (tileSize / 2.0f);
		const float top = (tileSize * pos.y) + (tileSize / 2.0f);

		const float r = color.r;
		const float g = color.g;
		const float b = color.b;
		const float a = color.a;

		Vertex bottomLeft{ left, bottom, -1, r, g, b, a };
		Vertex bottomRight{ right, bottom, -1, r, g, b, a };
		Vertex topLeft{ left, top, -1, r, g, b, a };
		Vertex topRight{ right, top, -1, r, g, b, a };

		batch.buffer[batch.index] = { bottomLeft, bottomRight, topLeft };
		batch.index++;

		batch.buffer[batch.index] = { bottomRight, topRight, topLeft };
		batch.index++;

		tris += 2;
	}

	void Renderer::Render()
	{
		shader.Use();
		shader.SetMatrix("MVP", projection * view);

		for (int i = 0; i < batches.size(); i++)
		{
			Flush(batches[i]);
		}

		ResetBuffers();
	}

	Renderer::Renderer() : batches(1), shader("assets/shaders/base.vert", "assets/shaders/base.frag")
	{
		GLuint IBO;

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glBufferData(GL_ARRAY_BUFFER, Batch::MAX_TRIS * sizeof(Triangle), nullptr, GL_DYNAMIC_DRAW);

		// Coordinates
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
		glEnableVertexAttribArray(0);

		// Color
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));
		glEnableVertexAttribArray(1);

		unsigned int indices[Batch::MAX_TRIS * 3];
		for (int i = 0; i < Batch::MAX_TRIS; i++)
		{
			const int offset = 3 * i;

			indices[offset + 0] = offset + 0;
			indices[offset + 1] = offset + 1;
			indices[offset + 2] = offset + 2;
		}

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(shader.ID);
	}
}