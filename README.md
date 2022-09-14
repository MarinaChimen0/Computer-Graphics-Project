# Computer Graphics Project
This project was implemented for the Computer Graphics module in the MSc in Computer Games Technology that I completed in 2020. Our task was to create 3D graphics scene taking place along a pre-programmed route, using OpenGL, C++ and GLSL with the Windows API and programming it in Visual Studio. 
My approach to this concept was to build a racing game in an underwater environment. Specifically, the player controls a submarine which mission is to follow a given route picking up the largest number of chests displayed on it, in the shortest amount of time. The route consists on a natural water canal with rock walls and a sand bottom.

The path is represented in the Game class (Game.h/Game.cpp) with the attribute m_pPath. In this class, the central line of the path is constructed in the CreateCentreline method by using a Catmull-Rom spline per each four control points. Defining splines in this way assures that they have C1 continuity, so the path conformed by them is not going to have any discontinuities and its derivatives are going to be continuous as well. To generate the path based on this centreline, two offset curves (one right and one left) are created around it in the CreateOffsetCurves method, computing a TNB frame per each centreline vertex. 

The next step is creating primitives to generate the visualisation of the path. In the CreateTrack method, the two offset curves are connected conforming triangles. For demarcating the edge of the route, the option chosen has been generating primitives that conform walls, using the methods CreateOffsetUpCurves, CreateRightWall and CreateLeftWall. Proper textures are loaded at the CreatePath and CreateWalls methods, and bound at RenderTrack and RenderWalls for rendering the primitives. 

The general shader program is the one used for rendering these components (shader.vert/shader.frag): in the day mode, the Phong shading model is used for computing the light, and in the night mode, the Blinn-Phong shading model per every spotlight. The computed colour of the light received is then combined with the texture colour.

The object that represents the camera in the Game class is m_pCamera, an object of the class CCamera (Camera.h/Camera.cpp). The following camera modes have been implemented with it in the Update method of the Game class: 

-	First person: The position of the camera is the same as the player (the submarine) position. The next point calculated for the TNB frame is used as the view vector, being added both the height of the submarine and the side displacement. The up vector computed for the TNB frame is used as the up vector of the camera.
-	Side view: The submarine position is used as the position of the camera but subtracting the normal computed in the TNB frame multiplied by a certain distance. The submarine position itself is taken as the view vector and the up vector computed for the TNB frame is used as the up vector of the camera.
-	Third person: The point in the path calculated for the TNB frame is used for the camera position but subtracting the tangent resulted and adding the binomial computed, both multiplied by different distance factors. The side displacement of the submarine is also added. For the view vector, it is added the binomial vector, multiplied by the same distance factor, to the up vector used in the TNB frame. 

To enhance the scene, we had to create some basic objects from triangle primitives. I chose creating cubes (Cube.h/Cube.cpp) to represent the chests and torus (Torus.h/Torus.cpp) as propulsion rings. In order to give them the proper position, rotation and scale, the transformations needed are applied to the matrix stack (modelViewMatrixStack) in a sort way: scaling, rotation and finally translation, which in the code, since matrices are post-multiplied, is written in the opposite order.

Different meshes, the submarine and the stones, are also loaded in the project using the COpenAssetImportMesh class. The m_pSubmarine attribute represents the submarine mesh, which is loaded in the Initialise method and then rendered in the Render method, that uses the variables m_submarinePosition and m_submarineOrientation for translating and rotating the submarine in the scene. These variables are calculated in the Update method by using a TNB frame with the distance already covered by the submarine. 

In terms of positioning the stones as obstacles, each stone model has been rendered ten times by using instanced rendering in the general vertex shader, one of the advanced rendering techniques that have been implemented in the project shaders:

-	Instanced rendering: The instanced rendering of the stones is implemented in the general vertex shader (shader.vert). The COpenAssetImportMesh class has been added a RenderInstance method similar to the Render one, but it receives a GLsizei parameter that is used when calling to glDrawElementsInstanced, instead of calling to glDrawElements. This achieves loading forty obstacles with just four calls.
-	Fog and perlin noise: In the general shader (shader.vert/shader.frag) it is implemented an exponential fog effect, in a blue colour to simulate a water environment, and a Perlin noise effect to simulate water in the surface of the canal. Once the Perlin noise functions have been implemented, they have been also used in a static way to deform the walls and the floor primitives of the route, to give them a more realistic appearance.
-	Radial blur: When the speed of the submarine is incremented a radial blur effect is applied to the scene image, in the fragment blur shader (blurShader.frag). In first place, the scene is rendered to a texture. Then radial effect is computed by forming a vector from the centre of the screen and sampling the image along this vector, creating a blurred colour. The speed of the submarine is used to increment the blur effect. 

A HUD is provided to give the user information about three aspects: the time spent, the amount of chests collected and the position of the player in the circuit. Moreover, when the player finishes the lap, the points achieved (depending on the time and the chests collected) are also displayed in the screen. The information about the time spent, the chests collected and the final punctuation, is rendered using the CFreeTypeFont class and the textShader. 

On the other side, to give information about the player location in the route, the strategy followed consisted on implementing a CCamera object in the Game class, m_pCameraTopView, and placing it in a central point of the route in a high position, to get a top view of the scene in which all the route can be visualized. Then, in the Render method, a FBO m_pFBOTopView has been used to render the scene from this camera to a texture. The RenderSceneTopView method, uses topviewShader (topviewShader.vert/ topviewShader.frag) in which only the submarine, the path and a planar terrain at the bottom are rendered.

The gameplay consists on picking up the largest number of chests displayed on the route, in the shortest amount of time. The player can control the submarine by using the WASD keys and the space bar. This is coded in the ProcessEvents method of the game class. In addition, in the Update method, the collisions with the stone obstacles and the chests are computed, by checking how much distance there is between the submarine position and them. If it is smaller than the decided collision distance, a collision takes place.
