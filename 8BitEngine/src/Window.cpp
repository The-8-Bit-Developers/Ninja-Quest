#include "Window.h"
#include <iostream>

Window::Window() {}

void Window::Create(const std::string& title, const int width, const int height)
{
	m_Window.create(sf::VideoMode(width, height), title, sf::Style::Default);
	m_Window.setVerticalSyncEnabled(true); // Enable vsync
	m_Window.setFramerateLimit(60); // Box2D sucks :( TODO: fix
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
	// We know the size.x to be just the size,
	// but the y size must change depending on the aspect ratio
	float height = camera.size.x * ((float)GetHeight() / (float)GetWidth());

	std::cout << std::to_string(GetWidth()) << std::endl;
	sf::View view(sf::Vector2f(camera.position.x, -camera.position.y), sf::Vector2f(camera.size.x, height));
	m_Window.setView(view); 
}

Camera Window::GetDefaultCamera()
{
	Camera camera;
	camera.position = Vec2((float)GetWidth() / 2, -(float)GetHeight() / 2);
	camera.size = Vec2((float)GetWidth(), (float)GetHeight() * ((float)GetHeight() / (float)GetWidth()));
	return camera;
}

void Window::Display() { m_Window.display(); }

bool Window::ShouldClose() { return !m_Window.isOpen(); }
void Window::Close() { m_Window.close();  }

Window::~Window()
{

}