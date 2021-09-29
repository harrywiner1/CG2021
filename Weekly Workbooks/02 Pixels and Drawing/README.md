## Pixels and Drawing
### <a href='https://web.microsoftstream.com/group/1dcf4e99-7117-4f18-9936-c5e6712daf00?view=videos' target='_blank'> Weekly Briefing ![](../../resources/icons/briefing.png) </a>
### Task 1: Introduction


Before commencing with this week's workbook, make sure you have first watched the "Weekly Briefing" linked to above.

This week we will take a look at the fundamentals of computer graphics: namely pixels and drawing. Everything that we cover this week will be in 2 dimensions. In future weeks we will move on to begin working in 3D, it is however important that we walk before we try to run !

You will hopefully find this workbook fairly straightforward and should be able to complete it relatively quickly. This is intended to be a gentle week in order to provide some flexibility in the workload - in case you are still having difficulties installing SDL2, compiling and running your code. Next week the workbooks start to get more challenging !  


# 
### Task 2: Single Element Numerical Interpolation


Interpolation (informally: "filling in the gaps between known values") is an essential operation in computer graphics. We will use it time and time again in this unit (for various different purposes). It is essential that we become familiar with the basic principles - we are going to need to interpolate some pretty complex constructs later on !

For the time being, let us start simply: create a duplicate of the _RedNoise_ template (giving the new project a suitable name) and then add a new function called `interpolateSingleFloats` that takes 3 parameters:
- `from` - a floating point number to start from
- `to` - a floating point number to go up to
- `numberOfValues` - the number of steps required from the start to the end

This function should return an **evenly spaced** list (as a vector) of size `numberOfValues` that contains floating point numbers that start at `from` and ends at `to`. So, for example, calling: `interpolateSingleFloats(2.2, 8.5, 7)` should return a vector containing the following values: `2.2 3.25 4.3 5.35 6.4 7.45 8.5`. You might like to take a look at 
<a href="https://www.tutorialspoint.com/cpp_standard_library/cpp_vector_push_back.htm" target="_blank">this tutorial</a> on using vectors in C++ in order to get you started.

Once written, test out your function to make sure it correctly prints out the values from the above example. Do this by adding the following code to your project's main function:

``` cpp
std::vector<float> result;
result = interpolateSingleFloats(2.2, 8.5, 7);
for(size_t i=0; i<result.size(); i++) std::cout << result[i] << " ";
std::cout << std::endl;
```

Note that `std::cout` refers to the character output object of the standard library and that `std::endl` is a flushed newline. It is also worth commenting that `size_t` is an unsigned integer type provided by C++ that is useful for comparing the size of things (like vectors).  


# 
### Task 3: Single Dimension Greyscale Interpolation
 <a href='03%20Single%20Dimension%20Greyscale%20Interpolation/slides/segment-1.pdf' target='_blank'> ![](../../resources/icons/slides.png) </a> <a href='03%20Single%20Dimension%20Greyscale%20Interpolation/audio/segment-1.mp4' target='_blank'> ![](../../resources/icons/audio.png) </a>

Interpolating numbers is fine, but this unit is supposed to be about graphics - so let's draw something ! Using the floating point interpolation function that you wrote in the previous task, draw a greyscale gradient across the `DrawingWindow`, from left to right (as shown in the image below). 

For each pixel in the window, you will need to create a suitable colour by packing RGB channel data into a `uint32_t` variable. To find out more about about pixels and pixel colour packing, take a look at the slides and audio narration for this section (the blue buttons above).  


![](03%20Single%20Dimension%20Greyscale%20Interpolation/images/grey-scale.jpg)

**Hints & Tips:**  
Use the pixel looping code from the _RedNoise_ template to help get you started with this task.  
Don't worry about the transparency/alpha channel of the pixels - just set it to 255 (fully opaque) as we did in the _RedNoise_ template.  


# 
### Task 4: Three Element Numerical Interpolation


Write a new function called `interpolateThreeElementValues` that operates on three-element floating point values (rather than just single floats). These three-element floating point values should be instances of the <a href="https://en.wikibooks.org/wiki/GLSL_Programming/Vector_and_Matrix_Operations" target="_blank">vec3</a> class provided by GLM. If we have a function that can interpolate these, then we can deal with a wide range of entities including colours, 3D coordinates, directions (in fact anything with 3 numerical elements !)  

By way of example, if we have the following two `vec3` variables:
``` cpp
glm::vec3 from(1, 4, 9.2);
glm::vec3 to(4, 1, 9.8);
```
And pass them into our new interpolation function (along with a `numberOfValues` of 4), then we should get the following results back:

``` cpp
(1, 4, 9.2)
(2, 3, 9.4)
(3, 2, 9.6)
(4, 1, 9.8)
```

Write your function and then test it with the above values in order to check that it operates as intended.  


**Hints & Tips:**  
In order to use the `vec3` class, you will need to import the GLM header file with `#include <glm/glm.hpp>`  


# 
### Task 5: Two Dimensional Colour Interpolation
 <a href='05%20Two%20Dimensional%20Colour%20Interpolation/animation/segment-1.mp4' target='_blank'> ![](../../resources/icons/animation.png) </a>

In the earlier grayscale interpolation task, we were interpolating a single float value in just one dimension (the x axis). In this current task we will interpolate 3 float values (Red / Green / Blue) in _two_ dimensions (x _and_ y). Click on the blue button above to view a video showing more details about interpolating RGB colour values. Once you are confident with the concepts covered, implement the 2D RGB interpolation in order to produce the effect shown in the image below. To help you in this task, you can use the `interpolateThreeElementValues` function that you wrote in the previous task. If you are a mathematician, it might seem a bit strange to use a vector to store and manipulate a colour. But C++ programmers do a lot of unusual things with their data types, so we should probably try to get used to this kind of thing.
  


![](05%20Two%20Dimensional%20Colour%20Interpolation/images/colour-spectrum.jpg)

**Hints & Tips:**  
A good way to tackle this problem is to start by creating variables for the four corners of the window and initialising them with the following primary colours:

``` cpp
glm::vec3 topLeft(255, 0, 0);        // red 
glm::vec3 topRight(0, 0, 255);       // blue 
glm::vec3 bottomRight(0, 255, 0);    // green 
glm::vec3 bottomLeft(255, 255, 0);   // yellow
```

Then, using your previously written `vec3` interpolation function to help:
1. Calculate the colour of all the pixels in the first (left-most) column of the window
2. Calculate the colour of all the pixels in the last (right-most) column of the window
3. For each row, calculate the colour of all the pixels in that row by interpolating between the first and last pixels  


# 
End of Workbook