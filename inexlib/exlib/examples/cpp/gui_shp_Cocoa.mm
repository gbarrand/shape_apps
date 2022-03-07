// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file exlib.license for terms.

//exlib_build_use exlib inlib freetype inlib_glutess AGL Cocoa
//exlib_build_use png jpeg zlib
//exlib_build_use shapelib

//exlib_build_darwin_app
//exlib_build_resfile ./res/*.jpg
//exlib_build_resfile ./res/*.ttf
//exlib_build_resfile ./res/*.otf
//exlib_build_resfile ../../data/ne_10m_coastline.shp
//exlib_build_resfile ../../data/ne_10m_coastline.shx

#include <exlib/Cocoa/gui_viewer.hm>

#define EXLIB_SCREEN_MGR Cocoa

#include "gui_shp_common.icc"
