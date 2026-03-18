# Path Tracer y Photon Mapper

Motores de renderizado 3D que simulan la iluminación global mediante técnicas de **Path Tracing** y **Photon Mapping**.

Soporta luces puntuales y de área, materiales con efectos de reflexión y refracción, y escenas personalizadas.

## Autores
- Boleslaw Wozniak, Dorian
- Seral Gracia, Álvaro

## Path Tracer

### Escena con varios planetas y un cohete de distintos materiales, con una luz puntual y varias de área
<p align="center">
  <img src="images/path_tracer_planetarium.png" width="90%">
</p>

### Escena con varios juguetes de distintos materiales, con una luz puntual
<p align="center">
  <img src="images/path_tracer_toys.png" width="90%">
</p>

### Cornell Box con dos esferas difusas
En la izquierda, con una luz puntual; en la derecha, con una luz de área.
<p align="center">
  <img src="images/path_tracer_diffuse_puntual.png" width="45%">
  <img src="images/path_tracer_diffuse_area.png" width="45%">
</p>

### Cornell Box con una esfera plástica y una dieléctrica
En la izquierda, con una luz puntual; en la derecha, con una luz de área.
<p align="center">
  <img src="images/path_tracer_complete_puntual.png" width="45%">
  <img src="images/path_tracer_complete_area.png" width="45%">
</p>

### Otras geometrías
En la izquierda, con cilindros finitos; en el centro, con conos finitos; en la derecha, con polígonos convexos personalizados.
<p align="center">
  <img src="images/path_tracer_scene_cylinder_puntual.png" width="30%">
  <img src="images/path_tracer_cone_puntual.png" width="30%">
  <img src="images/path_tracer_custom2D_puntual.png" width="30%">
</p>

### Profundidad de campo
En la izquierda, con una apertura de cámara de 0.3 y una distancia focal enfocando la esfera más cercana; en la derecha, con una apertura de cámara de 0.6 y una distancia focal enfocando la esfera más cercana.
<p align="center">
  <img src="images/path_tracer_depth_puntual_1.png" width="45%">
  <img src="images/path_tracer_depth_puntual_2.png" width="45%">
</p>

## Photon Mapper

### Escena con varios juguetes de distintos materiales, con Next Event Estimation
<p align="center">
  <img src="images/photon_mapper_toys_with_nee.png" width="90%">
</p>

### Escena con varios juguetes de distintos materiales, sin Next Event Estimation
<p align="center">
  <img src="images/photon_mapper_toys_without_nee.png" width="90%">
</p>

### Cornell Box con dos esferas difusas
En la izquierda, sin Next Event Estimation; en la derecha, con Next Event Estimation.
<p align="center">
  <img src="images/photon_mapper_diffuse_without_nee.png" width="45%">
  <img src="images/photon_mapper_diffuse_with_nee.png" width="45%">
</p>

### Cornell Box con una esfera plástica y una dieléctrica
En la izquierda, sin Next Event Estimation; en la derecha, con Next Event Estimation.
<p align="center">
  <img src="images/photon_mapper_complete_without_nee.png" width="45%">
  <img src="images/photon_mapper_complete_with_nee.png" width="45%">
</p>

### Filtrado de fotones por superficie
En la izquierda, no se filtran los fotones; en la derecha, sí se filtran los fotones.
<p align="center">
  <img src="images/photon_mapper_filter.png" width="90%">
</p>

### Kernels sofisticados
En la fila de arriba se encuentra la imagen relativa al kernel Uniforme como base para comparar el resto. <br>
En la fila de abajo, por orden de izquierda a derecha: kernels Triangular, Epanechnikov, Gaussiano
y Coseno.
<p align="center">
  <img src="images/photon_mapper_kernels.png" width="90%">
</p>
