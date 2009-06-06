/*
 *  ofFBOTexture.cpp
 *  openFrameworks
 *
 *  Created by Zach Gage on 3/28/08.
 *  Copyright 2008 STFJ.NET. All rights reserved.
 *
 */

#include "ofFBOTexture.h"

void ofFBOTexture::allocate(int w, int h, bool autoClear) {
	_isActive = false;

    texData.width = w;
	texData.height = h;
	
	imageSaver.setUseTexture(false);
	imageSaver.allocate(texData.width, texData.height, OF_IMAGE_COLOR_ALPHA);
	
    if (GLEE_ARB_texture_rectangle){
        texData.tex_w = w;
        texData.tex_h = h;
        texData.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    } else {
        texData.tex_w = ofNextPow2(w);
        texData.tex_h = ofNextPow2(h);
    }
	
	if (GLEE_ARB_texture_rectangle){
		texData.tex_t = w;
		texData.tex_u = h;
	} else {
		texData.tex_t = 1.0f;
		texData.tex_u = 1.0f;
	}
	
	// attempt to free the previous bound texture, if we can:
	clean();
	
	texData.width = w;
	texData.height = h;
	texData.bFlipTexture = true;
	texData.glType = GL_RGBA;
	
	this->autoClear = autoClear;
	// create & setup FBO
	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	
	// Create the render buffer for depth
	glGenRenderbuffersEXT(1, &depthBuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, texData.tex_w, texData.tex_h);
	
	// create & setup texture
	glGenTextures(1, (GLuint *)texData.textureName);   // could be more then one, but for now, just one
	glBindTexture(texData.textureTarget, (GLuint)texData.textureName[0]);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(texData.textureTarget, 0, texData.glType, texData.tex_w, texData.tex_h, 0, texData.glType, GL_UNSIGNED_BYTE, 0);	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	// attach it to the FBO so we can render to it
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, texData.textureTarget, (GLuint)texData.textureName[0], 0);
	
	
	// Attach the depth render buffer to the FBO as it's depth attachment
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);
	
	
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT) {
		cout<<"glBufferTexture failed to initialize. Perhaps your graphics card doesnt support the framebuffer extension? If you are running osx prior to system 10.5, that could be the cause"<<endl;
		std::exit(1);
	}
	clear();
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}


void ofFBOTexture::saveImage(string fileName) {
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadPixels(0, 0, texData.width, texData.height, texData.glType, GL_UNSIGNED_BYTE, imageSaver.getPixels());
	imageSaver.saveThreaded(fileName);
}


void ofFBOTexture::setupScreenForMe(){
//	glPushAttrib(GL_VIEWPORT_BIT);
//	glViewport(0,0,texData.width, texData.height);
//	return;
	
	int w = texData.width;
	int h = texData.height;
	
	float halfFov, theTan, screenFov, aspect;
	screenFov 		= 60.0f;
	
	float eyeX 		= (float)w / 2.0;
	float eyeY 		= (float)h / 2.0;
	halfFov 		= PI * screenFov / 360.0;
	theTan 			= tanf(halfFov);
	float dist 		= eyeY / theTan;
	float nearDist 	= dist / 10.0;	// near / far clip plane
	float farDist 	= dist * 10.0;
	aspect 			= (float)w/(float)h;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(screenFov, aspect, nearDist, farDist);
	gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	
	glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
  	glTranslatef(0, -h, 0);       // shift origin up to upper-left corner.
	
    glViewport(0,0,texData.width, texData.height);
	
}

void ofFBOTexture::setupScreenForThem(){
//	glPopAttrib();
//	return;
	
    int w, h;
	
	w = glutGet(GLUT_WINDOW_WIDTH);
	h = glutGet(GLUT_WINDOW_HEIGHT);
	
	float halfFov, theTan, screenFov, aspect;
	screenFov 		= 60.0f;
	
	float eyeX 		= (float)w / 2.0;
	float eyeY 		= (float)h / 2.0;
	halfFov 		= PI * screenFov / 360.0;
	theTan 			= tanf(halfFov);
	float dist 		= eyeY / theTan;
	float nearDist 	= dist / 10.0;	// near / far clip plane
	float farDist 	= dist * 10.0;
	aspect 			= (float)w/(float)h;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(screenFov, aspect, nearDist, farDist);
	gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	
	glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
  	glTranslatef(0, -h, 0);       // shift origin up to upper-left corner.
	
	
    glViewport(0,0,w, h);
}

void ofFBOTexture::swapIn() {
	_isActive = true;
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo); // bind the FBO to the screen so we can draw to it
	
	if(autoClear) clear();
}

void ofFBOTexture::swapOut() {
	_isActive = false;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); //unbind the FBO
}

void ofFBOTexture::clean()
{
	// try to free up the texture memory so we don't reallocate
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/deletetextures.html
	if (texData.textureName[0] != 0){
		glDeleteTextures(1, (GLuint *)texData.textureName);
	}
	texData.width = 0;
	texData.height = 0;
	texData.bFlipTexture = false;
}

void ofFBOTexture::clear() {
	bool alreadyIn = _isActive;
	if(!alreadyIn) begin();		// MEMO
	//	glClearColor(0, 0, 0, 0); //clear color (bg of the FBO tex)		// MEMO
//	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen data of the texture to write on
	if(!alreadyIn) end();		// MEMO
}
