#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>

struct Point
{
    int x;
    int y;
    int z;
};

class IRenderer
{
public:
    virtual ~IRenderer() {};
    virtual void render_it(Point& point) = 0;
};

class OpenGLRenderer : public IRenderer 
{
    void render_it(Point& point)
    {
        std::cout << "Rendering with OpenGL the point {x: " << 
            point.x << ", y: " << 
            point.y << ", z: " << 
            point.z << "}." << std::endl;
    }
};

class DirectXRenderer : public IRenderer 
{
    void render_it(Point& point)
    {
        std::cout << "Rendering with DirectX the point {x: " << 
            point.x << ", y: " << 
            point.y << ", z: " << 
            point.z << "}." << std::endl;
    }
};

enum RENDER_TYPE
{
    OPENGL = 0,
    DIRECTX = 1
};

class IRendererFactory
{
public:
    virtual std::shared_ptr<IRenderer> create_renderer(const RENDER_TYPE& render_type) = 0;
};

class RendererFactory : public IRendererFactory
{
public:
    std::shared_ptr<IRenderer> create_renderer(const RENDER_TYPE& render_type) 
    {
        std::shared_ptr<IRenderer> render;

        switch (render_type)
        {
            case RENDER_TYPE::OPENGL:
                render = std::make_shared<OpenGLRenderer>();
                break;
            case RENDER_TYPE::DIRECTX:
                render =std::make_shared<DirectXRenderer>();
                break;       
            default:
                throw std::invalid_argument("The render type not suported.");
                break;
        }

        return render;
    }
};

/// @brief NOTE: Expose this functionality to the clients (.h in library for client)
class IGraphicCard
{
public:
    virtual void render_it(Point& point) = 0;
};

class GraphicCard : public IGraphicCard
{
private:
    std::shared_ptr<IRendererFactory> _render_factory;
    std::shared_ptr<IRenderer> _renderer;

public:
    GraphicCard()
    {
        _render_factory = std::make_shared<RendererFactory>();
    }

    void render_it(Point& point)
    {
        if (_renderer == nullptr)
        {  
            #ifdef __linux__ 
                _renderer = _render_factory->create_renderer(RENDER_TYPE::OPENGL);
            #elif _WIN32
                _renderer = _render_factory->create_renderer(RENDER_TYPE::DIRECTX);
            #endif
        }

        _renderer->render_it(point);
    }
};

int main()
{
    std::shared_ptr<IGraphicCard> graphic_card = std::make_shared<GraphicCard>();

    struct Point  points [] = {
        {1,2,3},
        {4,5,6},
        {7,8,9}
    };

    for(Point point : points)
    {
        graphic_card->render_it(point);
    }

    return 0;
}
