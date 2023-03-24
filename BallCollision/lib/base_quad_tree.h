#ifndef INTERVIEW_BALLCOLLISION_QUADTREE_H_
#define INTERVIEW_BALLCOLLISION_QUADTREE_H_

#include <unordered_set>
#include "Ball.h"

class HeapQuadTree;

class base_quad_tree {
 protected:
  struct AABB {
    sf::Vector2f center_{0, 0};
    double half_of_length_ = 0;

    AABB() = default;
    AABB(double x, double y, double half_of_length) :
        center_(x, y), half_of_length_(half_of_length) {}

    bool contains(const Ball &ball) const;
    bool intersects(const AABB &other) const;
  };

  struct BaseNode {
    AABB boundary;
    std::vector<uint32_t> balls_id;

    BaseNode() = default;
    BaseNode(const AABB &boundary) :
        boundary(boundary) {}

    virtual bool insert(base_quad_tree *qt_ptr, uint32_t idx) = 0;
    virtual void query(const base_quad_tree *qt_ptr,
                       const AABB &range,
                       std::vector<uint32_t> &close_balls_id) const = 0;
  };

  virtual std::vector<uint32_t> query(const AABB &range) const;

 public:
  base_quad_tree() = default;
  base_quad_tree(const std::vector<Ball> &balls);
  std::vector<uint32_t> get_close_balls(const Ball &ball) const;
  bool is_collided(const Ball &ball) const;
  bool insert(const Ball &ball);
  virtual void update(const std::vector<Ball> &balls) = 0;

 protected:
  std::unique_ptr<BaseNode> root_{nullptr};
  std::vector<Ball> balls_;
  const uint32_t MAX_AABB_BALLS_COUNT = 3;

  friend class HeapQuadTree;
  friend class StackQuadTree;
};

class HeapQuadTree : public base_quad_tree {
 private:
  struct Node : BaseNode {
    std::array<std::unique_ptr<Node>, 4> children;

    void subdivide();
    bool insert(base_quad_tree *qt_ptr, uint32_t idx) override;
    void query(const base_quad_tree *qt_ptr,
               const AABB &range,
               std::vector<uint32_t> &close_balls_id) const override;

    Node(const AABB &boundary) :
        BaseNode(boundary) {}
  };

 public:
  HeapQuadTree(const AABB &boundary);
  HeapQuadTree(const std::vector<Ball> &balls,
               const AABB &boundary);
  void update(const std::vector<Ball> &balls) override;
};

class StackQuadTree : public base_quad_tree {
 private:
  struct Node : BaseNode {
    std::array<uint32_t, 4> children = {0, 0, 0, 0};

    void subdivide(StackQuadTree *qt_ptr);
    bool insert(base_quad_tree *qt_ptr, uint32_t idx) override;
    void query(const base_quad_tree *qt_ptr,
               const AABB &range,
               std::vector<uint32_t> &close_balls_id) const override;

    Node(const AABB &boundary) :
        BaseNode(boundary) {}
  };

 private:
  std::vector<Node> nodes_;

 public:
  StackQuadTree(const AABB &boundary);
  StackQuadTree(const std::vector<Ball> &balls,
                const AABB &boundary);
//  void update(const std::vector<Ball> &balls) override;
};

#endif //INTERVIEW_BALLCOLLISION_QUADTREE_H_