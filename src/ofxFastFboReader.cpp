#include "ofxFastFboReader.h"

//const int ofxFastFboReader::num_buffers = 2;

ofxFastFboReader::ofxFastFboReader(const int num_buffers) : pboIds(NULL), async(true), index(0), nextIndex(0), num_bytes(0), num_buffers(num_buffers) {}

ofxFastFboReader::~ofxFastFboReader()
{
	if (pboIds != NULL)
	{
		glDeleteBuffers(num_buffers, pboIds);
		delete [] pboIds;
		pboIds = NULL;
	}
}

bool ofxFastFboReader::readToPixels(ofFbo &fbo, ofPixelsRef pix, ofImageType type)
{
	genPBOs();
	
	int channels;
	int glType;
	
	if (type == OF_IMAGE_COLOR)
	{
		channels = 3;
		glType = GL_RGB;
	}
	else if (type == OF_IMAGE_COLOR_ALPHA)
	{
		channels = 4;
		glType = GL_RGBA;
	}
	else if (type == OF_IMAGE_GRAYSCALE)
	{
		channels = 1;
		glType = GL_LUMINANCE;
	}
	else
	{
		return false;
	}
	
	const int width = fbo.getWidth();
	const int height = fbo.getHeight();
	
	if (async)
	{
		index = (index + 1) % num_buffers;
		nextIndex = (index + 1) % num_buffers;
	}
	else
	{
		index = nextIndex = 0;
	}
	
	size_t nb = width * height * channels;
	
	if (nb != num_bytes)
	{
		num_bytes = nb;
		setupPBOs(num_bytes);
	}
	
	glReadBuffer(GL_FRONT);
	
	glBindFramebuffer(GL_FRAMEBUFFER, fbo.getIdDrawBuffer());

	glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[index]);
	glReadPixels(0, 0, width, height, glType, GL_UNSIGNED_BYTE, NULL);
	
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[nextIndex]);
	unsigned char* mem = (unsigned char*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
	
	if (mem)
	{
		pix.setFromPixels(mem, width, height, channels);
		glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
	}
	
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return mem != NULL;
}

bool ofxFastFboReader::readToPixels( ofFbo &fbo, ofFloatPixels& pix, int aGlType ) {
    genPBOs();
    
    int channels = 3;
    int numBytes = 4;
    int glType = aGlType;
    int glReadType = GL_RGB;
    
    if( glType == GL_RGB32F ) {
        // only support RGB32F to float pixels //
    } else if( glType == GL_RGBA32F ) {
        glReadType = GL_RGBA;
        channels = 4;
    } else {
        // no support for other options right now //
        return false;
    }
    
    const int width = fbo.getWidth();
    const int height = fbo.getHeight();
    
    if (async)
    {
        index = (index + 1) % num_buffers;
        nextIndex = (index + 1) % num_buffers;
    }
    else
    {
        index = nextIndex = 0;
    }
    
    size_t nb = width * height * (channels * numBytes);
    
    if (nb != num_bytes)
    {
        num_bytes = nb;
        setupPBOs(num_bytes);
    }
    
    glReadBuffer(GL_FRONT);
    
	glBindFramebuffer(GL_FRAMEBUFFER, fbo.getIdDrawBuffer());
    
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[index]);
    glReadPixels(0, 0, width, height, glReadType, GL_FLOAT, NULL );// (GLubyte*) NULL + (12) );
    
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[nextIndex]);
    float* mem = (float*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
    
    if (mem != NULL) {
        pix.setFromPixels( mem, width, height, channels );
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    } else {
        cout << "ofxFastFboReader :: 32F is null " << " | " << ofGetFrameNum() << endl;
    }
    
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return mem != NULL;
}

void ofxFastFboReader::genPBOs()
{
	if (!pboIds)
	{
		pboIds = new GLuint[num_buffers];
		glGenBuffers(num_buffers, pboIds);
	}
}

void ofxFastFboReader::setupPBOs(int num_bytes)
{
	for (int i = 0; i < num_buffers; i++)
	{
		glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[i]);
		glBufferData(GL_PIXEL_PACK_BUFFER, num_bytes, NULL, GL_STREAM_READ);
	}
	
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}
