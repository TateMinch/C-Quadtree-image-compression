#ifndef APPLICATION_H
#define APPLICATION_H
#include "olcPixelGameEngine.h"
#include "readPNG.h"
#include "quadTree.h"

class Application : public olc::PixelGameEngine
{
   const char *filename;
   olc::Pixel **image;
   byte **greyScale;
   byte **compressedImage;
   unsigned width, height;
   int threshold;
   QuadTree *tree;

public:
   Application(const char *, int);

public:
   bool OnUserCreate() override;
   bool OnUserUpdate(float) override;
   bool OnUserDestroy() override;
   bool showLines;

   int Width() const;
   int Height() const;
};

#endif


