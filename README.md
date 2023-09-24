# RayTracing


The goal of this assignment is to build a simple ray tracing program. It is separated into two parts, 
each with its own due date. This assignment is Part A.
 Part A (Assignment 1): You will build the ray generation and ray-object intersection 
functions in this part. Your tasks are the following.
1. Read in the input file, which contains these lines:
 Eye position: E x y z
 Output Image: O ULx ULy ULz URx URy URz LLx LLy LLz LRx LRy LRz
 Resolution: R w h
 Sphere: S Ox Oy Oz r
 Triangle: T x1 y1 z1 x2 y2 z2 x3 y3 z3
Where:
The output image is a rectangle defined by the upper-left corner (ULx, ULy, ULz), 
upper-right corner (URx, URy, URz), lower-left corner (LLx, LLy, LLz), and the 
lower-right corner (LRx, LRy, LRz).
(Ox, Oy, Oz) is the origin of a sphere with the radius = r, and
(x1, y1, z1), (x2, y2, z2), (x3, y3, z3) are the vertices of a triangle.
Note that the input file may contain multiple spheres and triangles.
2. Ray Generation: Determine the position of each screen pixel, then generate a ray 
from the eye position to each pixel.
3. Ray-object intersection: Determine whether a ray intersects the objects (which are 
the spheres and triangles defined by those lines starting with S or T in the input 
file.)
4. Output image: Draw an image of black and other color pixels, where the black 
means no intersection is found and the other color means at least one intersection 
is detected. The color represents the (x,y,z) of the intersection point, which will be 
explained next. The PPM image library is provided on the course website. You 
may also use other image format such as JPEG, PNG, or BMP if you prefer them 
to the PPM format.
5. To simplify the assignment, you may assume that all objects are within a unit 
square, which means the x, y, z values of the objects are between -1.0 and 1.0. 
Therefore, the (x,y,z) 3D coordinates may be converted to a (R,G,B) color by 
setting R=255*(x+1.0)/2.0, G=255*(y+1.0)/2.0, B=255*(z+1.0)/2.0.
These modules require many 3D vector operations. Therefore you’re advised to use a 3D 
vector or matrix library (of your own choice) or to build your own
