#include <CanvasTriangle.h>
#include <DrawingWindow.h>
#include <Utils.h>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>

#define WIDTH 2388
#define HEIGHT 1512

using namespace std;

vector<float> interpolateSingleFloats(float x, float y, int n)
{
	vector<float> result;
	float delta = (y - x) / (n - 1);
	for (int i = 0; i < n; i++)
	{
		result.push_back(x + (i * delta));
	}
	return result;
}

vector<glm::vec3> interpolate3D(glm::vec3 from, glm::vec3 to, int n) {
	vector<float> a = interpolateSingleFloats(from[0], to[0], n);
	vector<float> b = interpolateSingleFloats(from[1], to[1], n);
	vector<float> c = interpolateSingleFloats(from[2], to[2], n);
	vector<glm::vec3> result;
	for (int i = 0; i <= n; i++) {
		result.push_back(glm::vec3(a[i], b[i], c[i]));
	}
	return result;
}

void drawGreyscale(DrawingWindow &window) {
	window.clearPixels();
	vector<float> colors = interpolateSingleFloats(0, 255, WIDTH);
	for (size_t y = 0; y < window.height; y++) {
		for (size_t x = 0; x < window.width; x++) {
			uint32_t color = (255 << 24) + (int(colors[x]) << 16) +  (int(colors[x]) << 8) + (int(colors[x]));
			window.setPixelColour(x, y, color);
		}
	}
}

void drawColors(DrawingWindow &window) {
	window.clearPixels();
	vector<glm::vec3> lhs = interpolate3D(glm::vec3(204,186,174), glm::vec3(193,72,470), window.height);
	vector<glm::vec3> rhs = interpolate3D(glm::vec3(193,72,47), glm::vec3(204,186,174), window.height);
	for (size_t y = 0; y < window.height; y++) {
		vector<glm::vec3> across = interpolate3D(lhs[y], rhs[y], window.width);
		for (size_t x = 0; x < window.width; x++) {
			glm::vec3 value = across[x];
			uint32_t color = (255 << 24) + (int(value[0]) << 16) +  (int(value[1]) << 8) + (int(value[2]));
			window.setPixelColour(x, y, color);
		}
	}
}

void handleEvent(SDL_Event event, DrawingWindow &window) {
if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_LEFT) std::cout << "LEFT" << std::endl;
		else if (event.key.keysym.sym == SDLK_RIGHT) std::cout << "RIGHT" << std::endl;
		else if (event.key.keysym.sym == SDLK_UP) std::cout << "UP" << std::endl;
		else if (event.key.keysym.sym == SDLK_DOWN) std::cout << "DOWN" << std::endl;
	} else if (event.type == SDL_MOUSEBUTTONDOWN) {
		window.savePPM("output.ppm");
		window.saveBMP("output.bmp");
	}
}

int main()
{
	DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
	SDL_Event event;
	while (true) {
		if (window.pollForInputEvents(event)) handleEvent(event, window);
		drawColors(window);
		window.renderFrame();
	}

	std::vector<float> result;
	result = interpolateSingleFloats(2.2, 8.5, 7);
	for(size_t i=0; i<result.size(); i++) std::cout << result[i] << " ";
	std::cout << std::endl;
	glm::vec3 from(1, 4, 9.2);
	glm::vec3 to(4, 1, 9.8);
	vector<glm::vec3> result2 = interpolate3D(from, to, 4);
	for(size_t i=0; i<result2.size(); i++) {
		std::cout << result2[i][0] << " ";
		std::cout << result2[i][1] << " ";
		std::cout << result2[i][2] << " ";
		std::cout << std::endl;
	}
}
