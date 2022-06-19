#ifndef CONE_H
#define CONE_H

#include "model.h"

namespace Models{
    namespace ConeInternal{
        extern float vertices[];
		extern float normals[];
		extern float vertexNormals[];
		extern float texCoords[];
		extern float colors[];
		extern unsigned int vertexCount;
    }
    class Cone: public Model{
        public:
            Cone();
            virtual ~Cone();
            virtual void drawSolid(bool smooth=false);
    };
    
    extern Cone cone;
}

#endif // CONE_H