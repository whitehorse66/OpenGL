#pragma once
#include <array>
#include <GL/glew.h>

//Ş¿ƒf[ƒ^
struct Material {
	//ŠÂ‹«Œõ‚Ì”½ËŒW”
	alignas(16) std::array<GLfloat, 3> ambient;
	//ŠgU”½ËŒW”
	alignas(16) std::array<GLfloat, 3> diffuse;
	//‹¾–Ê”½ËŒW”
	alignas(16) std::array<GLfloat, 3> specular;
	//‹P‚«ŒnƒX
	alignas(4) GLfloat shininess;
};