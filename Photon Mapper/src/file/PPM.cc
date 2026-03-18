/**
 * @file PPM.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "PPM.hh"
#include "utils/Logger.hh"

#include <fstream>
#include <iomanip>
#include <sstream>

namespace PPM {

Image read(const std::string& path)
{
    std::ifstream in(path);

    if (in.is_open()) {
        Logger::debug("Reading file: " + path);

        enum ReadingState {
            MAGIC_NUMBER,
            WITDH,
            HEIGHT,
            MAX_VALUE,
            DATA_R,
            DATA_G,
            DATA_B
        } state
            = MAGIC_NUMBER;

        unsigned int width = 0, height = 0, disk_max = 0;

        double mem_max = 0;

        std::vector<RGB> data = {};

        std::string line;

        for (std::string line; std::getline(in, line);) {
            double r = 0, g = 0, b = 0;

            if (line.front() != '#') {
                std::stringstream ss(line);

                for (std::string token; ss >> token;) {

                    switch (state) {
                        case MAGIC_NUMBER:
                            if (token == "P3") state = WITDH;
                            else throw WrongMagicNumberException();
                            break;
                        case WITDH:
                            width = std::stoi(token);
                            state = HEIGHT;
                            Logger::debug("Width: " + std::to_string(width));
                            break;
                        case HEIGHT:
                            height = std::stoi(token);
                            data.reserve(width * height);
                            state = MAX_VALUE;
                            Logger::debug("Height: " + std::to_string(height));
                            break;
                        case MAX_VALUE:
                            disk_max = std::stoi(token);
                            state = DATA_R;
                            Logger::debug("Disk max: "
                                          + std::to_string(disk_max));
                            break;
                        case DATA_R:
                            r = std::stoi(token) * (mem_max / disk_max);
                            state = DATA_G;
                            break;
                        case DATA_G:
                            g = std::stoi(token) * (mem_max / disk_max);
                            state = DATA_B;
                            break;
                        case DATA_B:
                            b = std::stoi(token) * (mem_max / disk_max);
                            data.push_back({r, g, b});
                            r = g = b = 0;
                            state = DATA_R;
                            break;
                    }
                }
            } else {
                const std::string MAX_DECL = "#MAX=";

                if (line.find(MAX_DECL) == 0) {
                    mem_max = std::stod(line.substr(MAX_DECL.length()));
                    Logger::debug("Max value: " + std::to_string(mem_max));
                }
            }
        }

        in.close();

        return Image(width, height, data, disk_max, mem_max);
    } else {
        throw ReaderException();
    }
}

void write(const Image& image, const std::string& path)
{
    std::ofstream out(path);

    if (out.is_open()) {
        Logger::debug("Writing file: " + path);

        out << "P3" << std::endl
            << "#MAX=" << image.mem_max << std::endl
            << "# " << path << std::endl
            << image.width << " " << image.height << std::endl
            << image.disk_max << std::endl;

        Logger::debug("Width: " + std::to_string(image.width));
        Logger::debug("Height: " + std::to_string(image.height));
        Logger::debug("Disk max: " + std::to_string(image.disk_max));
        Logger::debug("Max value: " + std::to_string(image.mem_max));

        unsigned int line = 0, col = 0;

        for (const RGB& pixel : image.data) {
            const double FACTOR = image.disk_max / image.mem_max;

            out << std::fixed << std::setprecision(0) << pixel.r() * FACTOR
                << " " << pixel.g() * FACTOR << " " << pixel.b() * FACTOR;

            if (col + 1 < image.width) {
                col++;
                out << "\t";
            } else {
                col = 0;
                line++;
                out << "\n";
            }
        }
    } else {
        throw WriterException();
    }
}

const char* ReaderException::what() { return "Cannot read PPM file"; }

const char* WrongMagicNumberException::what()
{
    return "Wrong magic number in PPM file (expected P3)";
}

const char* WriterException::what() { return "Cannot write PPM file"; }

}
