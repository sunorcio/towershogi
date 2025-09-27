#ifndef MESH_H
#define MESH_H




static float vertexPrimitive[] = 
	{ -0.5, -0.5,  0. ,      0.5, -0.5,  0. ,      0. ,  0.5,  0.  };
static unsigned short elementPrimitive[] = {0, 1 ,2};


static float vertexCube[] = {
	-1., -1.,  1.,     1., -1.,  1.,    -1.,  1.,  1.,     1.,  1.,  1.,
	-1., -1., -1.,     1., -1., -1.,    -1.,  1., -1.,     1.,  1., -1.,
};
static unsigned short elementCube[] = {
	0, 1, 3,    0, 3, 2,
	1, 5, 7,    1, 7, 3,
	3, 7, 6,    3, 6, 2,
	7, 5, 4,    7, 4, 6,
	6, 4, 0,    6, 0, 2,
	4, 5, 1,    4, 1, 0,
};




#endif
