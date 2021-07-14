# mandelbrot-set-gpu
This is a Mandelbrot Set viewer implemented in C++ with [SFML](https://www.sfml-dev.org/). Here I list all the features of the application:
* **Viewer Modes**
  * [x] Default mode: Color according to the divergence speed
  * [ ] Data mode:
    * Display the values of some relevant parameters
    * Draw the reference point
  * [ ] Debug mode:
    * Color according to the distance to the Mandelbrot Set
    * Highlight parts of the image that used a rendering optimisation
    * Draw the path used in order to find the reference point
* **Rendering**
  * [x] Use a shader to process each pixel in parallel
  * [ ] Use the previous image to skip computation when:
    * [ ] the offset is changing
    * [ ] the limit is changing
    * [ ] the scale is changing
* **Escape time algorithm**
  * [x] Check if a a pixel lies within the cardioid or the period-2 bulb in order to skip computation
  * [x] Use an algebraic trick to minimize the number of multiplications  in the while loop (3 instead of 5)
* **Perturbation Theory**
  * [ ] Use Perturbation Theory rather than Escape Time algorithm when a reference point is given
  * [ ] Use gradient descent on an approximation of the distance function to find a reference point
  * [ ] Use binary search with a power series approximation in order to find the iteration for which it diverges
