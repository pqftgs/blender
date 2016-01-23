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

/** \file gameengine/Rasterizer/RAS_DisplayArrayBucket.cpp
 *  \ingroup bgerast
 */

#include "RAS_DisplayArrayBucket.h"
#include "RAS_DisplayArray.h"
#include "RAS_MaterialBucket.h"
#include "RAS_IPolygonMaterial.h"
#include "RAS_MeshObject.h"
#include "RAS_Deformer.h"
#include "RAS_IRasterizer.h"
#include "RAS_IStorage.h"
#include "KX_PythonInit.h"
#include "KX_KetsjiEngine.h"
#include "glew-mx.h"

#include <algorithm>

#ifdef _MSC_VER
#  pragma warning (disable:4786)
#endif

#ifdef WIN32
#  include <windows.h>
#endif // WIN32

RAS_DisplayArrayBucket::RAS_DisplayArrayBucket(RAS_MaterialBucket *bucket, RAS_DisplayArray *array, RAS_MeshObject *mesh)
	:m_refcount(1),
	m_bucket(bucket),
	m_displayArray(array),
	m_mesh(mesh),
	m_useDisplayList(false),
	m_meshModified(false),
	m_storageInfo(NULL)
{
	m_bucket->AddDisplayArrayBucket(this);
	glGenBuffersARB(1, &m_vbo);
}

RAS_DisplayArrayBucket::~RAS_DisplayArrayBucket()
{
	m_bucket->RemoveDisplayArrayBucket(this);
	DestructStorageInfo();

	if (m_displayArray) {
		delete m_displayArray;
	}
}

RAS_DisplayArrayBucket *RAS_DisplayArrayBucket::AddRef()
{
	++m_refcount;
	return this;
}

RAS_DisplayArrayBucket *RAS_DisplayArrayBucket::Release()
{
	--m_refcount;
	if (m_refcount == 0) {
		delete this;
		return NULL;
	}
	return this;
}

unsigned int RAS_DisplayArrayBucket::GetRefCount() const
{
	return m_refcount;
}

RAS_DisplayArrayBucket *RAS_DisplayArrayBucket::GetReplica()
{
	RAS_DisplayArrayBucket *replica = new RAS_DisplayArrayBucket(*this);
	replica->ProcessReplica();
	return replica;
}

void RAS_DisplayArrayBucket::ProcessReplica()
{
	m_refcount = 1;
	m_activeMeshSlots.clear();
	if (m_displayArray) {
		m_displayArray = new RAS_DisplayArray(*m_displayArray);
	}
	m_bucket->AddDisplayArrayBucket(this);
}

RAS_DisplayArray *RAS_DisplayArrayBucket::GetDisplayArray() const
{
	return m_displayArray;
}

RAS_MaterialBucket *RAS_DisplayArrayBucket::GetMaterialBucket() const
{
	return m_bucket;
}

void RAS_DisplayArrayBucket::ActivateMesh(RAS_MeshSlot *slot)
{
	m_activeMeshSlots.push_back(slot);
}

RAS_MeshSlotList& RAS_DisplayArrayBucket::GetActiveMeshSlots()
{
	return m_activeMeshSlots;
}

void RAS_DisplayArrayBucket::RemoveActiveMeshSlots()
{
	m_activeMeshSlots.clear();
}

unsigned int RAS_DisplayArrayBucket::GetNumActiveMeshSlots() const
{
	return m_activeMeshSlots.size();
}

void RAS_DisplayArrayBucket::AddDeformer(RAS_Deformer *deformer)
{
	m_deformerList.push_back(deformer);
}

void RAS_DisplayArrayBucket::RemoveDeformer(RAS_Deformer *deformer)
{
	RAS_DeformerList::iterator it = std::find(m_deformerList.begin(), m_deformerList.end(), deformer);
	if (it != m_deformerList.end()) {
		m_deformerList.erase(it);
	}
}

bool RAS_DisplayArrayBucket::UseDisplayList() const
{
	return m_useDisplayList;
}

bool RAS_DisplayArrayBucket::IsMeshModified() const
{
	return m_meshModified;
}

void RAS_DisplayArrayBucket::UpdateActiveMeshSlots(RAS_IRasterizer *rasty)
{
	// Reset values to default.
	m_useDisplayList = true;
	m_meshModified = false;

	RAS_IPolyMaterial *material = m_bucket->GetPolyMaterial();

	if (!rasty->UseDisplayLists()) {
		m_useDisplayList = false;
	}
	else if (m_bucket->IsZSort()) {
		m_useDisplayList = false;
	}
	else if (material->UsesObjectColor() || material->UseInstancing()) {
		m_useDisplayList = false;
	}
	// No display array mean modifiers.
	else if (!m_displayArray) {
		m_useDisplayList = false;
	}

	for (RAS_DeformerList::iterator it = m_deformerList.begin(), end = m_deformerList.end(); it != end; ++it) {
		RAS_Deformer *deformer = *it;

		deformer->Apply(material);

		// Test if one of deformers is dynamic.
		if (deformer->IsDynamic()) {
			m_useDisplayList = false;
			m_meshModified = true;
		}
	}

	if (m_mesh->GetModifiedFlag() & RAS_MeshObject::MESH_MODIFIED) {
		m_meshModified = true;
	}

	// Set the storage info modified if the mesh is modified.
	if (m_storageInfo) {
		m_storageInfo->SetMeshModified(rasty->GetDrawingMode(), m_meshModified);
	}
}

