#include <filesystem>
#include <iostream>
#include <fstream>
#include "files.h"

namespace io{

    void save(const Mat& m, const string& filepath)
    {
        auto path = fs::path(filepath);
        vector<uchar> output_bytes;
        imencode(".png", m, output_bytes);

        {
            std::ofstream ostrm(path, std::ios::binary);
            ostrm.write(reinterpret_cast<char*>(output_bytes.data()), output_bytes.size()); // binary output
        }

    }
}
