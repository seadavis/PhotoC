#include <filesystem>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


namespace io{
    enum SaveResult{
        Success,
        FileExists,
        IncorrectPermissions,
        Error
    };

    /**
     * Saves the mat to the given
     * filepath
     * 
     * if true is given to overwrite then if a file
     * of the same name is found the file is overwritten
    */
    SaveResult save(Mat, string filepath, bool overwrite);
}

