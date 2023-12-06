#include <iostream>
#include <string>

enum SHAPE
{
   CIRCLE = 0,
   SQUARE = 1
};

enum COLOR
{
   NONE = 0,
   PURPLE = 1,
   YELLOW = 2,
   RED = 3,
};

class IShape 
{
   public:
      virtual void set_color(const COLOR color) = 0;
      virtual COLOR get_color() = 0;
};

class Circle : public IShape
{
   private:
      COLOR color_ = NONE;

   public:
      Circle();
      void set_color(COLOR color) override;
      COLOR get_color() override;
      std::ostream& operator<<(std::ostream &strm, const Circle &a);
      ~Circle();
};

Circle::Circle()
{

}
std::ostream& operator<<(std::ostream &strm, const Circle &circle)
{
      return strm << "A(" << COLOR::PURPLE << ")";
}

void Circle::set_color(COLOR color)
{
   color_ = color;
}

COLOR Circle::get_color()
{
   return color_;
}

Circle::~Circle()
{
   
}

class Square : public IShape
{
   private:
      COLOR color_;

   public:
      Square();
      void set_color(COLOR color) override;
      COLOR get_color() override;
      ~Square();
};

Square::Square()
{

}

void Square::set_color(COLOR color)
{
   color_ = color;
}

COLOR Square::get_color()
{
   return color_;
}

Square::~Square()
{
   
}

class DefaultShape : public IShape
{
   private:
      COLOR color_;

   public:
      DefaultShape();
      void set_color(COLOR color) override;
      COLOR get_color() override;
      ~DefaultShape();
};

DefaultShape::DefaultShape()
{

}

void DefaultShape::set_color(const COLOR color)
{
   color_ = color;
}

COLOR DefaultShape::get_color()
{
   return color_;
}

DefaultShape::~DefaultShape()
{
   
}

class IDrawSimulator 
{
    public:
    virtual IShape* draw_shape(const SHAPE shape) = 0;
};

class DrawSimulator : public IDrawSimulator
{
    public:

    DrawSimulator();
    IShape* draw_shape(const SHAPE shape) override;
    ~DrawSimulator();

};

DrawSimulator::DrawSimulator()
{
 
}

DrawSimulator::~DrawSimulator()
{
 
}

IShape* DrawSimulator::draw_shape(SHAPE shape)
{
   IShape* drawn_shape = nullptr;

   switch (shape)
   {
      case SHAPE::CIRCLE:
         drawn_shape = new Circle();
         break;

      case SHAPE::SQUARE:
         drawn_shape = new Square();
         break;
   
      default:
         drawn_shape = new DefaultShape();
         break;
   }

   return drawn_shape;
}

int main()
{
   IDrawSimulator* draw_simulator = new DrawSimulator();
   
   auto shape_a = draw_simulator->draw_shape(SHAPE::CIRCLE);
   auto shape_b = draw_simulator->draw_shape(SHAPE::SQUARE);

   shape_a->set_color(COLOR::PURPLE);
   shape_b->set_color(COLOR::YELLOW);

   delete draw_simulator;

   delete shape_a;
   delete shape_b;
   
   return 0;
}