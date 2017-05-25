#ifndef __Helloworld_H__
#define __Helloworld_H__

#pragma once

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ

// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//
#include <objbase.h>


//
#if defined (__APPLE__)

#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#include <OpenGL/OpenGL.h>

#elif defined (_MSC_VER)

//
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/wglew.h>


#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )


#endif

//
#include <Singleton.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
//#include <CEGUI/ScriptingModules/LuaScriptModule/CEGUILua.h>

//
class Helloworld : public SingletonT<Helloworld>
{
public:
	Helloworld();
	virtual ~Helloworld();

	virtual bool	Initialize();
	virtual void	Release();
		
	virtual void	Rendering();

	virtual bool	LoadRenderWindow(HWND hWnd);
	virtual	bool	DestroyRenderWindow();
	virtual void	ResizeRenderWindow(float width, float height);

protected:
	virtual bool	InitializeOpenGL();
	virtual void	ReleaseOpenGL();

	virtual bool	_update();
	virtual void	_flush();
	virtual void	_render();


protected:
	HWND					m_hWnd;

	HDC						m_hDC;
	PIXELFORMATDESCRIPTOR	m_PixelFormatDesc;

	HGLRC					m_hGLRC;

protected:
	//
	float					m_fX;
	float					m_fY;
	float					m_fWidth;
	float					m_fHeight;

	//
	int						m_nColorBits;
	int						m_nDepthBits;
	int						m_nStencilBits;
};


//
class XGuiHelloworld : public Helloworld
{
public:
	XGuiHelloworld();
	virtual ~XGuiHelloworld();

	virtual bool	LoadRenderWindow(HWND hWnd);

private:
	CEGUI::OpenGL3Renderer*		m_pCEGUIRenderer;
	CEGUI::WindowManager*		m_pCEGUIWindowManager;
};

//
#endif //__Helloworld_H__


