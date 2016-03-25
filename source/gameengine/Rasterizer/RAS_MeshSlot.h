/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file RAS_MeshSlot.h
 *  \ingroup bgerast
 */

#ifndef __RAS_MESH_SLOT_H__
#define __RAS_MESH_SLOT_H__

#include <vector>

class RAS_MaterialBucket;
class RAS_DisplayArrayBucket;
struct DerivedMesh;
class RAS_Deformer;
class RAS_MeshObject;
class RAS_MeshUser;
class RAS_DisplayArray;
class RAS_TexVert;

class RAS_MeshSlot
{
private:
	RAS_DisplayArray *m_displayArray;

public:
	// for rendering
	RAS_MaterialBucket *m_bucket;
	RAS_DisplayArrayBucket *m_displayArrayBucket;
	RAS_MeshObject *m_mesh;
	RAS_Deformer *m_pDeformer;
	DerivedMesh *m_pDerivedMesh;
	RAS_MeshUser *m_meshUser;

	RAS_MeshSlot();
	RAS_MeshSlot(const RAS_MeshSlot& slot);
	virtual ~RAS_MeshSlot();

	void init(RAS_MaterialBucket *bucket);

	RAS_DisplayArray *GetDisplayArray();
	void SetDeformer(RAS_Deformer *deformer);
	void SetMeshUser(RAS_MeshUser *user);

	int AddVertex(const RAS_TexVert& tv);
	void AddPolygonVertex(int offset);

#ifdef WITH_CXX_GUARDEDALLOC
	MEM_CXX_CLASS_ALLOC_FUNCS("GE:RAS_MeshSlot")
#endif
};

typedef std::vector<RAS_MeshSlot *> RAS_MeshSlotList;

#endif  // __RAS_MESH_SLOT_H__