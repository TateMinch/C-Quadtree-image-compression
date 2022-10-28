#ifndef QUADTREE_H
#define QUADTREE_H
#include <iostream>

typedef unsigned char byte;

class QuadTree
{
   //Structure for quadTree nodes

   struct qnode
   {
      //x,y,width,height variables for tree node
      int x, y, width, height;
      //data value for tree node
      int pixel;
      //pointers to four subdivisions of a tree node
      qnode* topLeft = nullptr;
      qnode* topRight = nullptr;
      qnode* botLeft = nullptr;
      qnode* botRight = nullptr;
      bool isLeaf = false;
      
      qnode(int xVal, int yVal, int widthVal, int heightVal){   
         //sets root x and y coordinates to first element in original array
         x = xVal, y = yVal;
         //sets root width and height variables equal to original image w and h
         width = widthVal, height = heightVal;}
      ~qnode(){delete topLeft; delete topRight; delete botLeft; delete botRight;}
   };

   public:
   //constructor
   QuadTree(byte** image, int totWidth, int totHeight, int thresh);
   //Destructor
   ~QuadTree();

   //Subdivide function - breaks the image array into 4 more nodes
   void subDivide(qnode* newRoot);
   //recursive function to build the tree
   void buildTreeRecurse(qnode *root);

   bool setNodePixel(qnode* n);
   void setIntensity(int threshold);
   void setCompressedImage(qnode* root);
   void setLines();

   byte** getCompressedImage();
   void getStats();
   float getCompressionRatio();

private:

   int originalWidth;
   int originalHeight;
   int lastQFactor;
   int qualityFactor;
   int nodeTotal;
   int leafCount;
   qnode* root;
   byte** originalImage;
   byte** compressedImage;
   bool showLines;
};


#endif // !QUADTREE_H


