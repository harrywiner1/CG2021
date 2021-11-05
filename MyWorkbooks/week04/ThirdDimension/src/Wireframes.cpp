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

void printCanvasPoint(CanvasPoint point)
{
	std::cout << "(" << point.x << ',' << point.y << ")" << std::endl;
}

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

float Distance3d(glm::vec3 a, glm::vec3 b)
{
	int total = 0;
	for (int i = 0; i < 3; i++)
	{
		total += sqrt(pow(a[i] - b[i], 2));
	}
	return total;
}

CanvasPoint calcImagePoint(glm::vec3 cameraPos, glm::vec3 vertex, float focalLength, int windowWidth, int windowHeight)
{
	glm::vec3 distanceVector = vertex - cameraPos;
	if (distanceVector[2] > cameraPos[2])
	{
		std::cout << "Distance vector is behind camera" << std::endl;
	}

	float xPos = -1 * (focalLength * distanceVector[0] / distanceVector[2]) + (windowWidth / 2);
	float yPos = (focalLength * distanceVector[1] / distanceVector[2]) + (windowHeight / 2);
	return CanvasPoint(xPos, yPos);
}

void drawPoints(vector<ModelTriangle> triangles, float focalLength, glm::vec3 cameraPos, DrawingWindow &window)
{
	for (int i = 0; i < triangles.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			CanvasPoint point = calcImagePoint(cameraPos, triangles[i].vertices[j], focalLength, window.width, window.height);
			// printCanvasPoint(point);
			window.setPixelColour(point.x, point.y, colourToCode(triangles[i].colour));
		}
	}
}

void drawTriangle(CanvasTriangle triangle, DrawingWindow &window, Colour colour)
{
	drawLine(window, triangle[0], triangle[1], colour);
	drawLine(window, triangle[1], triangle[2], colour);
	drawLine(window, triangle[2], triangle[0], colour);
}

void drawWireframes(vector<ModelTriangle> triangles, float focalLength, glm::vec3 cameraPos, DrawingWindow &window)
{
	for (int i = 0; i < triangles.size(); i++)
	{

		CanvasPoint a = calcImagePoint(cameraPos, triangles[i].vertices[0], focalLength, window.width, window.height);
		CanvasPoint b = calcImagePoint(cameraPos, triangles[i].vertices[1], focalLength, window.width, window.height);
		CanvasPoint c = calcImagePoint(cameraPos, triangles[i].vertices[2], focalLength, window.width, window.height);

		CanvasTriangle triangle(a, b, c);

		drawTriangle(triangle, window, triangles[i].colour);
	}
}

void renderTriangles(vector<ModelTriangle> triangles, float focalLength, glm::vec3 cameraPos, DrawingWindow &window)
{
	for (int i = 0; i < triangles.size(); i++)
	{

		CanvasPoint a = calcImagePoint(cameraPos, triangles[i].vertices[0], focalLength, window.width, window.height);
		CanvasPoint b = calcImagePoint(cameraPos, triangles[i].vertices[1], focalLength, window.width, window.height);
		CanvasPoint c = calcImagePoint(cameraPos, triangles[i].vertices[2], focalLength, window.width, window.height);

		CanvasTriangle triangle(a, b, c);
		std::cout << i << std::endl;
		std::cout << "Model: " << triangles[i] << std::endl;

		std::cout << "Canvas: " << triangle << std::endl;
		printCanvasPoint(a);
		printCanvasPoint(b);
		printCanvasPoint(c);

		fillTriangle(window, a, b, c, triangles[i].colour);
	}
}

int main(int argc, char *argv[])
{
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	std::map<string, Colour> colours = readMTL("./models/cornell-box.mtl");
	vector<ModelTriangle> triangles = readObj("./models/cornell-box.obj", .17 * 3, colours);

	glm::vec3 cameraPos(0, 0, 3);
	float focalLength = 150;
	// drawWireframes(triangles, focalLength, cameraPos, window);
	renderTriangles(triangles, focalLength, cameraPos, window);
	//drawPoints(triangles, focalLength, cameraPos, window);
	while (true)
	{
		// We MUST poll for events - otherwise the window will freeze !

		if (window.pollForInputEvents(event))
			handleEvent(event, window);

		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
	}
}
