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
	if(m_pCEGUIResourceProvider) { CEGUI_DELETE_AO m_pCEGUIResourceProvider; m_pCEGUIResourceProvider = NULL; }
}

void	XGuiHelloworld::Release()
{
	CEGUI::System::destroy();

	Helloworld::Release();
}

bool	XGuiHelloworld::LoadRenderWindow(HWND hWnd)
{
	if(!Helloworld::LoadRenderWindow(hWnd))
	{ return false; }

	//
	m_pCEGUIRenderer		= &CEGUI::OpenGL3Renderer::create( );
	m_pCEGUIResourceProvider= CEGUI_NEW_AO CEGUI::DefaultResourceProvider();
	m_pCEGUIXMLParser		= createParser();
	m_pCEGUIImageCodec		= createImageCodec();
	CEGUI::System::create( *m_pCEGUIRenderer, 
		(CEGUI::ResourceProvider *)m_pCEGUIResourceProvider, 
		(CEGUI::XMLParser*)m_pCEGUIXMLParser, 
		(CEGUI::ImageCodec*)m_pCEGUIImageCodec, 
		(CEGUI::ScriptModule*)NULL,
		"", "xgui.log");
	m_pCEGUIWindowManager	= &CEGUI::WindowManager::getSingleton( );

	//
	static_cast<CEGUI::DefaultResourceProvider*>(m_pCEGUIResourceProvider)->setResourceGroupDirectory("ui", "../data/ui");
	static_cast<CEGUI::DefaultResourceProvider*>(m_pCEGUIResourceProvider)->setResourceGroupDirectory("fonts", "../data/fonts");
	static_cast<CEGUI::DefaultResourceProvider*>(m_pCEGUIResourceProvider)->setDefaultResourceGroup("ui");

	CEGUI::SchemeManager::getSingleton( ).createFromFile( "WindowsLook/WindowsLook.scheme" );
//	CEGUI::FontManager::getSingleton( ).createFreeTypeFont( "commonv2c", 10, false, "commonv2c.ttf", "fonts");
	CEGUI::FontManager::getSingleton( ).createFromFile("Jura-10.font", "fonts");
	CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setDefaultFont("Jura-10");
	CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setDefaultImage("WindowsLook/MouseArrow");


	CEGUI::Window *window = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("Layout/ceguitest.layout");
	CEGUI::System::getSingletonPtr()->getDefaultGUIContext().setRootWindow(window);

	//
	CEGUI::MultiLineEditbox* editbox	= static_cast<CEGUI::MultiLineEditbox*>(window->getChild("CeguiTestDlg2/mleditbox"));
	editbox->setText( "this text program added.\n" );

	//
	return true;
}

void	XGuiHelloworld::_render()
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.1f, 0.0f);

	CEGUI::System::getSingletonPtr()->renderAllGUIContexts();

	glFinish();
}