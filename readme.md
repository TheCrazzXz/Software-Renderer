# Patch note

## 17 of May, 2023 : V1 :
Featuring XYZ space to screen space (normalized device coordinates)  
Bugs : Something should be done fore the camera angles  
Bugs : Bunny (?)  
Bugs : Sutherland-Hodgeman clipping should be added to support "rendering vertices behind camera" but i'll do all that later

## 22 of May, 2023 : V2 :
Objects behind camera can still exist
Fixed thing for camera angles
Still crashes sometimes...

## 22 of May, 2023 : V2 + (10 at the power -1)
Added line clipping, so only portions of lines which are in the screen are processed, fixed the crash problem with this
%1

## 26 of May, 2023 : V2.5 (this is a software renderer without opengl nor directx or vulkan, [JOKE MODE ON] it's poser sh*t [JOKE MODE OFF]
- Added line clipping in window.
- Added support for rendering data with shaders.
- Fixed graphical problems / crashing problems (a lot).
- Now support for filling triangles ! (this was the hardest thing to implement lmao).
Use shSetWireframeMode(true) before rendering anything to enable wireframe (not filling triangles).

In engine (nothing to do with graphical library, i've created it just to test) :
- Entities are added, so instances of objects.
You can set objects position, rotation, scale, and two colors
Colors will be interpolated...
But : Note that for now if you don't use uniform colors, so different entity.start_color and entity.end_color,
The program might slow, mostly if you get closer to objects.

## 5th of June V2.6 : A lot of updates and optimization

- Finished with clipping problems
- Now really supports triangle filling, optimized by using memset to draw horizontal lines
- Improved interpolation (this example doesn't use it)

You can now draw objects anywhere on the space and fill them, it should work
