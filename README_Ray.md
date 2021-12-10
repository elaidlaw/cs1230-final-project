### Directional Lights, Specular Highlights, and Attenuation
These additions were fairly straightforward in terms of implementation. I simply modified my `computeLighting()` function in `RayScene.cpp` to account for the new effects.

### Texture Mapping
For texture mapping, it was first necessary to map the surface of each primitive shape to a UV grid. I added a `getUV()` method to each of my implicit shape classes, which takes in a point in object space and outputs it's UV value. The UV value was then used to access the texture color, which was integrated into the lighting computation.

### Shadows
For shadows, I added a method called `shadowRay()` in `RayScene.cpp` which takes a point, direction, and light depth and determines whether any objects in the scene occlude the light from the point. Then, when I am iterating over lights in the lighting computation, I simply the diffuse and specular contributions of lights that are occluded.

### Reflections
For reflections, I made very few changes to my code. I have a function in `RayScene.cpp` called `rayLighting()` which takes a ray, finds any intersections, and then computes the lighting. This function now takes in an integer recursion depth as well. The lighting function calls this function again for the reflected ray, but increments the depth. Eventually, when the depth is greater then one of the class member variables, `m_maxrecursiondepth`, no light is returned. This member variable can be changed in the RayScene constructor.
### Design Decisions


### Known Issues
- My program crashes when trying to render a new .xml after having already rendered an .xml (On Ed post #419 is says this is a bug in the stencil that won't be penalized)
- The GUI buttons controlling whether shadows, reflections, etc. are handled do not work. I wasn't sure if these were necessary or not
