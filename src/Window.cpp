#include "Window.h"
#include <iostream>

Window::Window() {}

void Window::Create(const std::string& title, const int width, const int height)
{
	// Create a window with a tilebar and a close button - no option to resize
	m_Window.create(sf::VideoMode(width, height), title, sf::Style::Titlebar | sf::Style::Close);
	m_Window.setVerticalSyncEnabled(true); // Enable vsync

	m_Width = width;
	m_Height = height;
}

void Window::SetPosition(int x, int y) { m_Window.setPosition(sf::Vector2i(x, y)); }
void Window::SetSize(unsigned int width, unsigned int height) { m_Window.setSize(sf::Vector2u(width, height)); }
void Window::SetTitle(const std::string& title) { m_Window.setTitle(title); }

int Window::GetWidth() { return m_Window.getSize().x; }
int Window::GetHeight() { return m_Window.getSize().y; }

bool Window::GetKeyDown(sf::Keyboard::Key key) { return sf::Keyboard::isKeyPressed(key); }
bool Window::GetEvent(sf::Event& event) { return m_Window.pollEvent(event); }

void Window::Clear(sf::Color colour) { m_Window.clear(colour); }

void Window::Draw(sf::Sprite& sprite) { m_Window.draw(sprite); }
void Window::Draw(sf::Text& text) { m_Window.draw(text); }
void Window::DrawBoundingBox(Vec2 position, Vec2 size)
{
	const float scale = 1.0f;

	sf::RectangleShape left;
	left.setPosition(sf::Vector2f(position.x, position.y));
	left.setSize(sf::Vector2f(scale, size.y));

	sf::RectangleShape up;
	up.setPosition(sf::Vector2f(position.x, position.y));
	up.setSize(sf::Vector2f(size.x, scale));

	sf::RectangleShape right;
	right.setPosition(sf::Vector2f(position.x + size.x, position.y));
	right.setSize(sf::Vector2f(scale, size.y));

	sf::RectangleShape down;
	down.setPosition(sf::Vector2f(position.x, position.y + size.y));
	down.setSize(sf::Vector2f(size.x + 1, scale));

	m_Window.draw(left);
	m_Window.draw(right);
	m_Window.draw(up);
	m_Window.draw(down);
}

void Window::SetCamera(Camera& camera) 
{ 
	sf::View view(sf::Vector2f(camera.position.x, camera.position.y), sf::Vector2f(camera.size.x, camera.size.y * ((float)m_Height / (float)m_Width)));
	m_Window.setView(view); 
}

Camera Window::GetDefaultCamera()
{
	Camera camera;
	camera.position = Vec2(m_Window.getDefaultView().getCenter().x, m_Window.getDefaultView().getCenter().y);
	camera.size = Vec2(m_Window.getDefaultView().getSize().x, m_Window.getDefaultView().getSize().y);
	return camera;
}

void Window::Display() { m_Window.display(); }

bool Window::ShouldClose() { return !m_Window.isOpen(); }
void Window::Close() { m_Window.close();  }

Window::~Window()
{

}