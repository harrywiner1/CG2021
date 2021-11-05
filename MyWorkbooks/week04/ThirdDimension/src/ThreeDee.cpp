#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include "../libs/personal/Vertex.h"
#include <ModelTriangle.h>
#include <map>
#include <math.h>
#include <Triangles.h>

#define WIDTH 600
#define HEIGHT 400

using namespace std;

vector<string> split(string input, string delimiter)
{
    vector<string> output = {};
    while (input.find(delimiter) != string::npos)
    {
        string token = input.substr(0, input.find(delimiter)); // token is "scott"
        output.push_back(token);
        input.erase(0, input.find(delimiter) + delimiter.length());
    }
    if (input.length() != 0)
    {
        output.push_back(input);
    }
    return output;
}

std::map<string, Colour> readMTL(string filename)
{
    string curLine;
    fstream MyReadFile(filename);

    std::string name;

    std::map<std::string, Colour> colours;

    while (getline(MyReadFile, curLine))
    {
        vector<string> cols = split(curLine, " ");
        if (curLine != "")
        {
            if (cols[0] == "newmtl")
            {
                name = (string)cols[1];
            }
            else if (cols[0] == "Kd")
            {
                Colour newColour(name, stof(cols[1]) * 255, stof(cols[2]) * 255, stof(cols[3]) * 255);
                colours[name] = newColour;
            }
        }
    }

    return colours;
}

vector<ModelTriangle> readObj(string filename, float scale, std::map<string, Colour> colours)
{
    // Create a text string, which is used to output the text file
    string curLine;

    // Read from the text file
    fstream MyReadFile(filename);

    // Use a while loop together with the getline() function to read the file line by line

    vector<ModelTriangle> allTriangles = {};
    vector<glm::vec3> allVectors = {glm::vec3()};

    Colour colour;

    while (getline(MyReadFile, curLine))
    {
        // std::cout << curLine << std::endl;
        // Output the text from the file
        vector<string> cols = split(curLine, " ");
        if (curLine != "")
        {

            if (cols[0].compare("v") == 0)
            {
                allVectors.push_back(glm::vec3(stof(cols[1]) * scale, stof(cols[2]) * scale, stof(cols[3]) * scale));
            }
            else if (cols[0].compare("usemtl") == 0)
            {
                colour = colours[cols[1]];
            }
            else if (cols[0].compare("f") == 0)
            {
                ModelTriangle triangle(allVectors[stoi(cols[1])], allVectors[stoi(cols[2])], allVectors[stoi(cols[3])], colour);
                allTriangles.push_back(triangle);
                // std::cout << allVectors[stoi(cols[1])][0] << allVectors[stoi(cols[1])][1] << allVectors[stoi(cols[1])][2] << std::endl;
            }
        }
    }
    return allTriangles;
}

CanvasPoint objToCanvasPoint(glm::vec3 point, glm::vec3 cameraPos, float focalLength, int windowWidth, int windowHeight)
{
    glm::vec3 distanceVector = cameraPos - point;
    float u = focalLength * distanceVector[0] / distanceVector[2] + windowWidth / 2;
    float v = focalLength * distanceVector[1] / distanceVector[2] + windowHeight / 2;

    return CanvasPoint(u, v);
}

void drawCanvasPoint(CanvasPoint x, Colour colour, DrawingWindow &window)
{
    window.setPixelColour(x.x, x.y, 4294967295);
}

void drawWireframes(vector<ModelTriangle> modelTriangles, float focalLength, glm::vec3 cameraPos, DrawingWindow &window)
{
    for (int i = 0; i < modelTriangles.size(); i++)
    {
        // for each triangle, convert the points from object file format into image plane format
        CanvasPoint a = objToCanvasPoint(modelTriangles[i].vertices[0], cameraPos, focalLength, window.width, window.height);
        CanvasPoint b = objToCanvasPoint(modelTriangles[i].vertices[1], cameraPos, focalLength, window.width, window.height);
        CanvasPoint c = objToCanvasPoint(modelTriangles[i].vertices[2], cameraPos, focalLength, window.width, window.height);

        drawCanvasPoint(a, modelTriangles[i].colour, window);
        drawCanvasPoint(b, modelTriangles[i].colour, window);
        drawCanvasPoint(c, modelTriangles[i].colour, window);
    }
}

void handleEvent(SDL_Event event, DrawingWindow &window)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_LEFT)
            std::cout << "LEFT" << std::endl;
        else if (event.key.keysym.sym == SDLK_RIGHT)
            std::cout << "RIGHT" << std::endl;
        else if (event.key.keysym.sym == SDLK_UP)
            std::cout << "UP" << std::endl;
        else if (event.key.keysym.sym == SDLK_DOWN)
            std::cout << "DOWN" << std::endl;
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        window.savePPM("output.ppm");
        window.saveBMP("output.bmp");
    }
}

int main(int argc, char *argv[])
{
    DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
    SDL_Event event;
    std::map<string, Colour> colours = readMTL("./models/cornell-box.mtl");
    vector<ModelTriangle> triangles = readObj("./models/blox.obj", .17, colours);

    glm::vec3 cameraPos(0, 0, 2);
    float focalLength = 4;
    drawWireframes(triangles, focalLength, cameraPos, window);
    //renderTriangles(triangles, focalLength, cameraPos, window);
    while (true)
    {
        // We MUST poll for events - otherwise the window will freeze !

        if (window.pollForInputEvents(event))
            handleEvent(event, window);

        // Need to render the frame at the end, or nothing actually gets shown on the screen !
        window.renderFrame();
    }
    return 0;
}
