/*
*@author Scott Mogul
*Home Work 1
*9/4/12
*
*
*/


#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CatPictureApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

private:
	Surface* mySurface_; //The Surface object whose pixel array we will modify
	static const int Width=800; //width of visible window
	static const int Height=600; //height of visible window
	static const int Size=1024; //surface size

	int frameNumber;

	
	void drawRect(int x1, int x2, int y1, int y2, uint8_t* pixArray);
	void drawCircle(int x, int y, int r, uint8_t* pixArray);
	void copy(int x1, int x2, int y1, int y2, int shiftH, uint8_t* pixArray);
	void tint(Color8u tintAmt, uint8_t* pixArray);
	void blur(uint8_t* pixArray);
};


/*
*Take in two x coordinates and two y coordinates and compute the width and height between them
*loops over the values in between and colors them red
*
*/
		
void CatPictureApp::drawRect(int x1, int x2, int y1, int y2, uint8_t* pixArray){
	int width = x2-x1;
	int height = y2-y1;
	for(int i=y1;i<height;i++)
	{
		for(int j=x1;j<width;j++)
		{
			pixArray[3*(j+i*Size)] = 255;
			pixArray[3*(j+i*Size)+1] = 2;
			pixArray[3*(j+i*Size)+2] = 50;
		}
	}

}

/*
*@peram int x is the x coordinate for the center of the cirlce
*@peram int y is the y coordinate for the center of the circle
*@peram uint8_t* the pixel array to be modified
*
*/
void CatPictureApp::drawCircle(int x, int y, int r, uint8_t* pixArray){
	for(int i=y-r;i<y+r;i++)
	{
		for(int j=x-r;j<x+r;j++)
		{
			int distance = (int)sqrt((double)((j-x)*(j-x)+(i-y)*(i-y)));
				if(distance<=r)
				{
					pixArray[3*(j+i*Size)] = 0;
					pixArray[3*(j+i*Size)+1] = 255;
					pixArray[3*(j+i*Size)+2] = 50;
				}
		}
	}

}

/*
*@peram x1 the start x coordinate
*@peram x2 the end x coordinate
*@peram y1 the start y coordinate
*@peram y2 the end y coordinate
*@peram shiftH the amount to be shifted
*@peram uint8_t* pixArray pixel array to be modified
*
*/
void CatPictureApp::copy(int x1, int x2, int y1, int y2, int shiftH, uint8_t* pixArray){
	int width = x2-x1;
	int height = y2-y1;
	for(int i=y1;i<height;i++)
	{
		for(int j=x1;j<width;j++)
		{
			pixArray[3*((j)+(i+shiftH)*Size)] = pixArray[3*(j+i*Size)];
			pixArray[3*((j)+(i+shiftH)*Size)+1]=pixArray[3*(j+i*Size)+1];
			pixArray[3*((j)+(i+shiftH)*Size)+2]=pixArray[3*(j+i*Size)+2];
		}
	}

}

/*
*@peram tintAmt the amount of tint to be applied to the red pixel
*@peram uint8_t* pixArray pixel array to be modified
*
*/
void CatPictureApp::tint(Color8u tintAmt, uint8_t* pixArray){
	for(int i=0;i<Height;i++)
	{
		for(int j=0;j<Width;j++)
		{	
			pixArray[3*(j+i*Size)] = tintAmt.r;
		}
	}

}

/*
*@peram uint8_t* pixArray pixel array to be modified
*
*/
void CatPictureApp::blur(uint8_t* pixArray){
	

	for(int i=0;i<Width;i++)
	{
		for(int j=0;j<Height;j++)
		{
			pixArray[3*(i + j*Size)] = (pixArray[3*(i + j*Size)] + pixArray[3*((i+1) + j*Size)] + pixArray[3*((i+1) + (j+1)*Size)] + pixArray[3*((i) + (j+1)*Size)] + pixArray[3*((i-1) + (j+1)*Size)] + pixArray[3*((i-1) + (j-1)*Size)] + pixArray[3*((i) + (j-1)*Size)])/8;
			pixArray[3*(i + j*Size)+1] = (pixArray[3*(i + j*Size)+1] + pixArray[3*((i+1) + j*Size)+1] + pixArray[3*((i+1) + (j+1)*Size)+1] + pixArray[3*((i) + (j+1)*Size)+1] + pixArray[3*((i-1) + (j+1)*Size)+1] + pixArray[3*((i-1) + (j-1)*Size)+1] + pixArray[3*((i) + (j-1)*Size)+1])/8;
			pixArray[3*(i + j*Size)+2] = (pixArray[3*(i + j*Size)+2] + pixArray[3*((i+1) + j*Size)+2] + pixArray[3*((i+1) + (j+1)*Size)+2] + pixArray[3*((i) + (j+1)*Size)+2] + pixArray[3*((i-1) + (j+1)*Size)+2] + pixArray[3*((i-1) + (j-1)*Size)+2] + pixArray[3*((i) + (j-1)*Size)+2])/8;
		}
	}

}

/*
*initalizes the surface and the frameNumber variable
*
*/
void CatPictureApp::setup()
{
	mySurface_ = new Surface(Size,Size,false);
	frameNumber=0;
	
}

void CatPictureApp::mouseDown( MouseEvent event )
{
}

/*
*updates the surface with a rectangle, circle, copy, tint and blur
*also writes the first frame of the image to a png file
*
*/
void CatPictureApp::update()
{
	uint8_t* pixArray = (*mySurface_).getData();
	drawRect(5,200,5,200,pixArray);
	drawCircle(400,400,50,pixArray);
	copy(20,400,50,300,100,pixArray);
	tint(Color8u(50,10,205),pixArray);
	blur(pixArray);

	if(frameNumber == 0){
		writeImage("ScottsImage.png",*mySurface_);
	}
	frameNumber++;
}
/*
*draws the surface
*/
void CatPictureApp::draw()
{
	gl::draw(*mySurface_);
}

CINDER_APP_BASIC( CatPictureApp, RendererGl )
