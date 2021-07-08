# mandelbrot-set-gpu
This is a Mandelbrot Set viewer. 

The features that should be implemented:
* **Rendering**
  * [x] Use a shader to process each pixel in parallel
  * [x] Use previous image to skip computation when only the offset is changing
  * [ ] Use previous image to skip computation when only the limit is changing
* **Escape time algorithm**
  * [x] Check if a a pixel lies within the cardioid or the period-2 bulb in order to skip computation
  * [x] Use an algebraic trick to minimize the number of multiplications  in the while loop (3 instead of 5)
* **Perturbation Theory**
  * [ ] Use Perturbation Theory rather than Escape Time algorithm when a reference point is given
  * [ ] Find a reference point automatically
  * [ ] Use binary search with a power series approximation in order to find the iteration for which it diverges
