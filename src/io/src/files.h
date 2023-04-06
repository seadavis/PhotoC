#include <filesystem>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

using namespace std;
using namespace cv;

namespace io{
    
    /**
     * Saves the mat to the given
     * filepath. Saves it as a png.
     * 
     * Does not guarantee that the file is a .png.
     * If the OS requires it, then this will throw an exception
     * 
     * if no exceptions thrown then the save worked
    */
    void save(const Mat&, const string& filepath);
}

