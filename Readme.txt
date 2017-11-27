The main part of this code example is the ConvexPolygon class, which contains a
few interesting functions, most notably:

  - ConvexPolygon::setConvexHull - a function which sets a ConvexPolygon to the
    convex hull of a set of ponts.

  - ConvexPolygon::overlaps - a function which determines whether two convex
    polygons overlap. This is implemented using a rotating calipers algorithm.