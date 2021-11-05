#pragma once

#include <string>
#include <vector>
#include "CanvasTriangle.h"
#include "DrawingWindow.h"
#include "Utils.h"
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include "CanvasPoint.h"
#include "Colour.h"

std::vector<std::string> split(const std::string &line, char delimiter);
void fillTriangle(DrawingWindow &window, CanvasPoint a, CanvasPoint b, CanvasPoint c, Colour colour);
uint32_t colourToCode(Colour colour);
void drawTriangle(DrawingWindow &window, CanvasPoint a, CanvasPoint b, CanvasPoint c, Colour colour);
void drawLine(DrawingWindow &window, CanvasPoint start, CanvasPoint end, Colour colour);