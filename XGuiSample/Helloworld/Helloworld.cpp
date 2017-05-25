// Helloworld.cpp : 定义应用程序的入口点。
//

#include "Helloworld.h"

//
SINGLETON_IMPLE(Helloworld);
Helloworld::Helloworld()
{
	m_hWnd		= NULL;
	m_hDC		= NULL;
	m_hGLRC		= NULL;
	
	ZeroMemory(&m_PixelFormatDesc, sizeof(PIXELFORMATDESCRIPTOR));

	m_fX			= 0.0f;
	m_fY			= 0.0f;
	m_fWidth		= 0.0f;
	m_fHeight		= 0.0f;

	m_nColorBits	= 32;
	m_nDepthBits	= 16;
	m_nStencilBits	= 1;
}

Helloworld::~Helloworld()
{

}

void	Helloworld::Release()
{
}

bool	Helloworld::Initialize()
{
	return true;
}

bool	Helloworld::DestroyRenderWindow()
{
	ReleaseOpenGL();

	if(m_hDC)
	{
		ReleaseDC(m_hWnd, m_hDC);
		m_hDC = NULL;
	}
	return true;
}

bool	Helloworld::LoadRenderWindow(HWND hWnd)
{
	//
	m_hWnd		= hWnd;
	m_hDC		= GetDC(m_hWnd);
	if(!m_hDC){ return false; }

	//
	if(!InitializeOpenGL())
	{ return false; }

	//
	return true;
}

void	Helloworld::ResizeRenderWindow(float width, float height)
{
	this->m_fWidth		= width;
	this->m_fHeight		= height;
}

void	Helloworld::ReleaseOpenGL()
{
	if(m_hGLRC)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hGLRC);

		m_hGLRC	= NULL;
	}
}

bool	Helloworld::InitializeOpenGL()
{
	//
	ZeroMemory(&m_PixelFormatDesc, sizeof(PIXELFORMATDESCRIPTOR));
	m_PixelFormatDesc.nSize				= sizeof(PIXELFORMATDESCRIPTOR);
	m_PixelFormatDesc.nVersion			= 1;
	m_PixelFormatDesc.dwFlags			= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	m_PixelFormatDesc.iPixelType		= PFD_TYPE_RGBA;
	m_PixelFormatDesc.cColorBits		= (BYTE)m_nColorBits;
	m_PixelFormatDesc.cDepthBits		= (BYTE)m_nDepthBits;
	m_PixelFormatDesc.cStencilBits		= (BYTE)m_nStencilBits;
	m_PixelFormatDesc.iLayerType		= PFD_MAIN_PLANE;	

	int nPixelFormat = ChoosePixelFormat(m_hDC, &m_PixelFormatDesc);
	if(nPixelFormat == 0){ return false; }

	SetPixelFormat(m_hDC, nPixelFormat, &m_PixelFormatDesc);

	m_hGLRC	= wglCreateContext(m_hDC);
	if(m_hGLRC == NULL){ return false; }

	if(!wglMakeCurrent(m_hDC, m_hGLRC))
	{
		return false;
	}

	//
	GLenum		result = glewInit( );
	if( result != GLEW_OK)
	{ 
		return false; 
	}

	//
	GLint attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB,	3,
		WGL_CONTEXT_MINOR_VERSION_ARB,	1,
		WGL_CONTEXT_FLAGS_ARB,			0,
		0
	};

	HGLRC hGLRC = NULL;
	if( !wglewIsSupported("WGL_ARB_create_context") || 
		(hGLRC = wglCreateContextAttribsARB(m_hDC, 0, attribs)) == NULL)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hGLRC);
		m_hGLRC	= NULL;
		return false;
	}
	else
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(m_hGLRC);
		wglMakeCurrent(m_hDC, hGLRC);
		m_hGLRC	= hGLRC;
	}

	return true;
}

void	Helloworld::Rendering()
{
	if(this->_update())
	{ 
		this->_render();
		this->_flush(); 
	}
}

bool	Helloworld::_update()
{
	//
	glViewport((GLint)m_fX, (GLint)m_fY, (GLsizei)m_fWidth, (GLsizei)m_fHeight);

	return true;
}

void	Helloworld::_flush()
{
	SwapBuffers(m_hDC);
}

void	Helloworld::_render()
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.1f, 0.0f);

	glFinish();
}


//
XGuiHelloworld::XGuiHelloworld()
{
}

XGuiHelloworld::~XGuiHelloworld()
{
}

bool	XGuiHelloworld::LoadRenderWindow(HWND hWnd)
{
	if(!Helloworld::LoadRenderWindow(hWnd))
	{ return false; }

	//
	m_pCEGUIRenderer		= &CEGUI::OpenGL3Renderer::create( );
	CEGUI::System::create( *m_pCEGUIRenderer, (CEGUI::ResourceProvider *)NULL, NULL, NULL, NULL );
	m_pCEGUIWindowManager	= &CEGUI::WindowManager::getSingleton( );

	//
	return true;
}