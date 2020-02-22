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

### Rendered images:
![alt text](https://github.com/agavinm/PathTracer/blob/master/images/contest.bmp "contest.bmp")  
![alt text](https://github.com/agavinm/PathTracer/blob/master/images/default.bmp "default.bmp")  
![alt text](https://github.com/agavinm/PathTracer/blob/master/images/circle.bmp "circle.bmp")  
![alt text](https://github.com/agavinm/PathTracer/blob/master/images/dna.bmp "dna.bmp")  
![alt text](https://github.com/agavinm/PathTracer/blob/master/images/refraction.bmp "refraction.bmp")  
![alt text](https://github.com/agavinm/PathTracer/blob/master/images/specular.bmp "specular.bmp")  
