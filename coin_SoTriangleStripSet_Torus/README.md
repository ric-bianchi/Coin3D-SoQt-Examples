# Build a Torus shape

The example shows how to visualize a custom shape.

In particular, we define the visualization of a Torus shape, which is not part of the built-in shapes provided by Coin/OpenInventor.

![](assets/README-a43d3802.png)


## Purpose

We want to define a torus in a quite general way, to be able to build:
- a complete torus, filled or hollow
- a complete toroidal surface
- a toroidal segment, filled or as a "pipe"
- a toroidal polyhedron, with a given number of faces in the two angular directions ("poloidal" and "toroidal")

The shape is defined by several parameters:

 - a major radius, `rMajor`, which defines the main radius of the circular shape
 ![](assets/README-99a46011.png)
 - a minor radius, `rMinor`, which defines the radius of the cross section of the torus, i.e., the outer radius of the toroidal shape:
 - an inner radius, `rInner`, which defines the inner radius of the cavity if the torus is hollow. The value of this variable deeply affects the nature and the aspect of the torus:
   - if `rInner` is equal to `-1`, then the torus is made of a toroidal surface only, with no thickness, as shown in the figure below:
   ![toroidal surface only](assets/README-ec67fe9e.png)
   - if `rInner` is equal to `0`, then the torus becomes a "solid torus", whose internal volume is completely filled, as shown in the figure below:
   ![](assets/README-407b4ca2.png)
   - if `rInner` is greater than `0`, then the torus becomes a "pipe": it is hollow, with `rInner` defining the radius of the inner cavity; this is shown in the figure below:
   ![](assets/README-54c66f3d.png)

- a starting angle `SPhi`, in degrees, which defines the starting angle of a toroidal segment
- a angular interval `DPhi`, in degrees, which defines the length of the toroidal segment in the toroidal angular direction. In the figure below, the toroidal segment is defined with a starting angle `SPhi` of 0 and an angular length of 270 degrees:
![](assets/README-ad3db86f.png)

## Constructors

The shape is defined in a class, the `MyTorus`, which exposes a constructor featuring both required and optional arguments:

```cpp
MyTorus(double rMajor, double rMinor, double rInner=-1, double SPhi=0/*degrees*/, double DPhi=360/*degrees*/, int divsMajor=70, int divsMinor=40);
```

Among them, the **REQUIRED**  arguments are:

- rMajor, the major radius of the toroidal shape
- rMinor, the radius of the tube

These additional arguments are **OPTIONAL**:

- rInner, the inner radius of the cavity if the torus is hollow; i.e., the inner radius of the "pipe"
- `SPhi`, the starting angle of the toroidal segment, in degrees
- `DPhi`, the angular length of the toroidal segment, in degrees
- `divsMajor`, the number of faces used to render the toroidal shape along the toroidal length (the "major" length)
- `divsMinor`, the number of faces used to render the toroidal shape along the poloidal length (the "minor" length)


There is also a constructor taking no arguments to build a simple example torus.
