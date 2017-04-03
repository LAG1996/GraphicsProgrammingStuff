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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ListVertices.size() * sizeof(GLuint), &indices, GL_STATIC_DRAW);

	//Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	//Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

	//Texture Coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));


	glBindVertexArray(0);
}

void Mesh::Draw(Shader shader)
{
	GLuint diffuse_Num = 1;
	GLuint specular_Num = 1;

	for (int i = 0; i < ListTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		stringstream ss;
		string number;
		string name = ListTextures[i].type;

		if (ListTextures[i].type == "texture_diffuse")
		{
			ss << diffuse_Num++;
		}
		if (ListTextures[i].type == "texture_specular")
		{
			ss << specular_Num++;
		}

		number = ss.str();

		glUniform1i(glGetUniformLocation(shader.Program, ("material." + name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, ListTextures[i].id); //Bind the texture
	}
	glActiveTexture(GL_TEXTURE0);

	//Draw the mesh with textures and shading and all that beautiful glory
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}