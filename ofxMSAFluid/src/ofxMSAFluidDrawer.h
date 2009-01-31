/***********************************************************************
 
 This class is for drawing a fluidsolver using the OpenFrameworks texture
 
 -----------------------------------
 
 Copyright (c) 2009, Memo Akten, www.memo.tv
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 ***********************************************************************/


#pragma once


#include "ofMain.h"
#include "ofxMSAFluidSolver.h"

#define FLUID_DRAW_COLOR		0
#define FLUID_DRAW_MOTION		1
#define FLUID_DRAW_SPEED		2
#define FLUID_DRAW_VECTORS		3

#define FLUID_DEFAULT_SIZE		-1

//#define FLUID_TEXTURE


class ofxMSAFluidDrawer {
public:
	float alpha;
	
	ofxMSAFluidDrawer();
	virtual ~ofxMSAFluidDrawer();
	
	ofxMSAFluidSolver* setup(int NX = FLUID_DEFAULT_NX, int NY = FLUID_DEFAULT_NY);
	ofxMSAFluidSolver* setFluidSolver(ofxMSAFluidSolver* f);
	ofxMSAFluidSolver* getFluidSolver();
	
	void update();
	virtual void draw(float x = 0, float y = 0, float renderWidth = FLUID_DEFAULT_SIZE, float renderHeight = FLUID_DEFAULT_SIZE);	// this one does chooses one of the below based on drawmode
	void drawColor(float x = 0, float y = 0, float renderWidth = FLUID_DEFAULT_SIZE, float renderHeight = FLUID_DEFAULT_SIZE);		// if width or height is less than 0, use actual size
	void drawMotion(float x = 0, float y = 0, float renderWidth = FLUID_DEFAULT_SIZE, float renderHeight = FLUID_DEFAULT_SIZE);		// if width or height is less than 0, use actual size
	void drawSpeed(float x = 0, float y = 0, float renderWidth = FLUID_DEFAULT_SIZE, float renderHeight = FLUID_DEFAULT_SIZE);		// if width or height is less than 0, use actual size
	void drawVectors(float x = 0, float y = 0, float renderWidth = FLUID_DEFAULT_SIZE, float renderHeight = FLUID_DEFAULT_SIZE) {}	// TODO
	void reset();
	
	void setDrawMode(int newDrawMode);
	
protected:	
	unsigned char		*_pixels;						// pixels array to be drawn
	int					_byteCount;						// number of byes in the pixel array (size * 3)
	int					_drawMode;
#ifdef FLUID_TEXTURE
	ofTexture			tex;
#endif	
	
	ofxMSAFluidSolver	*_fluidSolver;
	bool				_didICreateTheFluid;
	
	virtual void		createTexture();
	
	void deleteFluidSolver();
	bool isFluidReady();
	
};