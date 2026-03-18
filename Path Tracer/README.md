# Informática Gráfica: Path Tracer

## Authors
- Dorian Boleslaw Wozniak (817570@unizar.es)
- Álvaro Seral Gracia (819425@unizar.es)

## Description

This project implements a path tracing.

## Compilation

You need make and a C++17 compiler.

To compile the project:

```bash
make
make debug      # for debug build (-g -DDEBUG)
make release    # for release build (-O2 -DNDEBUG)
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

-m, --mode:             raytracer, direct_lighting, pathtracer (default: pathtracer)
-w, --width:            image width (default: 256)
-h, --height:           image height (default: 256)
-f, --fov:              field of view (default: 36.8699)
-p, --paths:            number of light paths sampled per pixel (default: 30)
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
