/*

    This file is part of libncf.

    declspec.h
    MS compiler symbol exporting.

    Copyright (C) 2008, 2010 - 2012
    Papadopoulos Nikolaos

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General
    Public License along with this library; if not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301 USA

*/

#ifndef NCF_DECLSPEC_H_INCLUDED
#define NCF_DECLSPEC_H_INCLUDED

#ifdef _MSC_VER
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>

	#ifdef _WINDLL
		#define DECLSPEC __declspec(dllexport)
	#else
		#define DECLSPEC __declspec(dllimport)
	#endif

#else
	#define DECLSPEC

#endif

#endif /* NCF_DECLSPEC_H_INCLUDED */
