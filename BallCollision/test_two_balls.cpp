#include <chrono>
#include <random>
#include "SFML/Graphics.hpp"
#include "lib/MiddleAverageFilter.h"
#include "lib/Ball.h"
#include "base_quad_tree.h"
Math::MiddleAverageFilter<double, 100> fpscounter;

void draw_fps(sf::RenderWindow &window, double fps) {
  char c[32];
  snprintf(c, 32, "FPS: %f", fps);
  std::string string(c);
  sf::String str(c);
  window.setTitle(str);
}

double get_system_energy(const std::vector<Ball>& balls) {
  double ENERGY = 0;
  for (auto& ball: balls) {
    ENERGY += ball.get_velocity() * ball.get_radius() * ball.get_radius();
  }
  return ENERGY;
}

#include <iostream>

int main() {
  sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "ball collision demo");
  std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());

  HeapQuadTree quad_tree({std::max(WINDOW_Y, WINDOW_X) / 2,
                          std::max(WINDOW_Y, WINDOW_X) / 2,
                          std::max(WINDOW_Y, WINDOW_X) / 2});

  // rand() returns new number every time we invoke it.
  // So we had to take this function-call out of the for-loop condition
  size_t balls_count = 2;
  std::vector<Ball> balls(balls_count);

  balls[0].set_properties({680, 54}, {0.774232, -0.632902}, 55, 20, false);
  balls[1].set_properties({763, 141}, {0.71478, -0.699349}, 50, 10, false);


  // TODO: May be we need to set framerate limit before balls insertion?
  window.setFramerateLimit(120);

  sf::Clock clock;
  double lastime = clock.restart().asSeconds();

  std::cout << "ENERGY = " << get_system_energy(balls) << '\n';

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    double current_time = clock.getElapsedTime().asSeconds();
    double deltaTime = current_time - lastime;
    fpscounter.push(1.0f / (current_time - lastime));
    lastime = current_time;

    for (auto &ball : balls) {
      ball.set_collided(false);
    }

    quad_tree.update(balls);

    for (uint32_t i = 0; i < balls_count; ++i) {
      Ball &ball = balls[i];
      auto close_balls_id = quad_tree.get_close_balls(ball);
      for (uint32_t id : close_balls_id) {
        if (id > i) {
          Ball &other = balls[id];
          if (ball.intersects(other)) {
            ball.set_collided(true);
            other.set_collided(true);
            collide(ball, other, deltaTime);
            std::cout << "ENERGY = " << get_system_energy(balls) << '\n';
          }
        }
      }
    }

    for (auto &ball : balls) {
      ball.move(deltaTime);
    }

    window.clear();
    // We need to specify &. Otherwise we'll create ball's
    // copy while we iterate through balls vector.
    for (const auto &ball : balls) {
      ball.draw(window);
    }

    draw_fps(window, fpscounter.getAverage());
    window.display();
  }
  return 0;
}