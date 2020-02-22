# PathTracer

Path tracer written in C++ from scratch.  
  
### Usage:
<pre><code>./PathTracer &ltppp> &ltwidth> &ltheight> &ltfile name> &ltscene>
* &ltfile name> needs to be one of
       - .ppm
       - .bmp
OPTIONAL* &ltscene> can be a triangular .ply file or a predefined scene.
Available scenes: circle, contest, default, dna, donut, noEmitters, refraction, report, specular.
</code></pre>

### Example:
<pre><code>~$ ./PathTracer 512 1280 720 images/circle.bmp circle
[INFO] Rendering 1280x720 scene with 512ppp (8 objects) (8 threads)
[INFO] Render finished in 3m 21,476s
[INFO] Storing image as ppm images/circle_hdr.ppm
[INFO] gammaCurve -> [INFO] Clamping image with v=1.000 and applying gamma curve with gamma=4.000
[INFO] Storing image as bmp images/circle.bmp
</code></pre>