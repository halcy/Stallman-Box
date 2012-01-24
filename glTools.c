/**
 * A bunch of low level opengl-related things that are better off wrapped.
 * Mostly concerning the generation and use of Buffer Objects, Shaders and
 * Textures.
 */

#include "glTools.h"
#include "PNGTextureLoader.h"

GLuint assocVAO;

uint8_t* loadData(const char* filename, int* len) {
	int size;
	uint8_t* buffer;
	FILE* fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	buffer = (uint8_t*)malloc(size);
	fseek(fp, 0, SEEK_SET );
	int res = fread(buffer, 1, size, fp);
	if(res != size) {
		fprintf(stderr, "ERROR: Read problem.\n");
		exit(-1);
	}
	fclose(fp);
	*len = size;
	return(buffer);
}

char* loadText(const char* filename, int* len) {
	long size;
	char* buffer;
	FILE* fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	buffer = (char*)malloc(size + 1);
	fseek(fp, 0, SEEK_SET );
	int res = fread(buffer, 1, size, fp);
	if(res != size) {
		fprintf(stderr, "ERROR: Read problem.\n");
		exit(-1);
	}
	fclose(fp);
	buffer[size] = '\0';
	*len = size;
	return(buffer);
}

void printShaderInfoLog(GLuint object) {
	GLint blen = 0;
	GLint slen = 0;

	glGetShaderiv(object, GL_INFO_LOG_LENGTH , &blen);
	if(blen > 1) {
		GLchar* compilerLog = (GLchar*)malloc(blen);
		glGetShaderInfoLog(object, blen, &slen, compilerLog);
		fprintf(stderr, "INFO: Shader info log:\n%s\n", compilerLog);
		free (compilerLog);
	}
}

void printProgramInfoLog(GLuint object) {
	GLint blen = 0;
	GLint slen = 0;

	glGetProgramiv(object, GL_INFO_LOG_LENGTH , &blen);
	if(blen > 1) {
		GLchar* compilerLog = (GLchar*)malloc(blen);
		glGetProgramInfoLog(object, blen, &slen, compilerLog);
		fprintf(stderr, "INFO: Shader info log:\n%s\n", compilerLog);
		free (compilerLog);
	}
}

GLuint loadShader(const char* filename, GLuint type) {
	int len;
	char* data;
	const char *constData;

	GLuint shader = glCreateShader(type);
	data = loadText(filename, &len);
	constData = data;
	glShaderSource(shader, 1, &constData, &len);
	fprintf(stderr, "INFO: Compiling shader %s -> %d.\n", filename, shader);	
	glCompileShader(shader);
	printShaderInfoLog(shader);
	free(data);

	return shader;
}

void bindShaderAttribLocations(GLuint shader) {
	glBindAttribLocation(shader, 0, "inp_Position");
	glBindAttribLocation(shader, 1, "inp_Normal");
	glBindAttribLocation(shader, 2, "inp_TexCoord");

	glBindFragDataLocation(shader, 0, "out_FragColor" );
}

GLuint makeSimpleProgram(GLuint vert, GLuint frag) {
	GLuint program = glCreateProgram();

	glAttachShader(program, vert);
	glAttachShader(program, frag);

	bindShaderAttribLocations(program);
	
	fprintf(stderr, "INFO: Linking shader program (%d, %d).\n", vert, frag);
	glLinkProgram(program);
	printProgramInfoLog(program);
	
	return program;
}

GLuint makeFloat4Buffer(int w, int h) {
	GLuint buffertex;

	glGenTextures(1, &buffertex);
	glBindTexture(GL_TEXTURE_2D, buffertex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,w,h,0,GL_RGBA, GL_FLOAT,0);

	return buffertex;
}

GLuint makeIntBuffer(int w, int h) {
	GLuint buffertex;

	glGenTextures(1, &buffertex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, buffertex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, w, h, 0,  GL_RED, GL_FLOAT, 0);

	return buffertex;
}

GLuint makeFBO(GLuint texture) {
	GLuint fbo;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1920, 1080);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	return fbo;
}

