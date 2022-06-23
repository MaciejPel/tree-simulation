
#include "leaf.h"

namespace Models{
    Leaf leaf;

    Leaf::Leaf(){
        vertices = LeafInternal::vertices;
		vertexCount = LeafInternal::vertexCount;
		// normals = ConeInternal::normals;
		// texCoords = ConeInternal::texCoords;
    }

    Leaf::~Leaf(){
    }

    void Leaf::drawSolid(bool smooth) {
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(0,4,GL_FLOAT,false,0,vertices);
        glVertexAttribPointer(1,4,GL_FLOAT,false,0,normals);
        glVertexAttribPointer(2,4,GL_FLOAT,false,0,texCoords);

        glDrawArrays(GL_TRIANGLES,0,vertexCount);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);
    }

    namespace LeafInternal{
		unsigned int vertexCount = 6;

        float vertices[] = {
            0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f, 1.0f,

            1.0f, 0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
        };
    }
}


