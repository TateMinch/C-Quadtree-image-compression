#include "quadTree.h"

/**************************************************************************//**
 * This function is the constructor for the QuadTree class and initializes the 
 * width, height, previous quality factor and current quality factor. Also 
 * allocates 2d array for compressed image and calls the function to build the
 * quadtree.
 * 
 * @param[in] image - original greyscale 2d image array
 * @param[in] width - the width of the original image
 * @param[in] height - the height of the original image
 * @param[in] threshold - the threshold quality specified on command line
 *
 *****************************************************************************/
QuadTree::QuadTree(byte** image, int width, int height, int threshold)
{
   //initializes the variables
   originalWidth = width, originalHeight = height, lastQFactor = threshold, qualityFactor = threshold, nodeTotal = 1, leafCount = 1;
   //sets showLines equal to true to start the program
   showLines = true;
   //sets original image array equal to the image array passed in
   originalImage = image;

   //allocate 2d array for compressed image data
   compressedImage = new byte* [originalHeight];
   for(int j = 0; j < originalHeight; j++)
   {
      compressedImage[j] = new byte [originalWidth];
   }
   //initializes private tree root node variable to new node that represents 
   //the entire original image
   root = new qnode(0, 0, originalWidth, originalHeight);
   //call to recursive function that builds the tree, passing in the root node
   buildTreeRecurse(root);
   //call to setcompressedImage function that populates compressed image 2d array.
   setCompressedImage(root);
}

/**************************************************************************//**
 * This function is the destructor for the QuadTree class and it frees up the 
 * dynamically allocated memory created for the compressedImage data as well
 * as the dynamically allocated root node.
 *
 *****************************************************************************/
QuadTree::~QuadTree()
{
   for(int j = 0; j < originalHeight; j++)
      delete []compressedImage[j];
   
   delete []compressedImage;

   delete root;
}

/**************************************************************************//**
 * This function subdivides a given node of the tree into four further nodes.
 *
 * @param[in] newRoot - pointer to tree node
 *
 * @return - none
 *****************************************************************************/
void QuadTree::subDivide(qnode* newRoot)
{
   //Creates new nw node
   newRoot->topLeft = new qnode(newRoot->x, newRoot->y, (newRoot->width)/2, (newRoot->height)/2);
       
   //Creates new ne node
   newRoot->topRight = new qnode((newRoot->width)/2 + newRoot->x, newRoot->y, (newRoot->width)/2, (newRoot->height)/2);
   
   //Creates new sw node
   newRoot->botLeft = new qnode(newRoot->x,(newRoot->height)/2 +  newRoot->y, (newRoot->width)/2, (newRoot->height)/2);
   
   //Creates new se node
   newRoot->botRight = new qnode((newRoot->width)/2 + newRoot->x, (newRoot->height)/2 +  newRoot->y, (newRoot->width)/2, (newRoot->height)/2);  

   nodeTotal = nodeTotal + 4;

   return;
}

/**************************************************************************//**
 * This function recursively builds the tree.
 *
 * @param[in] currNode - pointer to tree node
 *
 * @returns - none
 *****************************************************************************/
void QuadTree::buildTreeRecurse(qnode *currNode)
{
   //if the node needs no further subdivision, return
   if(setNodePixel(currNode))
   {
      return;
   }

   //if root does require subdivision, subdivide using subDivide function
   subDivide(currNode);
   //call recursFunction with new nw node
   buildTreeRecurse(currNode->topLeft);
   //call recursFunction with new ne node
   buildTreeRecurse(currNode->topRight);
   //call recursFunction with new sw node
   buildTreeRecurse(currNode->botLeft);
   //call recursFunction with new se node
   buildTreeRecurse(currNode->botRight);
}

/**************************************************************************//**
 * This function evaluates whether or not the node passed in must be split
 * further.
 *
 * @param[in] currNode - pointer to tree node
 *
 * @return - False if the array must be subdivided
 * @return - True if the array values are all within the range of the thresh
 *****************************************************************************/
bool QuadTree::setNodePixel(qnode *currNode)
{
   //if the width and height are equal to 1, no further subdivision is necessary
   if (currNode->width == 1 && currNode->height == 1)
   {
      //set current node to be a leaf 
      currNode->isLeaf = true;
      //increment leafcount
      leafCount++;
      //initializes node data to cell data of original data when no subdivision
      //is necessary
      currNode->pixel = originalImage[currNode->y][currNode->x];
      //returns true
      return true;
   }
   
   //initializes variables necessary for calculations to 0 and max and min
   int average = 0, amountPixels = 0, max = 0, min = 255;

   //nested for loop - i begins at the nodes y value and goes until it is less 
   //than the nodes height plus y value
   for(int i = currNode->y; i < (currNode->height + currNode->y); i++)
      //j begins at nodes x value and goes until it is less than nodes width plus
      //nodes x value
      for(int j = currNode->x; j < (currNode->width + currNode->x); j++)
      {
         //increment average by value of array cell
         average = average + originalImage[i][j];
         //increment cell count
         amountPixels ++;

         //if cell value is greater than max
         if(originalImage[i][j] > max)
            //change max to cell value
            max = originalImage[i][j];
         //if cell value is less than min
         if(originalImage[i][j] < min)
            //set min to cell value
            min = originalImage[i][j];
      }
   
   //calculate average
   average = average / amountPixels;

   //if average - quality factor is greater than min
   //OR average plus quality factor is less than max
   if(average - qualityFactor > min || average + qualityFactor < max)
   {
      //return false
      return false;
   }
   //else, no further is necessary
   else
   {
      //set current node equal to a leaf
      currNode->isLeaf = true;
      //increment leaf count
      leafCount++;
      //initialize data to average
      currNode->pixel = average;
      //return true
      return true;
   }
}

