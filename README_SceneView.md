# SceneView

### Known Bugs
I know of no bugs in my SceneView program. However, I believe I use a smaller minimum tesselation value than the demo, so some of the shapes might look slightly different when the tesselation parameters are set to 1.

### Design Decisions
There were not too many design decisions to make for the Sceneview project. The majority of my code lies within `SceneviewScene.cpp` and the `m_phongShader` member variable is used to set the appropriate aspects of the OpenGL state prior to rendering. I do store a single instance of each primitive type, and these instances are only re-tesselated when the shape parameters change in the GUI.

### Extra Credit
I implemented an LOD strategy that uses lower tesselation parameters when there are a lot of objects in the scene. To do this I divide the shape parameters by the fourth root of the number of objects in the scene. For small scenes, this does not significantly change the tesselation parameters, while for large scenes there will be a more significant decrease in the total number of triangles to render. You can see that this strategy works by loading the chess scene and changing the shape parameters. You will see that the number of faces still grow as the shape parameters are increased, but more slowly than they would without this LOD strategy.
