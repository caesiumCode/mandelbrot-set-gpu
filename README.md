# mandelbrot-set-gpu
This is a Mandelbrot Set viewer implemented in C++ with [SFML](https://www.sfml-dev.org/). 

![](https://github.com/caesiumCode/mandelbrot-set-gpu/blob/main/examples/mandelbrot-set%5Bdefault%5D%5B0.000000:0.000000:5.000000%5D%5B100%5D.png)
![](https://github.com/caesiumCode/mandelbrot-set-gpu/blob/main/examples/mandelbrot-set%5Bdefault%5D%5B0.337789:0.510188:0.005795%5D%5B1955%5D.png)



You can easily interact with the viewer using the following inputs

| Input | Action |
| ----- | ----- |
| Up | Increase the limit |
| Down | Decrease the limit |
| Left Button + Mouse | Move inside the image |
| Mouse wheel | Zoom/Dezoom inside the image |
| Q | Default mode |
| S | Debug mode |


Here is a list of all the features/optimizations that is planned:
* **Viewer Modes**
  * [x] Default mode: Color according to the divergence speed
  * [x] Data overlay:
    * Display the values of some relevant parameters
    * Draw the reference point
  * [x] Debug mode:
    * Highlight parts of the image that used a rendering optimisation
    * Draw the path used in order to find the reference point
* **Rendering**
  * [x] Use a shader to process each pixel in parallel
  * [x] Use linear interpolation between the last two iterations for continuous color gradient
  * [ ] Use the previous image to skip computation when:
    * [x] the offset is changing
    * [ ] the limit is changing
    * [x] the scale is changing
* **Escape time algorithm**
  * [x] Check if a a pixel lies within the cardioid or the period-2 bulb in order to skip computation
  * [x] Use an algebraic trick to minimize the number of multiplications  in the while loop (3 instead of 5)
* **Perturbation Theory**
  * [ ] Use Perturbation Theory rather than Escape Time algorithm when a reference point is given
  * [x] Use gradient descent on an approximation of the distance function to find a reference point
  * [ ] Use binary search with a power series approximation in order to find the iteration for which it diverges