GLuint makeVBO(int entries, vertexentry* data) {
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexentry) * entries, data, GL_STATIC_DRAW);
	return vbo;
}

GLuint makeEBO(int entries, elemententry* data) {
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elemententry) * entries, data, GL_STATIC_DRAW);
	return(ebo);
}

void drawBuffers(GLuint vbo, GLuint ebo, GLuint shader, int entries, GLenum type) {
	glUseProgram(shader);
	drawBuffersActive(vbo, ebo, shader, entries, type);
}

void drawBuffersActive(GLuint vbo, GLuint ebo, GLuint shader, int entries, GLenum type) {
	//glBindVertexArray(assocVA[1]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexentry),
		0
	);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexentry),
		(GLvoid*)sizeof(vec3)
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexentry),
		(GLvoid*)(sizeof(vec3) * 2)
	);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(
		type,
		entries,
		GL_UNSIGNED_INT,
		(void*)0
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void setMvMatrix(matrix mvMatrixO, GLuint program) {
	matrix nMatrixT, nMatrixO;
	matrixInverse(&nMatrixT, mvMatrixO);
	matrixTranspose(&nMatrixO, nMatrixT);

	matrixAsUniform(mvMatrixO, program, "modelviewMatrix");
	matrixAsUniform(nMatrixO, program, "normalMatrix");
}

void setMvMatrixActive(matrix mvMatrixO, GLuint program) {
	matrix nMatrixT, nMatrixO;
	matrixInverse(&nMatrixT, mvMatrixO);
	matrixTranspose(&nMatrixO, nMatrixT);

	matrixAsUniformActive(mvMatrixO, program, "modelviewMatrix");
	matrixAsUniformActive(nMatrixO, program, "normalMatrix");
}

/*static void loadFunctions() {
	// Load vertex array funcs
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) wglGetProcAddress("glGenVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) wglGetProcAddress("glBindVertexArray");

	// Load mipmap generation
	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmap");

	// Load shaderimage load store
	glBindImageTextureEXT = (PFNGLBINDIMAGETEXTUREEXTPROC) wglGetProcAddress("glBindImageTextureEXT");
	glMemoryBarrierEXT = (PFNGLMEMORYBARRIEREXTPROC) wglGetProcAddress("glMemoryBarrierEXT");

	// Load FBOs / Renderbuffers
	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) wglGetProcAddress("glGenFramebuffers");
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) wglGetProcAddress("glBindFramebuffer");
	glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC) wglGetProcAddress("glFramebufferTextureLayer");
	glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) wglGetProcAddress("glFramebufferTexture2D");
	glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) wglGetProcAddress("glCheckFramebufferStatus");
	glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) wglGetProcAddress("glDeleteFramebuffers");
	glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC) wglGetProcAddress("glMapBufferRange");
	glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC) wglGetProcAddress("glFlushMappedBufferRange");
	glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) wglGetProcAddress("glGenRenderbuffers");
	glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) wglGetProcAddress("glBindRenderbuffer");
	glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) wglGetProcAddress("glRenderbufferStorage");
	glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) wglGetProcAddress("glFramebufferRenderbuffer");
	glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC) wglGetProcAddress("glBlitFramebuffer");
	glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) wglGetProcAddress("glDeleteRenderbuffers");
	glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) wglGetProcAddress("glRenderbufferStorageMultisample");


	glTexStorage1D = (PFNGLTEXSTORAGE1DPROC) wglGetProcAddress("glTexStorage1D");
	glTexStorage2D = (PFNGLTEXSTORAGE2DPROC) wglGetProcAddress("glTexStorage2D");
	glTexStorage3D = (PFNGLTEXSTORAGE3DPROC) wglGetProcAddress("glTexStorage3D");
}*/

void setupGL() {
        //loadFunctions();
        //prepareVertices();

	glGenVertexArrays(1,&assocVAO);
	glBindVertexArray(assocVAO);
	
        glClearColor(0.0f, 1.0f, 1.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        glShadeModel(GL_SMOOTH);
}

static short le_short(unsigned char *bytes) {
	return bytes[0] | ((char)bytes[1] << 8);
}

// Read a tga file into a buffer for use as an OpenGL texture.
// Original code by Joe Groff, modified to handle alpha channels.
void *readTga( const char *filename, int *width, int *height, int *alpha ) {

	// TGA header for loading things into.
	struct tga_header {
		char id_length;
		char color_map_type;
		char data_type_code;
		unsigned char color_map_origin[2];
		unsigned char color_map_length[2];
		char color_map_depth;
		unsigned char x_origin[2];
		unsigned char y_origin[2];
		unsigned char width[2];
		unsigned char height[2];
		char bits_per_pixel;
		char image_descriptor;
	} header;

	int color_map_size, pixels_size;
	FILE *f;
	size_t read;
	void *pixels;

	// Try to open the file.
	f = fopen( filename, "rb" );
	if( !f ) {
		fprintf( stderr, "Unable to open %s for reading\n", filename );
		return NULL;
	}

	// Check for valid header data.
	read = fread( &header, 1, sizeof( header ), f );
	if( read != sizeof( header ) ) {
		fprintf( stderr, "%s has incomplete tga header\n", filename );
		fclose( f );
		return NULL;
	}
	if( header.data_type_code != 2 ) {
		fprintf(
			stderr,
			"%s is not an uncompressed RGB tga file\n",
			filename
		);
		fclose( f );
		return NULL;
	}
	if( (header.bits_per_pixel != 32) && (header.bits_per_pixel != 24) ) {
		fprintf(
			stderr,
			"%s is not 24/32-bit uncompressed RGB/A tga file.\n",
			filename
		);
		fclose( f );
		return NULL;
	}

	// Return to the outside if an alpha channel is present.
	if( header.bits_per_pixel == 32 ) {
		*alpha = 1;
	}
	else {
		*alpha = 0;
	}

	// Only handling non-palleted images.
	color_map_size =
		le_short(header.color_map_length) * (header.color_map_depth/8);
	if( color_map_size > 0 ) {
		fprintf(
			stderr,
			"%s is colormapped, cannot handle that.\n",
			filename
		);
		fclose( f );
		return NULL;
	}

	// Set return width/height values and calculate image size.
	*width = le_short( header.width );
	*height = le_short( header.height );
	pixels_size = *width * *height * ( header.bits_per_pixel / 8 );
	pixels = malloc( pixels_size );

	// Read image.
	read = fread( pixels, 1, pixels_size, f );
	if( read != pixels_size ) {
		fprintf( stderr, "%s has incomplete image\n", filename );
		fclose( f );
		free( pixels );
		return NULL;
	}

	return pixels;
}

// Load a texture from a TGA file.
GLuint loadTextureTGA( const char *filename ) {

	// Load tga data into buffer.
	int width, height, alpha;
	unsigned char* pixels;

	pixels = readTga( filename, &width, &height, &alpha );
	if( pixels == 0 ) {
		fprintf( stderr, "ERROR: Image loading failed: %s\n", filename );
		return 0;
	}

	// Generate texture, bind as active texture.
	GLuint texture;
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );

	// Texture parameters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	// Load pixels from buffer into texture.
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		alpha == 1 ? GL_RGBA8 : GL_RGB8,
		width,
		height,
		0,
		alpha == 1 ? GL_BGRA : GL_BGR,
		GL_UNSIGNED_BYTE,
		pixels
	);

	// Release buffer.
	free(pixels);

	return texture;
}

// Load a texture from a PNG file.
GLuint loadTexturePNG( const char *filename ) {
	
	// Load png data into buffer.
	int width, height;
	float* pixels;

	pixels = loadPNGToFloatBuffer(filename, &width, &height);
	if( pixels == 0 ) {
		fprintf( stderr, "ERROR: Image loading failed: %s\n", filename );
		return 0;
	}

	// Generate texture, bind as active texture.
	GLuint texture;
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );

	// Texture parameters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	// Load pixels from buffer into texture.
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA8,
		width,
		height,
		0,
		GL_RGBA,
		GL_FLOAT,
		pixels
	);

	// Release buffer.
	free(pixels);

	return texture;
}
