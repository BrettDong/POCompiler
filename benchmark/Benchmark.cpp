#include <benchmark/benchmark.h>

#include "FileReader/MemMapFileReader.hpp"
#include "LineView.hpp"
#include "Parser.hpp"
#include "Writer.hpp"

#include <memory>

class ParserBenchmarkFixture : public benchmark::Fixture {
  protected:
    std::unique_ptr<FileReader> mFileReader;

  public:
    void SetUp(const benchmark::State &) override {
        mFileReader = std::make_unique<MemMapFileReader>();
        mFileReader->open(REF_PO_FILE);
        std::size_t lc = 0;
        for (auto &&line : LineView(mFileReader->view())) {
            lc += line.length();
        }
        benchmark::DoNotOptimize(lc);
    }

    void TearDown(const benchmark::State &) override {
        mFileReader->close();
    }
};

BENCHMARK_F(ParserBenchmarkFixture, ParserBenchmark)(benchmark::State &state) {
    for (auto _ : state) {
        Parser parser;
        auto result = parser.parsePOFile(*mFileReader);
        benchmark::DoNotOptimize(result);
    }
}

class WriterBenchmarkFixture : public benchmark::Fixture {
  protected:
    std::map<std::string, std::string> mEntries;

  public:
    void SetUp(const benchmark::State &) override {
        auto fileReader = std::make_unique<MemMapFileReader>();
        fileReader->open(REF_PO_FILE);
        auto parser = Parser();
        mEntries = parser.parsePOFile(*fileReader);
    }

    void TearDown(const benchmark::State &) override {}
};

BENCHMARK_F(WriterBenchmarkFixture, WriterBenchmark)(benchmark::State &state) {
    for (auto _ : state) {
        writeMO(mEntries, "/dev/null");
    }
}

BENCHMARK_MAIN();
