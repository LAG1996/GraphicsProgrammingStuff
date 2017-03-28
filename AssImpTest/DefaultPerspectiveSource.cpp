/*
Author: Luis Angel Garcia
Description: A practice in shading and interactive viewing using GLUT API.
*/

#include <GL/glut.h>
#include <iostream>

#include "Material.h"
#include "Light.h"
#include "Prism.h"
#include "Vec4.h"

#include<assimp/cimport.h>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<assimp/material.h>

void LoadFromFile(const char* fileName)
{
	const aiScene* scene = aiImportFile(fileName, aiProcessPreset_TargetRealtime_MaxQuality);

	if (!scene)
	{
		std::cout << "Could not load file: " << fileName << aiGetErrorString() <<  std::endl;
	}

	//Load all meshes (Assimp separates a model into a mesh for each material)
	

	aiReleaseImport(scene);
}