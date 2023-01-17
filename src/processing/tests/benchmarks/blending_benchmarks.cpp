#include <benchmark/benchmark.h>
#include "processing.h"
#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace processing;
using namespace std;

Mat loadBackgroundImage(string path)
{
  auto backgroundImageRGB = imread(path, CV_LOAD_IMAGE_UNCHANGED);

  Mat backgroundImage;
  cvtColor(backgroundImageRGB, backgroundImage, CV_BGR2BGRA);
  return backgroundImage;
}

static void BM_CompositeCalculations(benchmark::State& state) {

    auto backgroundImagePath = "./src/processing/tests/target_images/beach_large.png";
    auto backgroundImage = loadBackgroundImage(backgroundImagePath);

    auto mask = "./src/processing/tests/masks/eagle_mask_large.png";
    auto original = "./src/processing/tests/original_source_images/eagle_large.png";

    auto canvas = CompositeCanvas();
    canvas.setSize(2400, 1600);
    canvas.setBackground(backgroundImage);
    canvas.setComposite(mask, original);

    Mat result;

    for (auto _ : state) {
        // This code gets timed
        result = canvas.currentImg();
    }

    result = canvas.currentImg();
    string fileName =  "large_eagle_results.png";
    imwrite( "./src/processing/tests/benchmark_results/" + fileName, result);

}


// Register the function as a benchmark
BENCHMARK(BM_CompositeCalculations);


BENCHMARK_MAIN();