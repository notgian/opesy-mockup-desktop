#pragma once

#include <GL/glew.h>

// Helper function to load an image file from disk into an OpenGL texture (GLuint)
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);