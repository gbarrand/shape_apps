// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file exlib.license for terms.

//exlib_build_use inlib exlib shapelib

// example of usage :
//   UNIX> <rshp exe> ../../data/ne_10m_land.shp

//#define USE_DBMALLOC

//#include <shapefil.h>
#include <exlib/shapelib>

#include <cstdio>
#include <cstdlib>

int main(int argc,char** argv) {
  if(argc<2) {
    ::printf( "Give a .shp file as first argument.\n");
    ::printf( "For example: ../../data/ne_10m_coastline.shp\n");
    ::exit( 1 );
  }
  
  SHPHandle hSHP = ::SHPOpen(argv[1],"rb");
  if(hSHP==NULL) {
    ::printf( "Unable to open:%s\n", argv[1]);
    ::exit( 1 );
  }

  // Print out the file bounds :
  int nShapeType, nEntities;
  double adfMinBound[4], adfMaxBound[4];
  ::SHPGetInfo( hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound );

  ::printf( "Shapefile Type: %s   # of Shapes: %d\n\n",::SHPTypeName( nShapeType ), nEntities );

  int nPrecision = 15;
  ::printf( "File Bounds: (%.*g,%.*g,%.*g,%.*g)\n"
            "         to  (%.*g,%.*g,%.*g,%.*g)\n",
            nPrecision, adfMinBound[0],
            nPrecision, adfMinBound[1],
            nPrecision, adfMinBound[2],
            nPrecision, adfMinBound[3],
            nPrecision, adfMaxBound[0],
            nPrecision, adfMaxBound[1],
            nPrecision, adfMaxBound[2],
            nPrecision, adfMaxBound[3] );

  // Skim over the list of shapes, printing all the vertices :
  int bHeaderOnly = 0;
  int bValidate = 0;
  int nInvalidCount = 0;
  for(int i = 0; i < nEntities && !bHeaderOnly; i++ ) {
    SHPObject* psShape = ::SHPReadObject(hSHP,i);
    if(psShape==NULL) {
      ::fprintf(stderr,"Unable to read shape %d, terminating object reading.\n",i);
      break;
    }

    if( psShape->bMeasureIsUsed )
      ::printf( "\nShape:%d (%s)  nVertices=%d, nParts=%d\n"
                    "  Bounds:(%.*g,%.*g, %.*g, %.*g)\n"
                    "      to (%.*g,%.*g, %.*g, %.*g)\n",
                    i, ::SHPTypeName(psShape->nSHPType),
                    psShape->nVertices, psShape->nParts,
                    nPrecision, psShape->dfXMin,
                    nPrecision, psShape->dfYMin,
                    nPrecision, psShape->dfZMin,
                    nPrecision, psShape->dfMMin,
                    nPrecision, psShape->dfXMax,
                    nPrecision, psShape->dfYMax,
                    nPrecision, psShape->dfZMax,
                    nPrecision, psShape->dfMMax );
    else
      ::printf( "\nShape:%d (%s)  nVertices=%d, nParts=%d\n"
                    "  Bounds:(%.*g,%.*g, %.*g)\n"
                    "      to (%.*g,%.*g, %.*g)\n",
                    i, ::SHPTypeName(psShape->nSHPType),
                    psShape->nVertices, psShape->nParts,
                    nPrecision, psShape->dfXMin,
                    nPrecision, psShape->dfYMin,
                    nPrecision, psShape->dfZMin,
                    nPrecision, psShape->dfXMax,
                    nPrecision, psShape->dfYMax,
                    nPrecision, psShape->dfZMax );

    if( psShape->nParts > 0 && psShape->panPartStart[0] != 0 ) {
      ::fprintf( stderr, "panPartStart[0] = %d, not zero as expected.\n",psShape->panPartStart[0]);
    }

    int j,iPart;
    for(j = 0,iPart = 1; j < psShape->nVertices; j++ ) {
      const char* pszPartType = "";

      if( j == 0 && (psShape->nParts > 0) ) pszPartType = ::SHPPartTypeName( psShape->panPartType[0] );

      const char* pszPlus;
      if( (iPart < psShape->nParts) && (psShape->panPartStart[iPart] == j) ) {
        pszPartType = ::SHPPartTypeName( psShape->panPartType[iPart] );
        iPart++;
        pszPlus = "+";
      } else {
        pszPlus = " ";
      }
      /*
      if( psShape->bMeasureIsUsed )
        ::printf("   %s (%.*g,%.*g, %.*g, %.*g) %s \n",
                       pszPlus,
                       nPrecision, psShape->padfX[j],
                       nPrecision, psShape->padfY[j],
                       nPrecision, psShape->padfZ[j],
                       nPrecision, psShape->padfM[j],
                       pszPartType );
      else
        ::printf("   %s (%.*g,%.*g, %.*g) %s \n",
                       pszPlus,
                       nPrecision, psShape->padfX[j],
                       nPrecision, psShape->padfY[j],
                       nPrecision, psShape->padfZ[j],
                       pszPartType );
      */

    }

    if(bValidate) {
      int nAltered = ::SHPRewindObject(hSHP,psShape);
      if(nAltered>0) {
        ::printf( "  %d rings wound in the wrong direction.\n",nAltered);
        nInvalidCount++;
      }
    }

    ::SHPDestroyObject( psShape );
  }

  ::SHPClose(hSHP);

  if(bValidate) {
    ::printf( "%d object has invalid ring orderings.\n",nInvalidCount);
  }

#ifdef USE_DBMALLOC
  ::malloc_dump(2);
#endif

  ::exit(0);
}
