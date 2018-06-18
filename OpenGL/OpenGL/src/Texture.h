#pragma once
#include "Renderer.h"

class Texture
{
private:
	unsigned int	m_RendererID;
	std::string		m_Filepath;
	unsigned char*	m_LocalBuffer;
	int				m_Width;
	int				m_Height;
	int				m_BPP;
public:
	Texture(const std::string& filepath);
	~Texture();

	void Bind()const;
	void Unbind()const;

	inline int GetWidth() const { return m_Width; }
	inline int getHeiht() const { return m_Height; }
};
