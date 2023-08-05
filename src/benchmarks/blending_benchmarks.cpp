#include <benchmark/benchmark.h>
#include "processing.h"
#include "common_tests.h"
#include "MeanStacker.h"
#include "BrightenStacker.h"
#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>
#include <map>
#include <tuple>
#include <filesystem>

using namespace processing;
using namespace std;
using namespace common::tests;

template <class ...Args>
static void BM_CompositeCalculations(benchmark::State& state, Args&&... args) {

    auto args_tuple = make_tuple(move(args)...);
    string backgroundImagePath = "./src/benchmarks/benchmark_data/background/" + string(get<2>(args_tuple));
    auto backgroundImage = loadStdImage(backgroundImagePath);

    auto mask = "./src/benchmarks/benchmark_data/masks/" + string(get<0>(args_tuple));
    auto original = "./src/benchmarks/benchmark_data/source/" + string(get<1>(args_tuple));

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

template <class ...Args>
static void BM_BrightenBlend(benchmark::State& state, Args&&... args) {

    auto args_tuple = make_tuple(move(args)...);
    std::string folder = get<0>(args_tuple);
    BrightenStacker b;
    vector<Mat> imagesToStack = vector<Mat>();
    for (const auto & entry : filesystem::directory_iterator(folder))
    {
        Mat img = loadStdImage(entry.path().c_str());
        imagesToStack.push_back(img);
    } 

    Mat total_image;

    for (auto _ : state)
    {
        for(int i = 0; i < imagesToStack.size(); i++)
        {
            b.AddToStack(imagesToStack[i]);
        }
        
        total_image = b.GetCurrentBlend();
    }

    imwrite("./src/benchmarks/benchmark-results/star_trails.png", total_image);
}

BENCHMARK_CAPTURE(BM_BrightenBlend, StarTrails, "./src/blending/tests/long_exposure_data/90Photos_2Seconds_Trails")->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();