/**
 * @file main.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "file/PPM.hh"
#include "materials/Material.hh"
#include "primitives/Plane.hh"
#include "primitives/Primitive.hh"
#include "primitives/Sphere.hh"
#include "primitives/Custom2D.hh"
#include "primitives/Cylinder.hh"
#include "primitives/Cone.hh"
#include "render/Camera.hh"
#include "render/Render.hh"
#include "render/Scene.hh"
#include "utils/Logger.hh"
#include "utils/Stopwatch.hh"

#include <iostream>

// No borrar ID_COUNTER, se usa para generar identidades de nuevas primitivas, 
// que a su vez se usan para identificar superficies en el mapa de fotones
unsigned int Primitive::ID_COUNTER = 0;

// ----- ARGUMENTS -------------------------------------------------------------

const std::string USAGE =
    R"(Usage: ./photon_mapper [OPTION...]

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
    )";

/// @brief List of parameters
struct Arguments {
public:
    unsigned int       width = 256;
    unsigned int       height = 256;
    double             fov = 36.8699;
    unsigned int       paths = 30;
    unsigned int       num_walks = 100000;
    unsigned int       max_nearest = 100;
    double             max_radius = 0.15; // El maximo suele estar entre 0.1 y 0.05
    bool               with_next_event = false; // only photon, sin next event estimation
    std::string        kernel = "UNIFORM"; 
    double             aperture = 0.0;
    double             focal_distance = 3.25;
    std::string        output = "output.ppm";

    Arguments(int argc, char** argv)
    {
        if (argc > 1
            && (std::string(argv[1]) == "-h"
                || std::string(argv[1]) == "--help")) {
            std::cout << USAGE << std::endl;
            exit(0);
        }

        for (int i = 1; i < argc; i++) {
            if (std::string(argv[i]) == "-w"
                || std::string(argv[i]) == "--width") {
                this->width = std::stoi(argv[++i]);
            } else if (std::string(argv[i]) == "-h"
                       || std::string(argv[i]) == "--height") {
                this->height = std::stoi(argv[++i]);
            } else if (std::string(argv[i]) == "-f"
                       || std::string(argv[i]) == "--fov") {
                this->fov = std::stod(argv[++i]);
            } else if (std::string(argv[i]) == "-p"
                       || std::string(argv[i]) == "--paths") {
                this->paths = std::stoi(argv[++i]);
            } else if (std::string(argv[i]) == "-n"
                       || std::string(argv[i]) == "--num_walks") {
                this->num_walks = std::stoi(argv[++i]);
            } else if (std::string(argv[i]) == "-mn"
                       || std::string(argv[i]) == "--max_nearest") {
                this->max_nearest = std::stoi(argv[++i]);
            } else if (std::string(argv[i]) == "-mr"
                       || std::string(argv[i]) == "--max_radius") {
                this->max_radius = std::stod(argv[++i]);
            } else if (std::string(argv[i]) == "-wne"
                       || std::string(argv[i]) == "--with_next_event") {
                this->with_next_event = true;
            } else if (std::string(argv[i]) == "-k"
                       || std::string(argv[i]) == "--kernel") {
                this->kernel = argv[++i];
            } else if (std::string(argv[i]) == "-a"
                       || std::string(argv[i]) == "--aperture") {
                this->aperture = std::stod(argv[++i]);
            } else if (std::string(argv[i]) == "-d"
                       || std::string(argv[i]) == "--focal_distance") {
                this->focal_distance = std::stod(argv[++i]);
            } else if (std::string(argv[i]) == "-o"
                       || std::string(argv[i]) == "--output") {
                this->output = argv[++i];
            } else if (std::string(argv[i]) == "-v"
                       || std::string(argv[i]) == "--verbose") {
#ifdef DEBUG
                Logger::output_level = Logger::L_DEBUG;
#else
                Logger::output_level = Logger::L_INFO;
#endif
            } else {
                Logger::error("Invalid argument: " + std::string(argv[i]));
                exit(1);
            }
        }
    }

    std::string toString()
    {
        return "{\n"
               "  width: "
            + std::to_string(width)
            + ",\n"
              "  height: "
            + std::to_string(height)
            + ",\n"
              "  fov: "
            + std::to_string(fov)
            + ",\n"
              "  paths: "
            + std::to_string(paths)
            + ",\n"
              "  num_walks: "
            + std::to_string(num_walks)
            + ",\n"
              "  max_nearest: "
            + std::to_string(max_nearest)
            + ",\n"
              "  max_radius: "
            + std::to_string(max_radius)
            + ",\n"
              "  aperture: "
            + std::to_string(aperture)
            + ",\n"
              "  focal_distance: "
            + std::to_string(focal_distance)
            + ",\n"
              "  output: "
            + output
            + "\n"
              "}";
    }
};

// ----- SCENES ----------------------------------------------------------------

[[maybe_unused]] const RGB BLACK = {0, 0, 0};
[[maybe_unused]] const RGB LIGHTGREY = {0.74, 0.74, 0.74};
[[maybe_unused]] const RGB GREY = {0.49, 0.49, 0.49};
[[maybe_unused]] const RGB DARKGREY = {0.24, 0.24, 0.24};
[[maybe_unused]] const RGB WHITE = {0.99, 0.99, 0.99};
[[maybe_unused]] const RGB RED = {0.99, 0, 0};
[[maybe_unused]] const RGB GREEN = {0, 0.99, 0};
[[maybe_unused]] const RGB BLUE = {0, 0, 0.99};
[[maybe_unused]] const RGB YELLOW = {0.99, 0.99, 0.49};
[[maybe_unused]] const RGB PINK = {0.99, 0.49, 0.99};
[[maybe_unused]] const RGB CYAN = {0.49, 0.99, 0.99};
[[maybe_unused]] const RGB ORANGE = {0.99, 0.49, 0.24};

Scene scene_diffuse_puntual = Scene(
    {
        std::make_shared<Plane>(1, Direction(1, 0, 0),
                                Material::Diffuse(RED)), // Left
        std::make_shared<Plane>(1, Direction(-1, 0, 0),
                                Material::Diffuse(GREEN)), // Right
        std::make_shared<Plane>(1, Direction(0, 1, 0),
                                Material::Diffuse(LIGHTGREY)), // Floor
        std::make_shared<Plane>(1, Direction(0, -1, 0),
                                Material::Diffuse(LIGHTGREY)), // Ceiling
        std::make_shared<Plane>(1, Direction(0, 0, -1),
                                Material::Diffuse(LIGHTGREY)), // Back
                                
        std::make_shared<Sphere>(Point(-0.5, -0.7, 0.25), 0.3,
                                 Material::Diffuse(PINK)), // Left sphere
        std::make_shared<Sphere>(Point(0.5, -0.7, -0.25), 0.3,
                                 Material::Diffuse(CYAN)) // Right sphere
    },
    {
        std::make_shared<PointLight>(Point(0, 0.5, 0), WHITE), // Light
    });

Scene scene_diffuse_black_walls_white_ball = Scene(
    {
        std::make_shared<Plane>(1, Direction(0, 1, 0),
                                Material::Diffuse(LIGHTGREY)),
        std::make_shared<Sphere>(Point(0.0, -0.6, 0.0), 0.4,
                                 Material::Diffuse(LIGHTGREY)),

        // std::make_shared<Sphere>(Point(-0.7, 0.3, 0.0), 0.2,
        //                          Material::Emission(LIGHTGREY)),
    },
    {
        std::make_shared<PointLight>(Point(-0.7, 0.3, 0.0),
                                     WHITE), // Light
    });

Scene scene_color_bleeding = Scene(
    {
        std::make_shared<Plane>(1, Direction(1, 0, 0),
                                Material::Diffuse(BLUE)), // Left
        std::make_shared<Plane>(1, Direction(-1, 0, 0),
                                Material::Diffuse(RED)), // Right
        std::make_shared<Plane>(1, Direction(0, 1, 0),
                                Material::Diffuse(GREEN)), // Floor
        std::make_shared<Plane>(1, Direction(0, -1, 0),
                                Material::Diffuse(LIGHTGREY)), // Ceiling
        std::make_shared<Plane>(1, Direction(0, 0, -1),
                                Material::Diffuse(LIGHTGREY)), // Back

        std::make_shared<Sphere>(Point(-0.5, -0.7, 0.25), 0.3,
                                 Material::Diffuse(LIGHTGREY)), // Left sphere
        std::make_shared<Sphere>(Point(0.5, -0.7, -0.25), 0.3,
                                 Material::Diffuse(LIGHTGREY)) // Right sphere
    },
    {
        std::make_shared<PointLight>(Point(0, 0.5, 0), WHITE), // Light
    });

Scene scene_diffuse_many_balls = Scene(
    {
        std::make_shared<Plane>(1, Direction(1, 0, 0), Material::Diffuse(RED)),
        std::make_shared<Plane>(1, Direction(-1, 0, 0), Material::Diffuse(GREEN)),
        std::make_shared<Plane>(1, Direction(0, 1, 0),
                                Material::Diffuse(LIGHTGREY)),
        std::make_shared<Plane>(1, Direction(0, -1, 0),
                                Material::Diffuse(LIGHTGREY)),
        std::make_shared<Plane>(1, Direction(0, 0, -1),
                                Material::Diffuse(LIGHTGREY)),

        std::make_shared<Sphere>(Point(-0.5, -0.7, 0.25), 0.3,
                                 Material::Diffuse(PINK)),
        std::make_shared<Sphere>(Point(0.5, -0.7, -0.25), 0.3,
                                 Material::Diffuse(CYAN)),
        std::make_shared<Sphere>(Point(-0.7, -0.85, -0.6), 0.15,
                                 Material::Diffuse(PINK)),
        std::make_shared<Sphere>(Point(0.6, 0.1, 0.6), 0.2,
                                 Material::Diffuse(CYAN)),
        std::make_shared<Sphere>(Point(-0.3, -0.4, 0.75), 0.1,
                                 Material::Diffuse(CYAN)),
        std::make_shared<Sphere>(Point(0.35, 0.2, -0.6), 0.12,
                                 Material::Diffuse(PINK)),
        std::make_shared<Sphere>(Point(-0.7, 0.4, 0.3), 0.1,
                                 Material::Diffuse(CYAN)),
        std::make_shared<Sphere>(Point(0.65, 0.6, 0.15), 0.08,
                                 Material::Diffuse(PINK)),
        std::make_shared<Sphere>(Point(0.0, 0.3, -0.75), 0.14,
                                 Material::Diffuse(CYAN)),
        std::make_shared<Sphere>(Point(-0.65, 0.1, 0.6), 0.18,
                                 Material::Diffuse(PINK))
    },
    {
        std::make_shared<PointLight>(Point(0, 0.5, 0), WHITE),
    });

Scene scene_specular_many_balls = Scene(
    {
        std::make_shared<Plane>(1, Direction(1, 0, 0), Material::Diffuse(RED)),
        std::make_shared<Plane>(1, Direction(-1, 0, 0), Material::Diffuse(GREEN)),
        std::make_shared<Plane>(1, Direction(0, 1, 0),
                                Material::Diffuse(LIGHTGREY)),
        std::make_shared<Plane>(1, Direction(0, -1, 0),
                                Material::Diffuse(LIGHTGREY)),
        std::make_shared<Plane>(1, Direction(0, 0, -1),

                                Material::Diffuse(LIGHTGREY)),
        std::make_shared<Sphere>(Point(-0.5, -0.7, 0.25), 0.3,
                                 Material::Specular(0.7)),
        std::make_shared<Sphere>(Point(0.5, -0.7, -0.25), 0.3,
                                 Material::Specular(0.7)),
        std::make_shared<Sphere>(Point(-0.7, -0.85, -0.6), 0.15,
                                 Material::Specular(0.7)),
        std::make_shared<Sphere>(Point(0.6, 0.1, 0.6), 0.2,
                                 Material::Specular(0.7)),
        std::make_shared<Sphere>(Point(-0.3, -0.4, 0.75), 0.1,
                                 Material::Specular(0.7)),
        std::make_shared<Sphere>(Point(0.35, 0.2, -0.6), 0.12,
                                 Material::Specular(0.7)),
        std::make_shared<Sphere>(Point(-0.7, 0.4, 0.3), 0.1,
                                 Material::Specular(0.7)),
        std::make_shared<Sphere>(Point(0.65, 0.6, 0.15), 0.08,
                                 Material::Specular(0.7)),
        std::make_shared<Sphere>(Point(0.0, 0.3, -0.75), 0.14,
                                 Material::Specular(0.7)),
        std::make_shared<Sphere>(Point(-0.65, 0.1, 0.6), 0.18,
                                 Material::Specular(0.7))
    },
    {
        std::make_shared<PointLight>(Point(0, 0.5, 0), WHITE), // Light
    });

Scene scene_refractive_many_balls = Scene(
    {
        std::make_shared<Plane>(1, Direction(1, 0, 0), Material::Diffuse(RED)),
        std::make_shared<Plane>(1, Direction(-1, 0, 0), Material::Diffuse(GREEN)),
        std::make_shared<Plane>(1, Direction(0, 1, 0),
                                Material::Diffuse(LIGHTGREY)),
        std::make_shared<Plane>(1, Direction(0, -1, 0),
                                Material::Diffuse(LIGHTGREY)),
        std::make_shared<Plane>(1, Direction(0, 0, -1),
                                Material::Diffuse(LIGHTGREY)),

        std::make_shared<Sphere>(Point(-0.5, -0.7, 0.25), 0.3,
                                Material::Refractive(0.7, 1.2)),
        std::make_shared<Sphere>(Point(0.5, -0.7, -0.25), 0.3,
                                 Material::Refractive(0.7, 1.2)),
        std::make_shared<Sphere>(Point(-0.7, -0.85, -0.6), 0.15,
                                 Material::Refractive(0.7, 1.2)),
        std::make_shared<Sphere>(Point(0.6, 0.1, 0.6), 0.2,
                                 Material::Refractive(0.7, 1.2)),
        std::make_shared<Sphere>(Point(-0.3, -0.4, 0.75), 0.1,
                                 Material::Refractive(0.7, 1.2)),
        std::make_shared<Sphere>(Point(0.35, 0.2, -0.6), 0.12,
                                 Material::Refractive(0.7, 1.2)),
        std::make_shared<Sphere>(Point(-0.7, 0.4, 0.3), 0.1,
                                 Material::Refractive(0.7, 1.2)),
        std::make_shared<Sphere>(Point(0.65, 0.6, 0.15), 0.08,
                                 Material::Refractive(0.7, 1.2)),
        std::make_shared<Sphere>(Point(0.0, 0.3, -0.75), 0.14,
                                 Material::Refractive(0.7, 1.2)),
        std::make_shared<Sphere>(Point(-0.65, 0.1, 0.6), 0.18,
                                 Material::Refractive(0.7, 1.2))
    },
    {
        std::make_shared<PointLight>(Point(0, 0.5, 0), WHITE),
    });

Scene scene_diffuse_puntual_many_lights = Scene(
    {
        std::make_shared<Plane>(1, Direction(1, 0, 0), Material::Diffuse(RED)),
        std::make_shared<Plane>(1, Direction(-1, 0, 0), Material::Diffuse(GREEN)),
        std::make_shared<Plane>(1, Direction(0, 1, 0),
                                Material::Diffuse(LIGHTGREY)),
        std::make_shared<Plane>(1, Direction(0, -1, 0),
                                Material::Diffuse(LIGHTGREY)),
        std::make_shared<Plane>(1, Direction(0, 0, -1),
                                Material::Diffuse(LIGHTGREY)),

        std::make_shared<Sphere>(Point(-0.5, -0.7, 0.25), 0.3,
                                 Material::Diffuse(PINK)),
        std::make_shared<Sphere>(Point(0.5, -0.7, -0.25), 0.3,
                                 Material::Diffuse(CYAN))
    },
    {
        std::make_shared<PointLight>(Point(0, 0.5, 0), WHITE),
        // std::make_shared<PointLight>(Point(0.1, 0.5, 0), WHITE),
        // std::make_shared<PointLight>(Point(0.2, 0.5, 0), WHITE),
        // std::make_shared<PointLight>(Point(-0.1, 0.5, 0), WHITE),
        // std::make_shared<PointLight>(Point(-0.2, 0.5, 0), WHITE),
    });

Scene scene_complete_puntual = Scene(
    {
        std::make_shared<Plane>(1, Direction(1, 0, 0),
                                Material::Diffuse(RED)), // Left
        std::make_shared<Plane>(1, Direction(-1, 0, 0),
                                Material::Diffuse(GREEN)), // Right
        std::make_shared<Plane>(1, Direction(0, 1, 0),
                                Material::Diffuse(LIGHTGREY)), // Floor
        std::make_shared<Plane>(1, Direction(0, -1, 0),
                                Material::Diffuse(LIGHTGREY)), // Ceiling
        std::make_shared<Plane>(1, Direction(0, 0, -1),
                                Material::Diffuse(LIGHTGREY)), // Back

        std::make_shared<Sphere>(Point(-0.5, -0.7, 0.25), 0.3,
                                 Material::Plastic(CYAN * 0.8, 0.19)), // Left sphere
        std::make_shared<Sphere>(Point(0.5, -0.7, -0.25), 0.3,
                                 Material::Dielectric(0.19, 0.79, 1.5)) // Right sphere
    },
    {
        std::make_shared<PointLight>(Point(0, 0.5, 0), WHITE), // Light
    });

Scene scene_others_puntual = Scene(
    {
        std::make_shared<Plane>(1, Direction(1, 0, 0),
                                Material::Diffuse(RED)), // Left
        std::make_shared<Plane>(1, Direction(-1, 0, 0),
                                Material::Diffuse(GREEN)), // Right
        std::make_shared<Plane>(1, Direction(0, 1, 0),
                                Material::Diffuse(LIGHTGREY)), // Floor
        std::make_shared<Plane>(1, Direction(0, -1, 0),
                                Material::Diffuse(LIGHTGREY)), // Ceiling
        std::make_shared<Plane>(1, Direction(0, 0, -1),
                                Material::Diffuse(LIGHTGREY)), // Back

        std::make_shared<Sphere>(
            Point(-0.5, -0.7, 0.25), 0.3,
            Material::Specular(0.99)), // Left sphere
        std::make_shared<Sphere>(
            Point(0.5, -0.7, -0.25), 0.3,
            Material::Refractive(0.99, 1.5)), // Right sphere
        std::make_shared<Sphere>(
            Point(0, -0.8, 0), 0.2,
            Material::Dielectric(BLUE * 0.2, 0, 0.79, 1.5)), // Center sphere
    },
    {
        std::make_shared<PointLight>(Point(0, 0.5, 0), WHITE), // Light
    });


Scene scene_cylinder_puntual = Scene(
    {
        std::make_shared<Plane>(1, Direction(1, 0, 0),
                                Material::Diffuse(RED)), // Left
        std::make_shared<Plane>(1, Direction(-1, 0, 0),
                                Material::Diffuse(GREEN)), // Right
        std::make_shared<Plane>(1, Direction(0, 1, 0),
                                Material::Diffuse(LIGHTGREY)), // Floor
        std::make_shared<Plane>(1, Direction(0, -1, 0),
                                Material::Diffuse(LIGHTGREY)), // Ceiling
        std::make_shared<Plane>(1, Direction(0, 0, -1),
                                Material::Diffuse(LIGHTGREY)), // Back

        std::make_shared<Cylinder>(
            Point(-0.5, -0.4, 0.25), 0.3, 0.6,
            Direction(0, -1, 0),
            Material::Plastic(CYAN * 0.8, 0.19)), // Left bottom cylinder
        std::make_shared<Cylinder>(
            Point(0.5, -0.4, -0.25), 0.3, 0.6,
            Direction(0, -1, 0),
            Material::Dielectric(0.19, 0.79, 1.5)), // Right bottom cylinder
        std::make_shared<Cylinder>(
            Point(-0.5, 0.4, -0.25), 0.3, 0.6,
            Direction(0, 1, 0),
            Material::Dielectric(BLUE * 0.2, 0, 0.79, 1.5)), // Left top cylinder
        std::make_shared<Cylinder>(
            Point(0.5, 0.4, 0.25), 0.3, 0.6,
            Direction(0, 1, 0),
            Material::Diffuse(PINK)), // Right top cylinder
    },
    {
        std::make_shared<PointLight>(Point(0, 0.5, 0), WHITE), // Light
    });


Scene scene_cone_puntual = Scene(
    {
        std::make_shared<Plane>(1, Direction(1, 0, 0),
                                Material::Diffuse(RED)), // Left
        std::make_shared<Plane>(1, Direction(-1, 0, 0),
                                Material::Diffuse(GREEN)), // Right
        std::make_shared<Plane>(1, Direction(0, 1, 0),
                                Material::Diffuse(LIGHTGREY)), // Floor
        std::make_shared<Plane>(1, Direction(0, -1, 0),
                                Material::Diffuse(LIGHTGREY)), // Ceiling
        std::make_shared<Plane>(1, Direction(0, 0, -1),
                                Material::Diffuse(LIGHTGREY)), // Back

        std::make_shared<Cone>(
            Point(-0.5, -0.4, 0.25), 0.3, 0.6,
            Direction(0, 1, 0),
            Material::Plastic(CYAN * 0.8, 0.19)), // Left bottom cylinder
        std::make_shared<Cone>(
            Point(0.5, -0.4, -0.25), 0.3, 0.6,
            Direction(0, 1, 0),
            Material::Dielectric(0.19, 0.79, 1.5)), // Right bottom cylinder
        std::make_shared<Cone>(
            Point(-0.5, 0.4, -0.25), 0.3, 0.6,
            Direction(0, -1, 0),
            Material::Dielectric(BLUE * 0.2, 0, 0.79, 1.5)), // Left top cylinder
        std::make_shared<Cone>(
            Point(0.5, 0.4, 0.25), 0.3, 0.6,
            Direction(0, -1, 0),
            Material::Diffuse(PINK)), // Right top cylinder
    },
    {
        std::make_shared<PointLight>(Point(0, 0.5, 0), WHITE), // Light
    });


Scene scene_custom2D_puntual = Scene(
    {
        std::make_shared<Plane>(1, Direction(1, 0, 0),
                                Material::Diffuse(RED)), // Left
        std::make_shared<Plane>(1, Direction(-1, 0, 0),
                                Material::Diffuse(GREEN)), // Right
        std::make_shared<Plane>(1, Direction(0, 1, 0),
                                Material::Diffuse(LIGHTGREY)), // Floor
        std::make_shared<Plane>(1, Direction(0, -1, 0),
                                Material::Diffuse(LIGHTGREY)), // Ceiling
        std::make_shared<Plane>(1, Direction(0, 0, -1),
                                Material::Diffuse(LIGHTGREY)), // Back

        std::make_shared<Custom2D>(
            std::vector<Point>{
                Point(-0.7, -0.6, 0.05),
                Point(-0.4, -0.2, 0.35),
                Point(-0.1, -0.6, 0.65),},
            Direction(1, 0, -1),
            Material::Plastic(CYAN * 0.8, 0.19)), // Left bottom cylinder
        std::make_shared<Custom2D>(
            std::vector<Point>{
                Point(0.7, -0.6, -0.05),
                Point(0.4, -0.2, -0.35),
                Point(0.1, -0.6, -0.65),},
            Direction(1, 0, -1),
            Material::Dielectric(0.19, 0.79, 1.5)), // Right bottom cylinder
        std::make_shared<Custom2D>(
            std::vector<Point>{
                Point(-0.7, 0.6, -0.05),
                Point(-0.4, 0.2, -0.35),
                Point(-0.1, 0.6, -0.65),},
            Direction(1, 0, -1),
            Material::Dielectric(BLUE * 0.2, 0, 0.79, 1.5)), // Left top cylinder
        std::make_shared<Custom2D>(
            std::vector<Point>{
                Point(0.7, 0.6, 0.05),
                Point(0.4, 0.2, 0.35),
                Point(0.1, 0.6, 0.65),},
            Direction(1, 0, -1),
            Material::Diffuse(PINK)), // Right top cylinder
    },
    {
        std::make_shared<PointLight>(Point(0, 0.5, 0), WHITE), // Light
    });

Scene scene_complete_puntual_no_caustic = Scene(
    {
        std::make_shared<Plane>(1, Direction(1, 0, 0),
                                Material::Diffuse(RED)), // Left
        std::make_shared<Plane>(1, Direction(-1, 0, 0),
                                Material::Diffuse(GREEN)), // Right
        std::make_shared<Plane>(1, Direction(0, 1, 0),
                                Material::Diffuse(LIGHTGREY)), // Floor
        std::make_shared<Plane>(1, Direction(0, -1, 0),
                                Material::Diffuse(LIGHTGREY)), // Ceiling
        std::make_shared<Plane>(1, Direction(0, 0, -1),
                                Material::Diffuse(LIGHTGREY)), // Back

        std::make_shared<Sphere>(Point(-0.5, -0.7, 0.25), 0.3,
                                 Material::Specular(0.99)),
        std::make_shared<Sphere>(Point(0.5, -0.7, -0.25), 0.3,
                                 Material::Dielectric(0.19, 0.79, 1.5)) // Right sphere
    },
    {
        std::make_shared<PointLight>(Point(0, 0.5, 0), WHITE), // Light
    });

Scene scene_depth_puntual = Scene(
    {
        std::make_shared<Plane>(1, Direction(1, 0, 0),
                                Material::Diffuse(RED)), // Left
        std::make_shared<Plane>(1, Direction(-1, 0, 0),
                                Material::Diffuse(GREEN)), // Right
        std::make_shared<Plane>(1, Direction(0, 1, 0),
                                Material::Diffuse(LIGHTGREY)), // Floor
        std::make_shared<Plane>(1, Direction(0, -1, 0),
                                Material::Diffuse(LIGHTGREY)), // Ceiling
        std::make_shared<Plane>(1, Direction(0, 0, -1),
                                Material::Diffuse(LIGHTGREY)), // Back

        std::make_shared<Sphere>(
            Point(-0.5, -0.7, 0.5), 0.3,
            Material::Plastic(CYAN * 0.8, 0.19)), // Left sphere
        std::make_shared<Sphere>(
            Point(0, -0.7, 0), 0.3,
            Material::Diffuse(PINK)), // Left sphere
        std::make_shared<Sphere>(
            Point(0.5, -0.7, -0.5), 0.3,
            Material::Dielectric(0.19, 0.79, 1.5)) // Right sphere
    },
    {
        std::make_shared<PointLight>(Point(0, 0.5, 0), WHITE), // Light
    });

Scene scene_toys()
{
    std::vector<Point> batman = {
        Point(0, 0, 0),       Point(3, 0, -3),  Point(3, -1, -3),
        Point(4, -1, -4),     Point(4, 0, -4),  Point(4.5, -1, -4.5),
        Point(5.5, -1, -5.5), Point(6, 0, -6),  Point(6, -1, -6),
        Point(7, -1, -7),     Point(7, 0, -7),  Point(10, 0, -10),
        Point(9, -1, -9),     Point(9, -2, -9), Point(6, -2, -6),
        Point(5, -3, -5),     Point(4, -2, -4), Point(1, -2, -1),
        Point(1, -1, -1),
    };

    for (auto& point : batman) {
        point = point * 0.075;               // Scale down the points
        point = point + Point(0, 0.75, 0.5); // Move up, back and right
    }

    return Scene(
        {
            std::make_shared<Plane>(1, Direction(1, 0, 0),
                                     Material::Diffuse(LIGHTGREY)), // Left
            std::make_shared<Plane>(1, Direction(-1, 0, 0),
                                     Material::Diffuse(LIGHTGREY)), // Right
            std::make_shared<Plane>(1, Direction(0, 1, 0),
                                     Material::Diffuse(CYAN)), // Floor
            std::make_shared<Plane>(1, Direction(0, -1, 0),
                                     Material::Diffuse(WHITE)), // Ceiling
            std::make_shared<Plane>(1, Direction(0, 0, -1),
                                     Material::Diffuse(LIGHTGREY)), // Back

            // Dielectric castle
            // Posición Z promedio: -0.25
            std::make_shared<Cylinder>(
                Point(0.5, -1, -0.25), 0.4, 0.2,
                Direction(0, 1, 0),
                Material::Diffuse(GREEN)),
            std::make_shared<Cylinder>(
                Point(0.5, -0.8, -0.25), 0.05, 0.2,
                Direction(0, 1, 0),
                Material::Dielectric(0.19, 0.79, 1.5)),
            std::make_shared<Cone>(
                Point(0.5, -0.6, -0.25), 0.06, 0.06,
                Direction(0, -1, 0),
                Material::Dielectric(0.19, 0.79, 1.5)),
            // Tower 1
            std::make_shared<Cylinder>(
                Point(0.7, -0.8, -0.05), 0.025, 0.1,
                Direction(0, 1, 0),
                Material::Dielectric(0.19, 0.79, 1.5)),
            std::make_shared<Cone>(
                Point(0.7, -0.7, -0.05), 0.035, 0.035,
                Direction(0, -1, 0),
                Material::Dielectric(0.19, 0.79, 1.5)),
            // Tower 2
            std::make_shared<Cylinder>(
                Point(0.7, -0.8, -0.45), 0.025, 0.1,
                Direction(0, 1, 0),
                Material::Dielectric(0.19, 0.79, 1.5)),
            std::make_shared<Cone>(
                Point(0.7, -0.7, -0.45), 0.035, 0.035,
                Direction(0, -1, 0),
                Material::Dielectric(0.19, 0.79, 1.5)),
            // Tower 3
            std::make_shared<Cylinder>(
                Point(0.3, -0.8, -0.05), 0.025, 0.1,
                Direction(0, 1, 0),
                Material::Dielectric(0.19, 0.79, 1.5)),
            std::make_shared<Cone>(
                Point(0.3, -0.7, -0.05), 0.035, 0.035,
                Direction(0, -1, 0),
                Material::Dielectric(0.19, 0.79, 1.5)),
            // Tower 4
            std::make_shared<Cylinder>(
                Point(0.3, -0.8, -0.45), 0.025, 0.1,
                Direction(0, 1, 0),
                Material::Dielectric(0.19, 0.79, 1.5)),
            std::make_shared<Cone>(
                Point(0.3, -0.7, -0.45), 0.035, 0.035,
                Direction(0, -1, 0),
                Material::Dielectric(0.19, 0.79, 1.5)),
            // Walls
            std::make_shared<Custom2D>(
                std::vector<Point>{
                    Point(0.325, -0.8, -0.45),
                    Point(0.325, -0.725, -0.45),
                    Point(0.675, -0.8, -0.45),
                    Point(0.675, -0.725, -0.45)},
                Direction(0, 0, -1),
                Material::Dielectric(0.19, 0.79, 1.5)),
            std::make_shared<Custom2D>(
                std::vector<Point>{
                    Point(0.325, -0.8, -0.05),
                    Point(0.325, -0.725, -0.05),
                    Point(0.675, -0.8, -0.05),
                    Point(0.675, -0.725, -0.05)},
                Direction(0, 0, 1),
                Material::Dielectric(0.19, 0.79, 1.5)),
            std::make_shared<Custom2D>(
                std::vector<Point>{
                    Point(0.3, -0.8, -0.075),
                    Point(0.3, -0.725, -0.075),
                    Point(0.3, -0.8, -0.425),
                    Point(0.3, -0.725, -0.425)},
                Direction(-1, 0, 0),
                Material::Dielectric(0.19, 0.79, 1.5)),
            std::make_shared<Custom2D>(
                std::vector<Point>{
                    Point(0.7, -0.8, -0.075),
                    Point(0.7, -0.725, -0.075),
                    Point(0.7, -0.8, -0.425),
                    Point(0.7, -0.725, -0.425)},
                Direction(1, 0, 0),
                Material::Dielectric(0.19, 0.79, 1.5)),

            // Diffuse icecream
            // Posición Z promedio: -0.6
            std::make_shared<Cylinder>(
                Point(-0.625, 0.45, -0.625), 0.025, 1,
                Direction(0, 1, 0),
                Material::Diffuse(CYAN)),
            std::make_shared<Cone>(
                Point(-0.5, 0.55, -0.5), 0.1, 0.4,
                Direction(1, 1, 1),
                Material::Diffuse(ORANGE)),
            std::make_shared<Sphere>(
                Point(-0.5, 0.55, -0.5), 0.125,
                Material::Diffuse(PINK)),

            // Diffuse batman
            // Posición Z promedio: 0.45
            std::make_shared<Custom2D>(
                batman,
                Direction(-1, 0, -1),
                Material::Diffuse(BLUE)),
            std::make_shared<Cylinder>(
                Point(0.6, 0.75, 0.45), 0.6, 0.3,
                Direction(-1, 0, -1),
                Material::Diffuse(YELLOW)),

            // Plastic Snowman
            // Posición Z promedio: 0.25
            std::make_shared<Cylinder>(
                Point(-0.5, -1, 0.25), 0.3, 0.3,
                Direction(0, 1, 0),
                Material::Diffuse(RED)),
            std::make_shared<Sphere>(
                Point(-0.5, -0.4, 0.25), 0.35,
                Material::Material::Plastic(WHITE * 0.8, 0.19)),
            std::make_shared<Sphere>(
                Point(-0.5, 0.1, 0.25), 0.25,
                Material::Plastic(WHITE * 0.8, 0.19)),
            std::make_shared<Cylinder>(
                Point(-0.5, 0.3, 0.25), 0.35, 0.1,
                Direction(0, 1, 0),
                Material::Plastic(DARKGREY * 0.8, 0.19)),
            std::make_shared<Cylinder>(
                Point(-0.5, 0.4, 0.25), 0.25, 0.35,
                Direction(0, 1, 0),
                Material::Plastic(DARKGREY * 0.8, 0.19)),
            // nose
            std::make_shared<Cone>(
                Point(-0.5, 0.1, 0.25), 0.1, 0.4,
                Direction(-1, 0, 1),
                Material::Plastic(ORANGE * 0.8, 0.19)),
            // eyes
            std::make_shared<Sphere>(
                Point(-0.25, 0.15, 0.25), 0.02,
                Material::Plastic(BLACK * 0.8, 0.19)),
            std::make_shared<Sphere>(
                Point(-0.5, 0.15, 0), 0.02,
                Material::Plastic(BLACK * 0.8, 0.19)),

        },
        {
            std::make_shared<PointLight>(Point(0, 0.5, 0), WHITE), // Light
        });
}

// ----- MAIN ------------------------------------------------------------------

int main(int argc, char** argv)
{
    Logger::output_level = Logger::L_WARNING;
    Arguments p(argc, argv);
    Logger::debug("Parsed arguments: " + p.toString());

    Camera camera({0, 0, -3.5}, {0, 0, -0.5}, p.width, p.height, p.fov);
    Scene scene = scene_complete_puntual;

    Logger::info("Rendering...");
    Stopwatch sw;
    sw.start();
    Image image = Render::render(camera, scene, p.paths, p.num_walks, 
                                 p.max_nearest, p.max_radius, 
                                 p.with_next_event, p.kernel,
                                 p.aperture, p.focal_distance)
                  .equalize()
                  .gamma(2.2);
    sw.stop();
    Logger::info("Rendering took " + sw.toString());
    Logger::info("Writing image...");
    sw.start();
    PPM::write(image, p.output);
    sw.stop();
    Logger::info("Writing took " + sw.toString());

    return EXIT_SUCCESS;
}
