# ***** BEGIN GPL LICENSE BLOCK *****
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# The Original Code is Copyright (C) 2006, Blender Foundation
# All rights reserved.
#
# The Original Code is: all of this file.
#
# Contributor(s): Jacques Beaurain.
#
# ***** END GPL LICENSE BLOCK *****

set(INC
	.
	../Expressions
	../Ketsji
	../SceneGraph
	../GameLogic
	../../blender/makesdna
	../../blender/blenlib
	../../blender/blenkernel
	../../blender/imbuf
	../../../intern/glew-mx
	../../../intern/guardedalloc
	../../../intern/string
)

set(INC_SYS
	../../../intern/moto/include
	${GLEW_INCLUDE_PATH}
	${PYTHON_INCLUDE_DIRS}
	${BOOST_INCLUDE_DIR}
)

set(SRC
	RAS_2DFilterManager.cpp
	RAS_BucketManager.cpp
	RAS_DisplayArray.cpp
	RAS_DisplayArrayBucket.cpp
	RAS_FramingManager.cpp
	RAS_IPolygonMaterial.cpp
	RAS_InstancingBuffer.cpp
	RAS_MaterialBucket.cpp
	RAS_MeshObject.cpp
	RAS_MeshSlot.cpp
	RAS_MeshUser.cpp
	RAS_Polygon.cpp
	RAS_Shader.cpp
	RAS_Texture.cpp
	RAS_TexVert.cpp
	RAS_ICanvas.cpp
	RAS_2DFilterData.cpp
	RAS_2DFilter.cpp
 
	RAS_2DFilterData.h
	RAS_2DFilter.h
	RAS_2DFilterManager.h
	RAS_BucketManager.h
	RAS_CameraData.h
	RAS_Deformer.h
	RAS_DisplayArray.h
	RAS_DisplayArrayBucket.h
	RAS_FramingManager.h
	RAS_ICanvas.h
	RAS_IPolygonMaterial.h
	RAS_IRasterizer.h
	RAS_IStorage.h
	RAS_ILightObject.h
	RAS_InstancingBuffer.h
	RAS_MaterialBucket.h
	RAS_MeshMaterial.h
	RAS_MeshObject.h
	RAS_MeshSlot.h
	RAS_MeshUser.h
	RAS_Polygon.h
	RAS_Rect.h
	RAS_Shader.h
	RAS_Texture.h
	RAS_TexVert.h
	RAS_OpenGLFilters/RAS_Blur2DFilter.h
	RAS_OpenGLFilters/RAS_Dilation2DFilter.h
	RAS_OpenGLFilters/RAS_Erosion2DFilter.h
	RAS_OpenGLFilters/RAS_GrayScale2DFilter.h
	RAS_OpenGLFilters/RAS_Invert2DFilter.h
	RAS_OpenGLFilters/RAS_Laplacian2DFilter.h
	RAS_OpenGLFilters/RAS_Prewitt2DFilter.h
	RAS_OpenGLFilters/RAS_Sepia2DFilter.h
	RAS_OpenGLFilters/RAS_Sharpen2DFilter.h
	RAS_OpenGLFilters/RAS_Sobel2DFilter.h
	RAS_OpenGLFilters/RAS_VertexShader2DFilter.h
)

add_definitions(${GL_DEFINITIONS})

blender_add_lib(ge_rasterizer "${SRC}" "${INC}" "${INC_SYS}")