void RAS_DisplayArrayBucket::SetMeshUnmodified()
{
	m_mesh->SetModifiedFlag(0);
}

RAS_IStorageInfo *RAS_DisplayArrayBucket::GetStorageInfo() const
{
	return m_storageInfo;
}

void RAS_DisplayArrayBucket::SetStorageInfo(RAS_IStorageInfo *info)
{
	m_storageInfo = info;
}

void RAS_DisplayArrayBucket::DestructStorageInfo()
{
	if (m_storageInfo) {
		delete m_storageInfo;
		m_storageInfo = NULL;
	}
}

void RAS_DisplayArrayBucket::RenderMeshSlots(const MT_Transform& cameratrans, RAS_IRasterizer *rasty)
{
	if (m_activeMeshSlots.size() == 0) {
		return;
	}

	// Update deformer and render settings.
	UpdateActiveMeshSlots(rasty);

	rasty->BindPrimitives(this);

	for (RAS_MeshSlotList::iterator it = m_activeMeshSlots.begin(), end = m_activeMeshSlots.end(); it != end; ++it) {
		RAS_MeshSlot *ms = *it;
		m_bucket->RenderMeshSlot(cameratrans, rasty, ms);
	}

	rasty->UnbindPrimitives(this);
}

void RAS_DisplayArrayBucket::UpdateActiveMeshSlotsInstancingBuffer(RAS_IRasterizer *rasty, RAS_DisplayArrayBucket::InstancingObject *buffer)
{
	RAS_IPolyMaterial *material = m_bucket->GetPolyMaterial();
	for (unsigned int i = 0, size = m_activeMeshSlots.size(); i < size; ++i) {
		RAS_MeshSlot *ms = m_activeMeshSlots[i];
		InstancingObject& data = buffer[i];
		float mat[16];
		rasty->SetClientObject(ms->m_clientObj);
		rasty->GetTransform(ms->m_OpenGLMatrix, material->GetDrawingMode(), mat);
		data.matrix[0] = mat[0];
		data.matrix[1] = mat[4];
		data.matrix[2] = mat[8];
		data.matrix[3] = mat[1];
		data.matrix[4] = mat[5];
		data.matrix[5] = mat[9];
		data.matrix[6] = mat[2];
		data.matrix[7] = mat[6];
		data.matrix[8] = mat[10];
		data.position[0] = mat[12];
		data.position[1] = mat[13];
		data.position[2] = mat[14];

		data.color[0] = ms->m_RGBAcolor[0] * 255.0f;
		data.color[1] = ms->m_RGBAcolor[1] * 255.0f;
		data.color[2] = ms->m_RGBAcolor[2] * 255.0f;
		data.color[3] = ms->m_RGBAcolor[3] * 255.0f;
	}
}

#define USE_BUFFER
void RAS_DisplayArrayBucket::RenderMeshSlotsInstancing(const MT_Transform& cameratrans, RAS_IRasterizer *rasty)
{
	if (m_activeMeshSlots.size() == 0) {
		return;
	}

	RAS_IPolyMaterial *material = m_bucket->GetPolyMaterial();

	// Update deformer and render settings.
	UpdateActiveMeshSlots(rasty);

// 	material->ActivateMeshSlot(m_activeMeshSlots[0], rasty);

	double time = KX_GetActiveEngine()->GetRealTime();
#ifdef USE_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(InstancingObject) * m_activeMeshSlots.size(), 0, GL_DYNAMIC_DRAW);
	InstancingObject *buffer = (InstancingObject *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
#else
	InstancingObject buffer[m_activeMeshSlots.size()];
#endif

	UpdateActiveMeshSlotsInstancingBuffer(rasty, buffer);
	glUnmapBuffer(GL_ARRAY_BUFFER);
// 	std::cout << "pack data take : " << KX_GetActiveEngine()->GetRealTime() - time << ", num instances : " << m_activeMeshSlots.size() << std::endl;
	material->ActivateInstancing(
#ifdef USE_BUFFER
		(void *)((InstancingObject *)NULL)->matrix,
		(void *)((InstancingObject *)NULL)->position,
		(void *)((InstancingObject *)NULL)->color,
#else
		(void *)buffer->matrix,
		(void *)buffer->position,
		(void *)buffer->color,
#endif
		sizeof(InstancingObject));

	time = KX_GetActiveEngine()->GetRealTime();
#ifdef USE_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
	rasty->BindPrimitives(this);
	rasty->IndexPrimitivesInstancing(this);
	material->DesactivateInstancing();
	rasty->UnbindPrimitives(this);
// 	std::cout << "draw take : " << KX_GetActiveEngine()->GetRealTime() - time << std::endl;
}

