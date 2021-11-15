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
#include <RayTriangleIntersection.h>
#include <glm/gtx/string_cast.hpp>
#define WIDTH 600
#define HEIGHT 400

#define RIGHT 0
#define UP 1
#define FORWARD 2

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

float Distance3d(glm::vec3 a, glm::vec3 b)
{
	int total = 0;
	for (int i = 0; i < 3; i++)
	{
		total += sqrt(pow(a[i] - b[i], 2));
	}
	return total;
}

CanvasPoint calcImagePoint(glm::vec3 cameraPos, glm::mat3 cameraOri, glm::vec3 vertex, float focalLength, int windowWidth, int windowHeight)
{
	glm::vec3 distanceVector = (vertex - cameraPos) * cameraOri;
	if (distanceVector[2] > cameraPos[2])
	{
		//std::cout << "Distance vector is behind camera" << std::endl;
		return CanvasPoint(-1, -1);
	}

	float xPos = -1 * (focalLength * distanceVector[0] / distanceVector[2]) + (windowWidth / 2);
	float yPos = (focalLength * distanceVector[1] / distanceVector[2]) + (windowHeight / 2);
	return CanvasPoint(xPos, yPos);
}

void drawPoints(vector<ModelTriangle> triangles, float focalLength, glm::vec3 cameraPos, glm::mat3 cameraOri, DrawingWindow &window)
{
	for (int i = 0; i < triangles.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			CanvasPoint point = calcImagePoint(cameraPos, cameraOri, triangles[i].vertices[j], focalLength, window.width, window.height);
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

void drawWireframes(vector<ModelTriangle> triangles, float focalLength, glm::vec3 cameraPos, glm::mat3 cameraOri, DrawingWindow &window)
{
	for (int i = 0; i < triangles.size(); i++)
	{

		CanvasPoint a = calcImagePoint(cameraPos, cameraOri, triangles[i].vertices[0], focalLength, window.width, window.height);
		CanvasPoint b = calcImagePoint(cameraPos, cameraOri, triangles[i].vertices[1], focalLength, window.width, window.height);
		CanvasPoint c = calcImagePoint(cameraPos, cameraOri, triangles[i].vertices[2], focalLength, window.width, window.height);

		CanvasTriangle triangle(a, b, c);

		drawTriangle(triangle, window, triangles[i].colour);
	}
}

void render(vector<ModelTriangle> triangles, float focalLength, glm::vec3 cameraPos, glm::mat3 cameraOri, DrawingWindow &window)
{
	for (int i = 0; i < triangles.size(); i++)
	{

		CanvasPoint a = calcImagePoint(cameraPos, cameraOri, triangles[i].vertices[0], focalLength, window.width, window.height);
		CanvasPoint b = calcImagePoint(cameraPos, cameraOri, triangles[i].vertices[1], focalLength, window.width, window.height);
		CanvasPoint c = calcImagePoint(cameraPos, cameraOri, triangles[i].vertices[2], focalLength, window.width, window.height);

		CanvasTriangle triangle(a, b, c);

		fillTriangle(window, a, b, c, triangles[i].colour);
	}
}

void translateCamera(glm::vec3 &cameraPos, int direction, float magnitude)
{
	if (direction == RIGHT)
		cameraPos = cameraPos + glm::vec3(magnitude, 0, 0);
	else if (direction == UP)
		cameraPos = cameraPos + glm::vec3(0, magnitude, 0);
	else if (direction == FORWARD)
		cameraPos = cameraPos + glm::vec3(0, 0, magnitude);
}

void rotateCamera(glm::mat3 &cameraOri, int direction, float magnitude)
{
	if (direction == UP)
	{
		cameraOri *= glm::mat3(glm::vec3(1, 0, 0),
							   glm::vec3(0, cos(magnitude), -sin(magnitude)),
							   glm::vec3(0, sin(magnitude), cos(magnitude)));
	}
	else if (direction == RIGHT)
	{
		cameraOri *= glm::mat3(glm::vec3(cos(magnitude), 0, sin(magnitude)),
							   glm::vec3(0, 1, 0),
							   glm::vec3(-sin(magnitude), 0, cos(magnitude)));
	}
	else if (direction == FORWARD)
	{
		cameraOri *= glm::mat3(glm::vec3(cos(magnitude), -sin(magnitude), 0),
							   glm::vec3(sin(magnitude), cos(magnitude), 0),
							   glm::vec3(0, 0, 1));
	}

	//std::cout << cameraOri << std::endl;
}

/**
 * @brief returns the current angle
 * 
 * @param cameraPos 
 * @param radius 
 */
void orbit(glm::vec3 &cameraPos, float radius, int t)
{
	if (cameraPos[2] == 0)
	{
		return;
	}
	float newAngle = (t * .01);
	cameraPos[0] = radius * sin(newAngle);
	cameraPos[2] = radius * cos(newAngle);
	// std::cout << cameraPos[0] << "," << cameraPos[1] << "," << cameraPos[2] << "," << std::endl;
}

void orbit(glm::vec3 &cameraPos, glm::vec3 centre, float deltaRad)
{

	glm::vec3 distance = cameraPos - centre;
	float radius = sqrt(distance[0] * distance[0] + distance[2] * distance[2]);
	//float angle = asin(distance[2] / radius) - deltaRad;
	float angle = atan(distance[2] / distance[0]) - deltaRad;
	cameraPos[2] = radius * sin(angle);
	cameraPos[0] = radius * cos(angle);
	std::cout << glm::to_string(cameraPos) << ", distance: " << glm::to_string(distance) << ", angle: " << angle << ", radius: " << radius << std::endl;
}

void lookAt(glm::vec3 cameraPos, glm::mat3 &cameraOri, glm::vec3 subject)
{
	std::cout << "Mat before: " << glm::to_string(cameraOri) << std::endl;
	std::cout << "Camerapos: " << glm::to_string(cameraPos) << std::endl;
	glm::vec3 forward = cameraPos - subject;

	glm::vec3 right = glm::cross(glm::vec3(0, 1, 0), forward);
	glm::vec3 up = glm::cross(forward, right);

	cameraOri = glm::transpose(glm::mat3(right, up, forward));
	std::cout << "Mat after: " << glm::to_string(cameraOri) << std::endl;
}

RayTriangleIntersection getClosestIntersection(glm::vec3 cameraPos, glm::vec3 rayDirection, vector<ModelTriangle> triangles)
{
	float minDistance = 10000;
	for (ModelTriangle &triangle : triangles)
	{

		std::cout << "Min distance: " << minDistance << std::endl;
		glm::vec3 e0 = triangle.vertices[1] - triangle.vertices[0];
		glm::vec3 e1 = triangle.vertices[2] - triangle.vertices[0];
		glm::vec3 SPVector = cameraPos - triangle.vertices[0];
		glm::mat3 DEMatrix(-rayDirection, e0, e1);
		glm::vec3 possibleSolution = glm::inverse(DEMatrix) * SPVector;

		RayTriangleIntersection intersection(possibleSolution, possibleSolution[0], triangle, 0);

		float u = possibleSolution[1];
		float v = possibleSolution[2];

		if (intersection.distanceFromCamera >= 0 && u >= 0 && v >= 0 && u + v <= 1)
		{
			if (intersection.distanceFromCamera <= minDistance)
				minDistance = intersection.distanceFromCamera;
		}
		std::cout << "Distance: " << intersection.distanceFromCamera << std::endl;
	}
}

int draw(vector<ModelTriangle> triangles, float focalLength, glm::vec3 &cameraPos, glm::mat3 cameraOri, DrawingWindow &window, int t)
{
	//orbit(cameraPos, glm::vec3(0, 0, 0), .01);
	//render(triangles, focalLength, cameraPos, cameraOri, window);
	drawWireframes(triangles, focalLength, cameraPos, cameraOri, window);
	return 0;
}

void handleEvent(SDL_Event event, DrawingWindow &window, glm::vec3 &cameraPos, glm::mat3 &cameraOri)
{
	float deltaRad = .1;
	float deltaTrans = .5;
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_LEFT)
			translateCamera(cameraPos, RIGHT, -1 * deltaTrans);
		else if (event.key.keysym.sym == SDLK_RIGHT)
			translateCamera(cameraPos, RIGHT, deltaTrans);
		else if (event.key.keysym.sym == SDLK_UP)
			translateCamera(cameraPos, FORWARD, -1 * deltaTrans);
		else if (event.key.keysym.sym == SDLK_DOWN)
			translateCamera(cameraPos, FORWARD, deltaTrans);
		else if (event.key.keysym.sym == SDLK_i)
			rotateCamera(cameraOri, UP, deltaRad);
		else if (event.key.keysym.sym == SDLK_k)
			rotateCamera(cameraOri, UP, -deltaRad);
		else if (event.key.keysym.sym == SDLK_j)
			rotateCamera(cameraOri, RIGHT, deltaRad);
		else if (event.key.keysym.sym == SDLK_l)
			rotateCamera(cameraOri, RIGHT, -deltaRad);
		else if (event.key.keysym.sym == SDLK_u)
			rotateCamera(cameraOri, FORWARD, deltaRad);
		else if (event.key.keysym.sym == SDLK_o)
			rotateCamera(cameraOri, FORWARD, -deltaRad);
		else if (event.key.keysym.sym == SDLK_SPACE)
			lookAt(cameraPos, cameraOri, glm::vec3(0, 0, 0));
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
	std::map<string, Colour> colours = readMTL("models/cornell-box.mtl");
	vector<ModelTriangle> triangles = readObj("models/cornell-box.obj", .17, colours);

	glm::vec3 cameraPos(0, 0, 4);
	glm::mat3 cameraOri(glm::vec3(1, 0, 0),
						glm::vec3(0, 1, 0),
						glm::vec3(0, 0, 1));
	float focalLength = 150;
	int t = 0;
	// drawWireframes(triangles, focalLength, cameraPos, window);
	//drawPoints(triangles, focalLength, cameraPos, window);
	getClosestIntersection(cameraPos, glm::vec3(0, 0, 0), triangles);
	while (true)
	{
		// We MUST poll for events - otherwise the window will freeze !
		window.clearPixels();
		if (window.pollForInputEvents(event))
			handleEvent(event, window, cameraPos, cameraOri);
		//draw(triangles, focalLength, cameraPos, cameraOri, window, t);
		// Need to render the frame at the end, or nothing actually gets shown on the screen !
		window.renderFrame();
		t++;
	}
}
