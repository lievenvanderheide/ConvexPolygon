The main part of this code example is the ConvexPolygon class, which contains a
few interesting functions, most notably:

  - ConvexPolygon::setConvexHull - a function which sets a ConvexPolygon to the
    convex hull of a set of ponts.

  - ConvexPolygon::overlaps - a function which determines whether two convex
    polygons overlap. This is implemented using a rotating calipers algorithm.
  
The Visual Studio project contains a very crude Win32 application which
demonstrates this functionality. To use it, try the following:

  - Add red points by left clicking. You will see that the convex hull of the
    red points will be computed as soon as there are at least 3 red points.
	
  - Add green points by right clicking. Again, the convex hull of the green
    points will appear as soon as there are at least 3 green points.
	
  - When both convex hulls are present, a line of text will appear in the top
    left corner, which says either "Overlap" or "No Overlap", depending on
	whether the convex polygons formed by the two convex hulls overlap.
	
  - Use the "Open" menu to open the example point sets.