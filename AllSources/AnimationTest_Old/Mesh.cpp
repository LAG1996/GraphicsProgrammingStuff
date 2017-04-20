#include "Mesh.h"
void Mesh::_SetUpMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, ListVertices.size() * sizeof(Vertex), &ListVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	//Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	//Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

	//Texture Coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	//Vector IDs
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, id));

	//Vector Weights
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Weights));


	glBindVertexArray(0);
}

void Mesh::Draw(Shader shader, glm::vec3 Color, float shininess)
{
	GLuint diffuse_Num = 1;
	GLuint specular_Num = 1;
	if (this->ListTextures.size() == 0)
	{
		glUniform1i(glGetUniformLocation(shader.Program, "accessTextures"), 0);
		glUniform3f(glGetUniformLocation(shader.Program, "material.defaultDiffuse"), 0.3f, 0.3f, 0.3f);
	}
	else
	{
		glUniform1i(glGetUniformLocation(shader.Program, "accessTextures"), 1);
		for (int i = 0; i < this->ListTextures.size(); i++)
		{


			glActiveTexture(GL_TEXTURE0 + i);
			stringstream ss;
			string number;
			string name = ListTextures[i].type;

			if (this->ListTextures[i].type == "texture_diffuse")
			{
				ss << diffuse_Num++;
			}
			if (this->ListTextures[i].type == "texture_specular")
			{
				ss << specular_Num++;
			}

			number = ss.str();

			glUniform1i(glGetUniformLocation(shader.Program, ("material." + name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, this->ListTextures[i].id); //Bind the texture
		}
	}

	glActiveTexture(GL_TEXTURE0);

	glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), shininess);

	glUniform3f(glGetUniformLocation(shader.Program, "material.ambient"), Color.x, Color.y, Color.z);


	//Draw the mesh with textures and shading and all that beautiful glory
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (GLuint i = 0; i < this->ListTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}