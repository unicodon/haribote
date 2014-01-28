#ifndef DRAW_PRIMITIVE_H
#define DRAW_PRIMITIVE_H

void DrawSphere(float radius, bool bDrawWire = false);
void DrawCube(float length, bool bDrawWire = false);
void DrawBox(float cx, float cy, float cz, bool bDrawWire = false);
void DrawBoxRev(float cx, float cy, float cz, bool bDrawWire = false);
void DrawCone(float radius, float height, bool bDrawWire = false);
void DrawOctahedron(float length, bool bDrawWire = false);
void DrawCylinder(float radius, float length, bool bDrawWire = false);

#endif  //DRAW_PRIMITIVE_H
