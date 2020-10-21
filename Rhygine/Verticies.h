#pragma once
// Helper class for different Vertex classes.

struct VertexPosColor {
	struct {
		float x, y, z;
	} pos;
	struct {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	} color;
};

struct VertexPosUV {
	struct {
		float x, y, z;
	} pos;
	struct {
		float u;
		float v;
	} texCoords;
};

struct VertexPosNormalUV {
	struct {
		float x, y, z;
	} pos;
	struct {
		float nx, ny, nz;
	} normal;
	struct {
		float u;
		float v;
	} texCoords;
};

struct VertexPosNormalColor {
	struct {
		float x, y, z;
	} pos;
	struct {
		float nx, ny, nz;
	} normal;
	struct {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	} color;
};
