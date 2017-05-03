#include "Mesh.h"
#include "Model.h"

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Vertex Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);

	//Texture Coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(2);

	//Vector IDs
	glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, id));
	glEnableVertexAttribArray(3);

	//Vector Weights
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Weights));
	glEnableVertexAttribArray(4);

	//Instance matrix
	GLsizei vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid *)0);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid *)(vec4Size));
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid *)(2 * vec4Size));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid *)(3 * vec4Size));

	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);

	glBindVertexArray(0);
}

void Mesh::Draw(Shader shader, glm::vec3 Color, float shininess, bool instanced, int count = 0)
{
	GLuint diffuse_Num = 1;
	GLuint specular_Num = 1;
	if (this->ListTextures.size() == 0)
	{
		glUniform1i(glGetUniformLocation(shader.Program, "accessTextures"), 0);
		glUniform3f(glGetUniformLocation(shader.Program, "material.defaultDiffuse"), Color.x, Color.y, Color.z);
	}
	else
	{
		glUniform1i(glGetUniformLocation(shader.Program, "accessTextures"), 1);
		for (int i = 0; i < this->ListTextures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			std::stringstream ss;
			std::string number;
			std::string name = ListTextures[i].type;

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

	glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), shininess);

	glUniform3f(glGetUniformLocation(shader.Program, "material.ambient"), 1.0f, 1.0f, 1.0f);

	//Load the bone matrices if the model is rigged
	if (Model_I_Belong_To->is_rigged)
	{
		/*
		for (int i = 0; i < Model_I_Belong_To->GetSkeleton()->List_Bones.size(); i++)
		{
			std::cout << "[\n " << Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(i)[0][0] << ", " << Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(i)[0][1] << ", " << Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(i)[0][2] << ", " << Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(i)[0][3] << std::endl
				<< Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(i)[1][0] << ", " << Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(i)[1][1] << ", " << Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(i)[1][2] << ", " << Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(i)[1][3] << std::endl
				<< Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(i)[2][0] << ", " << Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(i)[2][1] << ", " << Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(i)[2][2] << ", " << Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(i)[2][3] << std::endl
				<< Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(i)[3][0] << ", " << Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(i)[3][1] << ", " << Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(i)[3][2] << ", " << Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(i)[3][3] << std::endl
				<< "]" << std::endl;
		}*/
		
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "mBones"),
			Model_I_Belong_To->GetSkeleton()->BoneMatrices.size(), GL_FALSE,
			glm::value_ptr(Model_I_Belong_To->GetSkeleton()->BoneMatrices.at(0)));
	}
		

	//Draw the mesh with textures and shading and all that beautiful glory
	glBindVertexArray(VAO);
	if (instanced)
	{
		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, count);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	
	
	glBindVertexArray(0);

	for (GLuint i = 0; i < this->ListTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}