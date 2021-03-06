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
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file RAS_Texture.h
 *  \ingroup ketsji
 */

#ifndef __RAS_TEXTURE_H__
#define __RAS_TEXTURE_H__

#include "STR_String.h"

struct MTex;
struct Image;

class RAS_Texture
{
protected:
	int m_bindCode;
	STR_String m_name;

public:
	RAS_Texture();
	virtual ~RAS_Texture();

	virtual bool Ok() = 0;

	virtual MTex *GetMTex() = 0;
	virtual Image *GetImage() = 0;
	STR_String& GetName();

	virtual unsigned int GetTextureType() const = 0;

	/// Return GL_TEXTURE_2D
	static int GetCubeMapTextureType();
	/// Return GL_TEXTURE_CUBE_MAP
	static int GetTexture2DType();

	// Check if bindcode is an existing texture
	static bool CheckBindCode(int bindcode);

	enum {MaxUnits = 8};

	virtual void ActivateTexture(int unit) = 0;
	virtual void DisableTexture() = 0;

	int GetBindCode() const;
	void SetBindCode(int bindcode);
};

#endif // __RAS_TEXTURE_H__
