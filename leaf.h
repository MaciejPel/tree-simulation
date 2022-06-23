#ifndef LEAF_H
#define LEAF_H

#include "model.h"

namespace Models{
    namespace LeafInternal{
        extern float vertices[];
		extern float normals[];
		extern float vertexNormals[];
		extern float texCoords[];
		extern float colors[];
		extern unsigned int vertexCount;
    }
    class Leaf: public Model{
        public:
            Leaf();
            virtual ~Leaf();
            virtual void drawSolid(bool smooth=false);
    };
    
    extern Leaf leaf;
}

#endif // LEAF_H