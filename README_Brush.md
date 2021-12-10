# Brush

### Known Bugs
I know of no bugs in my assignment.

### Extra Credit
I fixed the alpha blending for extra credit. This can be toggled by using the "No Alpha Blending" checkbox. 
My implementation differs from the demo, but matches the description given in the handout. Specifically, when the "No Alpha Blending" option is enabled, each time paint is put down, the alpha value is compared with previous values at that pixel during the current brush stroke, and the pixel is only updated if the new alpha/masking value is greater. This is what is described in the handout, whereas the demo appears to just cap the total alpha value when the checkbox is selected.

### Design Decisions
There weren't too many critical design decisions to be made since I closely followed the stencil code. I implemented all of the brushes as subclasses of the Brush class. Within these classes I implemented `makeMask()`, the purely virtual Brush method. Additionally, for the SmudgeBrush class I overrode the `brushDown()` and `brushDragged()` methods since it also needed to sample paint from the canvas. 

While implementing the alpha blending extra credit, I added a member variable vector to the Brush class. This vector stores the canvas data when the brush is put down, so that the brush can continue to be mixed with the original canvas instead of being added on top of other paint from the same stroke.
