#pragma once
#include <RhyColor.h>
// Helper class for different Vertex classes.

struct VertexPosColor
{
	struct {
		float x, y, z;
	} pos;
	struct {
		unsigned char r, g, b, a;
	} color;
};

struct VertexPosRhyColor
{
	struct {
		float x, y, z;
	} pos;
	RhyC::color color;
};

struct VertexPosUV
{
	struct {
		float x, y, z;
	} pos;
	struct {
		float u, v;
	} texCoords;
};

struct VertexPosNormalUV
{
	struct {
		float x, y, z;
	} pos;
	struct {
		float nx, ny, nz;
	} normal;
	struct {
		float u, v;
	} texCoords;
};

struct VertexPosTangentNormalUV
{
	struct {
		float x, y, z;
	} pos;
	struct {
		float x, y, z;
	} tangent;
	struct {
		float x, y, z;
	} normal;
	struct {
		float u, v;
	} texCoords;
};

struct VertexPosBiTangentTangentNormalUV
{
	struct {
		float x, y, z;
	} pos;
	struct {
		float x, y, z;
	} biTangent;
	struct {
		float x, y, z;
	} tangent;
	struct {
		float x, y, z;
	} normal;
	struct {
		float u, v;
	} texCoords;
};

struct VertexPosNormalColor
{
	struct {
		float x, y, z;
	} pos;
	struct {
		float nx, ny, nz;
	} normal;
	struct {
		unsigned char r, g, b, a;
	} color;
};

struct VertexPosRhyColorUV
{
	struct {
		float x, y, z;
	} pos;
	RhyC::color color;
	struct {
		float u, v;
	};
};
