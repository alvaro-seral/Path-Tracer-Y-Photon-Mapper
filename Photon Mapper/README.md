# Informática Gráfica: Photon Mapping

## Authors
- Dorian Boleslaw Wozniak (817570@unizar.es)
- Álvaro Seral Gracia (819425@unizar.es)

## Description

This project implements a photon mapper.

## Compilation

You need make and a C++17 compiler.

To compile the project:

```bash
make
make debug      # for debug build (-g -DDEBUG)
make release    # for release build (-O3 -DNDEBUG)
```

To clean the project:

```bash
make clean
```

## Usage

To run the program:

```bash
./path_tracer
```

By default, the program generates a 256x256 image in `output.ppm`.

You can pass arguments to the program:

```
-h, --help:             Show this message

-w, --width:            image width (default: 256)
-h, --height:           image height (default: 256)
-f, --fov:              field of view (default: 36.8699)
-p, --paths:            number of light paths sampled per pixel (default: 30)
-n, --num_walks:        number of photon walks (default: 100000)
-mn, --max_nearest:     number of maximum nearest neighbours (default: 100)
-mr, --max_radius:      number of maximum radius for neighbours (default: 0.15)
-wne, --with_next_event:     stablish method to compute direct light with next event estimation
-k, --kernel:           UNIFORM, TRIANGULAR, EPANECHNIKOV, GAUSSIAN, COSINE (default: UNIFORM)
-a, --aperture:         camera aperture (default: 0.0)
-d, --focal_distance:   camera focal distance (default: 3.25)
-o, --output:           output file (default: output.ppm)

-v, --verbose:          verbose output
```

## Documentation

You can generate the documentation with Doxygen using the included Doxyfile:

```bash
doxygen
```
The output will be in the `docs` directory in HTML.

## Modules

The project is subdivided into the following modules:

- `file`: File readers and writers
- `geometry`: Basic geometry: coordinates and rays
- `illumination`: Light sources and illumination models
- `image`: Image representation and manipulation
- `math_structures`: Math structures: vectors and matrices
- `math_transformations`: Affine transformations and change of basis on math structures
- `primitives`: Geometric primitives, such as spheres and planes
- `materials`: Emission, diffuse, specular and refractive materials
- `render`: Rendering algorithms and scene components
- `utils`: Other miscellaneous utilities
- `photon`: Definition of a photon