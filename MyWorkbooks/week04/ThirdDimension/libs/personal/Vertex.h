#pragma once

#include <iostream>

// a vertex in 3D
struct Vertex
{
    std::string name;
    float vals{};
    Vertex();
    Vertex(int a, int b, int c);
};