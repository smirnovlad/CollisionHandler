# Collision handler

A collision handler for objects of different shapes and sizes has been implemented. Perfectly elastic objects move within the window and collide according to the laws of physics. To optimize the algorithmic complexity of the algorithm, a quadtree structure is used.

<p align="center">
  <img src="https://media0.giphy.com/media/v1.Y2lkPTc5MGI3NjExOTY2Nzg4MWNlNjdkMTJiNGE4MTdiNzRmMjk3YTM3NzVmNDJkYzNhNCZlcD12MV9pbnRlcm5hbF9naWZzX2dpZklkJmN0PWc/lEDJao3qwbS12UCz7r/giphy.gif" width="600" height="338" />
</p>

## Solution

At the beginning of solving the problem, I considered several possible approaches:
* Dividing the window into square cells of a certain size and keeping track of which circles partially or completely cover each cell during the movement. This approach has multiple nuances.

* Storing a "hitbox" for each circle, which is a rectangle that fully contains the circle. During the movement of the circles, checking for intersections/touches of the hitboxes using a scanline and an interval tree. However, there are several questions regarding this approach.

* During the problem-solving process, dividing the window into squares according to the QuadTree data structure definition, which is the approach I settled on.

## Complexity

* The expected time complexity for adding a circle to the structure is **O(log n)**. In the worst case, adding a circle takes **O(n)** operations.

* The time complexity for one iteration, assuming a uniform distribution of circles, is  **O(n^1.5 log n)**. This is because the expected time for descending the QuadTree is **O(log n)**, and the expected number of neighbors is estimated to be **O(sqrt(n))**.

* Due to the limited number of neighbors, the expected time complexity for one iteration is  **O(n log n)**.
  
## Implementation details

Two implementations have been developed to support the QuadTree structure: a heap-based tree and a stack-based tree. The advantage of the second option over the first one lies in its faster performance. During the movement of circles while working with a laptop, there are occasional freezes, which is why I came up with the idea of supporting the QuadTree on the stack. It seems that using the stack-based tree indeed reduces the occurrence of freezes. I would like to note that on the second, more powerful computer, no freezes are observed.

If the derived formulas for overlapping circles without changing their relative positions are used, there may be unpredictable behavior in their subsequent movement. Therefore, before changing the velocities of two touching circles (which may overlap due to the discreteness of time and constraints on the circles' speed), I move one of them along the line connecting their centers so that they touch each other. In this case, on the next iteration, these two circles will not overlap each other. I haven't come up with a smarter solution... For additional comments regarding collision handling, please refer to the code in ball.cpp.

When circles go beyond the window boundaries, I assume that the circle's displacement beyond the edge is small. Therefore, I shift the ball to the position where it touches the corresponding boundary.

## Architecture

During the development of the application, the SOLID principles were followed. As a result, in the current version of the project, it is possible to add a new shape class (e.g., a rectangle) and work with it seamlessly. The simulation class and the QuadTree structure are both templated, allowing for flexibility in accommodating different types of shapes.

[Class diagram for simulation](UML/simulation.drawio.png?raw=true "Simulation class diagram")

[Class diagram for the QuadTree structure](UML/quad_tree.drawio.png?raw=true "quad_tree class diagram")

[Class diagram for shapes](UML/shape.drawio.png?raw=true "Shape class diagram")


## Build

The project has one target for execution:
<ul>
  <li> main </li>

[//]: # (  <li> test </li>)

[//]: # (  <li> test_two_balls </li>)
</ul>

[//]: # (Последние два я использовал для тестирования решения.)

To launch the application, you need to select the target **main**.

## References

[Formulas for calculating the movement of circles](https://en.wikipedia.org/wiki/Elastic_collision)

[QuadTree #1](https://vixra.org/pdf/2005.0108v1.pdf)

[QuadTree #2](https://github.com/pvigier/Quadtree)
