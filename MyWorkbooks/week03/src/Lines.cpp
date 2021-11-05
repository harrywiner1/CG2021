#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <CanvasPoint.h>
#include <Colour.h>

#define WIDTH 600
#define HEIGHT 400
#define WHITE 4294967295;

using namespace std;

// Question: how to import color

uint32_t colourToCode(Colour colour)
{
	return (255 << 24) + (colour.blue << 16) + (colour.green << 8) + (colour.red);
}

vector<float> increment1D(float x, float y)
{
	vector<float> result;
	for (int i = 0; i < (y - x); i++)
	{
		result.push_back(x + i);
	}
	return result;
}

vector<CanvasPoint> interpolate2D(CanvasPoint start, CanvasPoint end)
{

	float xDiff = end.x - start.x;
	float yDiff = end.y - start.y;
	float nSteps = max(abs(xDiff), abs(yDiff));

	float dX = xDiff / nSteps;
	float dY = yDiff / nSteps;

	vector<CanvasPoint> result;

	for (float i = 0.0; i < nSteps; i++)
	{
		float x = start.x + (dX * i);
		float y = start.y + (dY * i);

		result.push_back(CanvasPoint(x, y));
	}

	return result;
}
// pushes high values to the back
// bubble sort [1,2,3]
vector<float> sort3(vector<float> a)
{
	if (a[0] > a[1])
		swap(a[0], a[1]);
	if (a[0] > a[2])
		swap(a[0], a[2]);
	if (a[1] > a[2])
		swap(a[1], a[2]);
	return a;
}

vector<CanvasPoint> sortPointsByY(CanvasPoint a, CanvasPoint b, CanvasPoint c)
{
	vector<CanvasPoint> ps = {a, b, c};

	if (ps[0].y > ps[1].y)
		swap(ps[0], ps[1]);
	if (ps[0].y > ps[2].y)
		swap(ps[0], ps[2]);
	if (ps[1].y > ps[2].y)
		swap(ps[1], ps[2]);
	return ps;
}

float findAtY(vector<CanvasPoint> line, float height)
{
	for (int i = 0; i < line.size(); i++)
	{
		if (round(line[i].y) == round(height))
		{
			return line[i].x;
		}
	}
	std::
			cout
		<< "finAtY didn't return";
	return -1;
}

void printCPVector(vector<CanvasPoint> cps)
{
	for (int i = 0; i < cps.size(); i++)
	{
		std::cout << "(" << cps[i].x << "," << cps[i].y << "), ";
	}
	std::cout << std::endl;
}

void fillTriangle(DrawingWindow &window, CanvasPoint a, CanvasPoint b, CanvasPoint c, Colour colour)
{

	uint32_t colourCode = colourToCode(colour);

	// find middle point and save fourth point that makes flat line
	vector<float> ys = {a.y, b.y, c.y};
	ys = sort3(ys);
	vector<float> xs = {a.x, b.x, c.x};
	xs = sort3(xs);

	vector<CanvasPoint> ps = sortPointsByY(a, b, c);

	// iterate from the top to the bottom of the triangle
	// if above the middle value, then treat like one triangle, otherwise like the other

	vector<float>
		ySpectrum = increment1D(ys[0], ys[2]); // vector from the top to the bottom

	vector<CanvasPoint> lineAD = interpolate2D(ps[0], ps[2]);

	vector<CanvasPoint> lineA;
	vector<CanvasPoint> lineD;
	CanvasPoint middle;

	if (ps[0].y == ps[1].y || ps[1].y == ps[1].y)
	{
		lineA = lineAD;
		lineD = lineAD;
	}
	else
	{
		for (int i = 0; i < lineAD.size(); i++)
		{
			if (round(lineAD[i].y) == round(ps[1].y))
			{
				middle = lineAD[i];

				lineA = interpolate2D(ps[0], middle);
				lineD = interpolate2D(middle, ps[2]);
			}
		}
	}

	vector<CanvasPoint> lineB = interpolate2D(ps[0], ps[1]);
	vector<CanvasPoint> lineC = interpolate2D(ps[1], ps[2]);

	std::cout << "ySpectrum: ";
	for (int h = 0; h < ySpectrum.size(); h++)
	{
		std::cout << ySpectrum[h] << ",";
		float xA;
		float xB;
		if (ySpectrum[h] < ys[1] && lineA.size() > 0 && lineB.size() > 0)
		{ // if we are above the middle
			// is the pixel in the triangle
			xA = findAtY(lineB, ySpectrum[h]);
			xB = findAtY(lineA, ySpectrum[h]);
		}
		else
		{
			xA = findAtY(lineC, ySpectrum[h]);
			xB = findAtY(lineD, ySpectrum[h]);
		}
		for (int w = 0; w < window.width; w++)
		{
			if ((w >= xA && w <= xB) || (w <= xA && w >= xB))
			{
				window.setPixelColour(round(w), round(ySpectrum[h]), colourCode);
			}
		}
	}
}

void drawLine(DrawingWindow &window, CanvasPoint start, CanvasPoint end, Colour colour)
{

	uint32_t colourCode = colourToCode(colour);

	vector<CanvasPoint> line = interpolate2D(start, end);
	for (int i = 0; i < line.size(); i++)
	{
		window.setPixelColour(round(line[i].x), round(line[i].y), colourCode);
	}
}

void drawTriangle(DrawingWindow &window, CanvasPoint a, CanvasPoint b, CanvasPoint c, Colour colour)
{
	drawLine(window, a, b, colour);
	drawLine(window, b, c, colour);
	drawLine(window, c, a, colour);
}

void randomTriangle(DrawingWindow &window)
{
	CanvasPoint a(rand() % window.width, rand() % window.height);
	CanvasPoint b(rand() % window.width, rand() % window.height);
	CanvasPoint c(rand() % window.width, rand() % window.height);

	Colour random(rand() % 256, rand() % 256, rand() % 256);
	drawTriangle(window, a, b, c, random);
	fillTriangle(window, a, b, c, random);
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
		else if (event.key.keysym.sym == SDLK_u)
			randomTriangle(window);
		else if (event.key.keysym.sym == SDLK_c)
			window.clearPixels();
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}

int main()
{
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	CanvasPoint topLeft(0.0, 0.0);
	CanvasPoint middle(window.width / 2, window.height / 2);

	fillTriangle(window, CanvasPoint(round(window.width / 2), 390), CanvasPoint(10, round(window.height / 2)), CanvasPoint(round(window.width / 2), round(window.height / 2)), Colour(255, 255, 255));
	while (true)
	{
		if (window.pollForInputEvents(event))
			handleEvent(event, window);
		//drawLine(window, topLeft, middle, Colour(255, 0, 0));
		window.renderFrame();
	}
}
