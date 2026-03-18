/**
 * @file Custom2D.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Custom2D.hh"

#include <cmath>
#include <cassert>

Custom2D::Custom2D(const std::vector<Point>& points, const Direction& normal,
                   const Material& material) :
    Primitive(material),
    points(points),
    normal(normal.norm())
{
    assert(points.size() >= 3);
}

double Custom2D::intersects(const Ray& ray) const
{
    double denominator = ray.direction * normal;

    // Si el rayo es paralelo al plano, no hay intersección
    if (std::abs(denominator) < EPSILON)
        return INFINITY;

    // Calcular el centroide del polígono
    Point centroid(0, 0, 0);
    for (const auto& p : points) {
        centroid = centroid + p;
    }
    centroid = centroid / (double)points.size();

    // Calcular la distancia del plano al origen 
    double distance = -(normal * centroid);

    double numerator = (distance + ray.origin * normal);
    double t = -numerator / denominator;

    // Calcular el punto de intersección en el espacio 3D
    Point intersection = ray.origin + ray.direction * t;

    // Elegir dos ejes del plano para proyectar el polígono a 2D
    // Se crea la proyección ignorando el eje donde la normal tiene mayor componente
    int u, v;
    double abs_x = std::abs(normal.x);
    double abs_y = std::abs(normal.y);
    double abs_z = std::abs(normal.z);
    if (abs_x > abs_y && abs_x > abs_z) { // Se usa YZ
        u = 1; 
        v = 2; 
    } else if (abs_y > abs_z) { // Se usa XZ
        u = 0; 
        v = 2; 
    } else { // Se usa XY
        u = 0; 
        v = 1; 
    }

    auto get2D = [&](const Point& p) {
        return std::pair<double, double>{p[u], p[v]};
    };

    // Proyectar el punto de la intersección a 2D
    double px = intersection[u];
    double py = intersection[v];

    // Algoritmo Point in Polygon (Ray Casting)
    // https://medium.com/%40girishajmera/exploring-algorithms-to-determine-points-inside-or-outside-a-polygon-038952946f87
    // https://www.geeksforgeeks.org/point-in-polygon-in-cpp/
    int count = 0;
    int n = points.size();
    for (int i = 0; i < n; i++) {

        Point a = points[i];
        double ax = a[u];
        double ay = a[v];

        Point b = points[(i + 1) % n];
        double bx = b[u];
        double by = b[v];

        // Comprobar si una línea del polígono cruza una línea horizontal desde el punto de intersección
        if (((ay > py) != (by > py)) &&
            (px < (bx - ax) * (py - ay) / (by - ay) + ax)) {
            count++;
        }
        
    }

    // Si el número de cruces es impar, el punto está dentro del polígono
    if (count % 2 == 1)
        return t;
    else
        return INFINITY;
}

Direction Custom2D::normalTo([[maybe_unused]] const Point& point) const
{
    return normal;
}