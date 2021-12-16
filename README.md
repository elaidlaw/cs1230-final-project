# Hou a city is Laid

We built a procedural city generator.

### How to run

First, edit citytile.cpp line 105 to be "/path/to/projectroot/tiles/" + ...

Then build and run using Qt Creator.

To generate new tiles, use voronoi.py in the project root. After it finishes generating, type a file name into the command prompt and it will save the tile file there.

### Design details

The city is broken into tiles that are dynamically loaded and unloaded based on the camera's position. Each tile that loads uses a pregenerated
template that's generated using voronoi.py. To generate tiles, we begin by selecting a number of points that will be the "centers" of cells in a Voronoi
diagram. A Voronoi diagram is a diagram that divides into cells where each cell is all of the points that are closer to the "center" of that cell than any others.
We generate a Voronoi diagram to represent the "major roads" in our city.

To actually construct the Voronoi diagram, we first construct its dual graph, the Delaunay triangulation. We do this by first naively triangulating the space
and then using a flip algorithm to iteratively produce a Delaunay triangulation. Fromt there we can use the perpendicular bisectors of each edge in the triangulation
to find the edges of the Voronoi diagram.

Once we have the Voronoi diagram, we fill in each cell with a grid of streets that has a random angle and spacing on each axis. We then fill each grid square with
buildings. We start by adding a building at the center of each tile and then recursively subdivide that building in two a random number of times.

The heights of the buildings are generated using Perlin noise. The noise function takes as input the world position of the building, so even though the same tile
template gets used multiple times, the buildings will be different heights and the tiles will look different.

To color the buildings, we have four different distributions: one for concrete, one for brick, one for glass, and one for brownstone. Based on the height of each building
we vary the probability by which we draw a color from each of the distributions. For instance, if the building is tall, we'll be more likely to draw from the concrete or glass
distribution than if the building is short.

To navigate the city, we've implemented a random camera trajectory that uses Bezier curves to remain smooth as it travels. We also added atmospheric perspective in the form of
a "fog" that blends distant buildings into the sky.
