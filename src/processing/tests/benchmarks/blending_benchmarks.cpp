#include <benchmark/benchmark.h>
#include "processing.h"
#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>
#include <map>
#include <tuple>

using namespace processing;
using namespace std;

Mat loadBackgroundImage(string path)
{
  auto backgroundImageRGB = imread(path, CV_LOAD_IMAGE_UNCHANGED);

  Mat backgroundImage;
  cvtColor(backgroundImageRGB, backgroundImage, CV_BGR2BGRA);
  return backgroundImage;
}

template <class ...Args>
static void BM_CompositeCalculations(benchmark::State& state, Args&&... args) {

    auto args_tuple = make_tuple(move(args)...);
    string backgroundImagePath = "./src/processing/tests/benchmark_data/background/" + string(get<2>(args_tuple));
    auto backgroundImage = loadBackgroundImage(backgroundImagePath);

    auto mask = "./src/processing/tests/benchmark_data/masks/" + string(get<0>(args_tuple));
    auto original = "./src/processing/tests/benchmark_data/source/" + string(get<1>(args_tuple));

    auto canvas = CompositeCanvas();
    canvas.setSize(backgroundImage.size().width, backgroundImage.size().height);
    canvas.setBackground(backgroundImage);
    canvas.setComposite(mask, original);

    Mat result;

    for (auto _ : state) {
        result = canvas.currentImg();
    }

    result = canvas.currentImg();
    string fileName =   "./src/processing/tests/benchmark_results/" + string(get<3>(args_tuple));
    imwrite( fileName, result);

}

BENCHMARK_CAPTURE(BM_CompositeCalculations, 238x193, "eagle_238x193.png", "eagle_238x193.png", "beach_894x596.png", "result_238x192.png" )->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_CompositeCalculations, 300x243, "eagle_300x243.png", "eagle_300x243.png", "beach_894x596.png", "result_300x243.png" )->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_CompositeCalculations, 500x405, "eagle_500x405.png", "eagle_500x405.png", "beach_1500x1000.png", "result_500x405.png" )->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_CompositeCalculations, 700x567, "eagle_700x567.png", "eagle_700x567.png", "beach_1500x1000.png", "result_700x567.png" )->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_CompositeCalculations, 900x729, "eagle_900x729.png", "eagle_900x729.png", "beach_2500x1667.png", "result_900x729.png" )->Unit(benchmark::kMillisecond);
BENCHMARK_CAPTURE(BM_CompositeCalculations, 1200x973, "eagle_1200x972.png", "eagle_1200x973.png", "beach_2500x1667.png", "result_1200x973.png" )->Unit(benchmark::kMillisecond);

static void BM_Clone(benchmark::State& state) {

    auto backgroundImagePath = "./src/processing/tests/target_images/beach_large.png";
    auto backgroundImage = loadBackgroundImage(backgroundImagePath);

    for (auto _ : state) {
        auto clone = backgroundImage.clone();
    }

}

BENCHMARK(BM_Clone)->Unit(benchmark::kMillisecond);


BENCHMARK_MAIN();