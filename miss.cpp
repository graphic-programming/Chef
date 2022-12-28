/*
   Class Name:

      CMiss

   Description:

      draw mouse
*/

#include "stdafx.h"
#include "miss.h"


// constructor
CMiss::CMiss()
{
    id1               = 0;
    coord_per_vertex  = 3;                                     // x, y, z
	coord_per_texture = 2;                                     // s, t
	attrib_per_vertex = coord_per_vertex + coord_per_texture;
    stride            = attrib_per_vertex * sizeof(float);     // in bytes
	vertex_offset     = 0;                                     // x y z s t
	texture_offset    = coord_per_vertex * sizeof(float);      // 0 1 2 3 4

	vertex_count      = quad_vertex_count = 4;

	pos    = 0;
	offset = 0;

	value = 0;
}

// destructor
CMiss::~CMiss()
{
}

//
void CMiss::GenerateVertices(float *buffer, int *k, float x, float y, float z, float px1, float py1, float px2, float py2, int width, int height)
{
	int i;
	float x1, y1, x2, y2;
	float s1, t1, s2, t2;
	float cx, cy;

	s1 = px1 / width;
	s2 = px2 / width;

	t1 = py2 / height;
	t2 = py1 / height;

	cx = px2 - px1 + 1.0f;
	cy = py2 - py1 + 1.0f;

	x1 = x;
	y1 = y;
	x2 = x1 + cx;
	y2 = y1 + cy;

    //        0            1
    //     y2 +------------+
    //        |            |
    //        |            |
    //        |            |
    //        |            |
    //        |            |
    //        |            |
    //     y1 +------------+
	//        x1           x2
    //        3            2
	//       
	i = *k;

    // vertex 0
   buffer[i++] = x1;
   buffer[i++] = y2;
   buffer[i++] = z;
   buffer[i++] = s1;
   buffer[i++] = t2;

    // vertex 1
   buffer[i++] = x2;
   buffer[i++] = y2;
   buffer[i++] = z;
   buffer[i++] = s2;
   buffer[i++] = t2;

    // vertex 2
   buffer[i++] = x2;
   buffer[i++] = y1;
   buffer[i++] = z;
   buffer[i++] = s2;
   buffer[i++] = t1;

    // vertex 3
   buffer[i++] = x1;
   buffer[i++] = y1;
   buffer[i++] = z;
   buffer[i++] = s1;
   buffer[i++] = t1;

   *k = i;
}

// create vertex buffer object
void CMiss::Create(float z, int width, int height)
{
    float* vertex  = NULL;
	int i;
	float X1, X2, X3, X4;
	float Y1, Y2, Y3, Y4;

	image_count = 4;
	vertex_size = image_count * vertex_count * attrib_per_vertex;
	vertex   = new float[vertex_size];

	X1 = 291.0f;
	X2 = 329.0f;

	Y1 = 218.0f;
	Y2 = 230.0f;

	X3 = 317.0;
	X4 = 350.0;

	Y3 = 228.0f;
	Y4 = 255.0f;

	i = 0;

	//      X1    X2 
	//   Y1 +-----+
	//      |     |
	//   Y2 +-----+

	//
	//      X3    X4
	//   Y3 +-----+
	//      |     |
	//   Y4 +-----+
	//
	GenerateVertices(vertex, &i, 137, 157, z, X1, Y1, X2, Y2, width, height);
	GenerateVertices(vertex, &i, 162, 132, z, X3, Y3, X4, Y4, width, height);
	GenerateVertices(vertex, &i, 134, 132, z, X3, Y3, X4, Y4, width, height);
	GenerateVertices(vertex, &i, 106, 132, z, X3, Y3, X4, Y4, width, height);

	// for vertex  vertex
    glGenBuffersARB(1, &id1);                                                                // generate a vertex object
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, id1);                                               // bind the vertex object
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertex_size * sizeof(float), vertex, GL_STATIC_DRAW_ARB);  // copy vertex  data to the vertex object

    // release memory
    if(vertex != NULL) delete[] vertex;
}

// delete buffer object
void CMiss::Destroy()
{
    glDeleteBuffersARB(1, &id1);
}

//
void CMiss::Draw(float *matrix)
{
	GLint loc_m;

		// bind the buffer object
	    glBindBufferARB(GL_ARRAY_BUFFER_ARB, id1);

		// send projection matrix to a vertex shader
	    loc_m = glGetUniformLocationARB(program, "m_matrix");
	    glUniformMatrix4fvARB(loc_m, 1, false, matrix);

		// send vertices to a vertex shader
	    glVertexAttribPointerARB(LOC_VERTEX, coord_per_vertex, GL_FLOAT, GL_FALSE, stride, (GLvoid*)vertex_offset);

		// send texture to a vertex shader
	    glVertexAttribPointerARB(LOC_TEXTURE, coord_per_texture, GL_FLOAT, GL_FALSE, stride, (GLvoid*)texture_offset);

		// draw texture
	    glEnableVertexAttribArrayARB(LOC_VERTEX);
		glEnableVertexAttribArrayARB(LOC_TEXTURE);
		glDrawArrays(GL_QUADS, offset, vertex_count);
		glDisableVertexAttribArrayARB(LOC_TEXTURE);
	    glDisableVertexAttribArrayARB(LOC_VERTEX);

		// unbind buffer object
	    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

// set shader to be used
void CMiss::SetShader(GLhandleARB handle)
{
	    program = handle;
}

// Overloaded = sign
void CMiss::operator=(int num)
{
	value = num;
	vertex_count = value * quad_vertex_count;
}

// Define prefix increment operator.  
CMiss& CMiss::operator++()  
{  
	if(value == 0) value++;

   value++;
   vertex_count = value * quad_vertex_count;

   return *this;  
}  
  
// Define postfix increment operator.  
CMiss CMiss::operator++(int)  
{  
   CMiss temp = *this;  
   ++*this;
   return temp;  
}  
