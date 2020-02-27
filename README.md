# PathTracer

Path tracer written in C++ from scratch.  
Latest release: https://github.com/agavinm/PathTracer/releases/latest  
  
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
![contest](https://user-images.githubusercontent.com/37375662/75463325-eeca9e00-5985-11ea-96a0-580e748747d7.jpeg "contest")
![noEmitters](https://user-images.githubusercontent.com/37375662/75463683-71535d80-5986-11ea-9952-56f70e915e5d.jpeg "noEmitters")
![default](https://user-images.githubusercontent.com/37375662/75463697-744e4e00-5986-11ea-885f-b57f76caaf56.jpeg "default")
![circle](https://user-images.githubusercontent.com/37375662/75463724-7ca68900-5986-11ea-80e4-ce20075ab043.jpeg "circle")
![dna](https://user-images.githubusercontent.com/37375662/75463734-7fa17980-5986-11ea-87bd-30d7051adfdb.jpeg "dna")
![refraction](https://user-images.githubusercontent.com/37375662/75463764-89c37800-5986-11ea-8da7-855762e6a39f.jpeg "refraction")
![specular](https://user-images.githubusercontent.com/37375662/75463781-8cbe6880-5986-11ea-9b53-b2243675054f.jpeg "specular") 
