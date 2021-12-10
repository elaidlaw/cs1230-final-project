### Design Decisions
To represent shapes, I created a new ImplicitShape class. Each instance of this class has methods that get the t value of an intersection with a particular ray, and the normal of any point on the surface.
There are subclasses of ImplicitShape that represent shape parts (e.g. cylinder cap, sphere surface, cube face). The shapes themselves are implemented as another subclass of ImplicitShape called ImplicitSurfaceAggregation that acts as a collection of object surfaces.
Most of the rest of the logic exists in RayScene. RayScene maintains a collection of ImplicitShapes. It also has methods to produce rays in Camera Space and World Space. 
A further RayScene method computes the t value of the first object intersection. Finally, there is a lighting function in RayScene that considers scene parameters, surface normals, and scene lighting.

### Known Issues
- My program crashes when trying to render a new .xml after having already rendered an .xml (On Ed post #419 is says this is a bug in the stencil that won't be penalized)
- It takes a long time to render scenes with many objects like the chessboard. I don't think this is a bug, just raytracing being raytracing :)
