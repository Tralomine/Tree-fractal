#include <SFML/Graphics.hpp>
#include <cmath>
#include "sf_hsv_color.hpp"

double constexpr pi(3.1415926535897932384626);

struct param{
  float d_t;
  float d_l;
  size_t inc;
};

class Fract : public sf::Drawable
{
  public:
    Fract(sf::Vector2f p, float l, float theta, param param):
    m_p(p), m_l(l), m_theta(theta), m_param(param)
    {
      m_param.inc--;
      this->update();
    }

    void update()
    {
      m_childs.clear();
      if(m_l > 1 && m_param.inc > 0){
        m_childs.push_back({{m_p.x+m_l*cosf(m_theta), m_p.y+m_l*sinf(m_theta)}, m_l * m_param.d_l, m_theta + m_param.d_t, m_param});
        m_childs.push_back({{m_p.x+m_l*cosf(m_theta), m_p.y+m_l*sinf(m_theta)}, m_l * m_param.d_l, m_theta - m_param.d_t, m_param});
      }
    }

    void addDTheta(float m){m_param.d_t += m;}
    void addTheta(float m){m_theta += m;}
    void addDL(float m){m_param.d_l += m;}

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const{
      for(auto& c : m_childs){
        target.draw(c);
      }
      std::vector<sf::Vertex> v;
      v.push_back(sf::Vertex(m_p, hsv(16*(m_param.inc+1), 1.f, 1.f)));
      v.push_back(sf::Vertex({m_p.x+m_l*cosf(m_theta), m_p.y+m_l*sinf(m_theta)}, hsv(16*m_param.inc, 1.f, 1.f)));
      target.draw(v.data(), v.size(), sf::Lines, states);
    }

    sf::Vector2f m_p;
    float m_theta;
    float m_l;
    param m_param;
    std::vector<Fract> m_childs;
};

int main(int argc, char const *argv[]) {
  sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();

  int width = videoMode.width;
  int height = videoMode.height;

  Fract fract({width/2, height/2}, 256, 0, {0, 0.66666666, 14});
  Fract fract2({width/2, height/2}, 256, pi, {0, 0.6666666, 14});

  sf::RenderWindow app(videoMode, "test", sf::Style::Fullscreen);

  sf::Clock timer;
  sf::Font fira;
  fira.loadFromFile("fira.otf");
  sf::Text fps("test", fira, 18);

  while (app.isOpen()) {
    sf::Event event;
    while(app.pollEvent(event)){
      if(event.type == sf::Event::Closed){
        app.close();
      }
    }

    timer.restart();

    fract.addDTheta(0.001);
    fract.addTheta(0.001);
    fract.update();
    fract2.addDTheta(0.001);
    fract2.addTheta(0.001);
    fract2.update();

    app.clear();
    app.draw(fract);
    app.draw(fract2);

    // while(timer.getElapsedTime().asMilliseconds() < 16);
    fps.setString(std::to_string(int(1.0/timer.getElapsedTime().asSeconds())));
    timer.restart();

    app.draw(fps);
    app.display();

  }
  return 0;
}
