# Shape

### Known Bugs
I know of no bugs in my program

### Design Decisions
Each shape is implemented as it's own class that inherits from the OpenGLShape base class. This base class defines a few virtual functions that are useful for handling all shapes, such as 
* `GenVertexData()` which computes the vertices and normals of the shape
* `update()` which updates the shapes vertex list
* `setParam1()` which allows the first tessalation parameter to be updated
* `setParam2()` which allows the second tessalation parameter to be updated


Within ShapesScene a list of shared pointers of each shape type is maintened, and the m_shape variable is assigned to a different element of the list whenever the shape type changes. 
In order to share common functionality between shapes, I created a utilies.cpp class that contains helper functions to generate the vertices of shape components like different faces.
