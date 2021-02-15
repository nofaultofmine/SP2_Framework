#pragma once
#include "Vertex.h"
#include <Mtx44.h>
#include <MatrixStack.h>
Position operator*(const Mtx44& lhs, const Position& rhs);