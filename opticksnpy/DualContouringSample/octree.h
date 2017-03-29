/*

Implementations of Octree member functions.

Copyright (C) 2011  Tao Ju

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License
(LGPL) as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#pragma once

#include <functional>

#include "qef.h"
#include "mesh.h"

#include <glm/glm.hpp>
using glm::vec3;
using glm::vec4;
using glm::ivec3;

#include "NQuad.hpp"
#include "NBBox.hpp"



class Timer ; 
class Constructor ; 
struct NFieldGrid3 ; 

// ----------------------------------------------------------------------------

enum OctreeNodeType
{
	Node_None,
	Node_Internal,
	Node_Psuedo,
	Node_Leaf,
};

// ----------------------------------------------------------------------------

struct OctreeDrawInfo 
{
	OctreeDrawInfo()
		: index(-1)
		, corners(0)
	{
	}

	int				index;
	int				corners;
	vec3			position;
	vec3			averageNormal;
	svd::QefData	qef;
};

// ----------------------------------------------------------------------------

class OctreeNode
{
public:

	OctreeNode()
		: type(Node_None)
		, min(0, 0, 0)
		, size(0)
		, drawInfo(nullptr)
	{
		for (int i = 0; i < 8; i++)
		{
			children[i] = nullptr;
		}
	}

	OctreeNode(const OctreeNodeType _type)
		: type(_type)
		, min(0, 0, 0)
		, size(0)
		, drawInfo(nullptr)
	{
		for (int i = 0; i < 8; i++)
		{
			children[i] = nullptr;
		}
	}



	OctreeNodeType	type;
	ivec3			min;
	int				size;
	OctreeNode*		children[8];
	OctreeDrawInfo*	drawInfo;
};



inline void DestroyOctree(OctreeNode* node)
{
	if (!node) return ;
	for (int i = 0; i < 8; i++) DestroyOctree(node->children[i]);
	delete node->drawInfo;
	delete node;
}




inline bool operator == ( const OctreeNode& a, const OctreeNode& b)
{
    return a.type == b.type && 
           a.size == b.size &&
           a.min == b.min  ;
}



class Manager 
{
    public:
        enum { 
               BUILD_BOTTOM_UP = 0x1 << 0, 
               BUILD_TOP_DOWN  = 0x1 << 1,
               USE_BOTTOM_UP   = 0x1 << 2, 
               USE_TOP_DOWN    = 0x1 << 3, 
               BUILD_BOTH      = BUILD_BOTTOM_UP | BUILD_TOP_DOWN
             };
    public:
        Manager(const unsigned ctrl, const int nominal, const int coarse, const int verbosity, const float threshold, NFieldGrid3* fieldgrid, const nbbox& bb, Timer* timer);

        void buildOctree();
        void generateMeshFromOctree(VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer);

    private:
        unsigned m_ctrl ; 
        int      m_nominal_size ; 
        int      m_verbosity ; 
        float    m_threshold ;
        NFieldGrid3* m_fieldgrid ; 
        nbbox    m_bb ; 
        Timer*   m_timer ;    

        nvec4        m_ce ; 
        Constructor* m_ctor ; 

        OctreeNode*  m_bottom_up ;         
        OctreeNode*  m_top_down ;         
        OctreeNode*  m_raw ;         
        OctreeNode*  m_simplified ;         

};