/**************************************************************************//**
 * This function recursively fills the compressed image array with the 
 * compressed image data
 *
 * @param[in] currNode - pointer to tree node
 *
 * @return - None
 *****************************************************************************/
void QuadTree::setCompressedImage(qnode* currNode)
{
   //if showLines is true and the current node is a leaf
   if(showLines == true && currNode->isLeaf == true)
   {
      //nested for loop, i is equal to the roots y and goes till it is not less
      //than the roots height plus y value
      for(int i = currNode->y; i < (currNode->height + currNode->y); i++)
      //j is equal to the roots x coordinate and goes until it is not less than 
      //the roots width plus the x value
         for(int j = currNode->x; j < (currNode->width + currNode->x); j++)
         {
            //fill cells with the roots data
            compressedImage[i][j] = currNode->pixel;
            //fill the boundaries with white pixels to create lines
            if(i == 0 || j == 0 || i == (currNode->height + currNode->y) - 1 || j == (currNode->width + currNode->x) - 1)
               compressedImage[i][j] = 255;
         }
      //return
      return;
   }
   //else if showLines is equal to false and the current node is a leaf 
   else if(showLines == false && currNode->isLeaf == true)
   {
      //nested for loop, i is equal to the roots y and goes till it is not less
      //than the roots height plus y value
      for(int i = currNode->y; i < (currNode->height + currNode->y); i++)
      //j is equal to the roots x coordinate and goes until it is not less than 
      //the roots width plus x
         for(int j = currNode->x; j < (currNode->width + currNode->x); j++)
            //fill cells with the roots data
            compressedImage[i][j] = currNode->pixel;
      //return
      return;
   }
   //call function again with nw node
   setCompressedImage(currNode->topLeft);
   //call function again with ne node
   setCompressedImage(currNode->topRight);
   //call function again with sw node
   setCompressedImage(currNode->botLeft);
   //call function again with se node
   setCompressedImage(currNode->botRight);
}

/**************************************************************************//**
 * This function sets the intensity of the quality factor
 *
 * @param[in] threshold - current quality factor
 *
 * @return - None
 *****************************************************************************/
void QuadTree::setIntensity(int threshold)
{
   //initialize nodeTotal and leafCount to 0
   nodeTotal = 0;
   leafCount = 0;
   //add threshold to quality factor
   qualityFactor = qualityFactor + threshold;
   //if it is less than or equal to 0, make it 1
   if (qualityFactor <= 0)
      qualityFactor = 1;
   //if the last quality factor does not equal the current one
   if(lastQFactor != qualityFactor)
   {
      //delete root and rebuild tree
      delete root;
      //initializes private tree root node variable to new node that represents 
      //the entire original image
      root = new qnode(0, 0, originalWidth, originalHeight);
      //call to recursive function that builds the tree, passing in the root node
      buildTreeRecurse(root);
      //call to setcompressedImage function that populates compressed image 2d array.
      setCompressedImage(root);  
   }

   //set last quality factor equal to quality factor
   lastQFactor=qualityFactor;
   //return
   return;
}

/**************************************************************************//**
 * This function turns on or off the lines in the program
 *
 *
 * @return - None
 *****************************************************************************/
void QuadTree::setLines()
{
   //if showLines is true
   if(showLines == true)
   //set it equal to false
      showLines = false;
   //else
   else
      //set it equal to true
      showLines = true;
   
   //set compressed image
   setCompressedImage(root);
   return;
}

/**************************************************************************//**
 * This function returns the compressed image 2d array with all the data
 *
 * @return - compressedImage
 *****************************************************************************/
byte** QuadTree::getCompressedImage()
{
   return compressedImage;
}

/**************************************************************************//**
 * This function outputs the statistics for the program
 *
 *
 * @return - None
 *****************************************************************************/
void QuadTree::getStats()
{
std::cout << "-----------------------------------------------" << std::endl;
//outputs the number of nodes
std::cout << "Number of nodes: " << nodeTotal << std::endl;
//outputs the number of leaf nodes
std::cout << "Number of leaf nodes: " << leafCount << std::endl;
//outputs the compression ration
std::cout << "Estimated uncrompressed size: " << originalWidth * originalHeight << std::endl;
std::cout << "Estimated compressed size: " << leafCount * 2 << std::endl;
std::cout << "Compression Ratio: " << getCompressionRatio() << std::endl;
std::cout << "-----------------------------------------------" << std::endl;
//returns
return;
}

/**************************************************************************//**
 * This function calculates and returns the compression ratio
 *
 *
 * @return - None
 *****************************************************************************/
float QuadTree::getCompressionRatio()
{
   //return 100 times 2 times the leaf count divided by the node total squared
   return 100 * ((2 * (float)leafCount) / ((float)originalWidth*(float)originalHeight));
}
