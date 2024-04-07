#include <atomic>
#include <cstddef>
#include <gtest/gtest.h>


#include "cpr/threadpool.h"

TEST(ThreadPoolTests, BasicWorkOneThread) {
    std::atomic_uint32_t invCount{0};
    uint32_t invCountExpected{100};

    {
        cpr::ThreadPool tp;
        tp.SetMinThreadNum(1);
        tp.SetMaxThreadNum(1);
        tp.Start(0);

        for (size_t i = 0; i < invCountExpected; ++i) {
            tp.Submit([&invCount]() -> void { invCount++; });
        }

        // Wait for the thread pool to finish its work
        tp.Wait();
    }

    EXPECT_EQ(invCount, invCountExpected);
}

TEST(ThreadPoolTests, BasicWorkMultipleThreads) {
    std::atomic_uint32_t invCount{0};
    uint32_t invCountExpected{100};

    {
        cpr::ThreadPool tp;
        tp.SetMinThreadNum(1);
        tp.SetMaxThreadNum(10);
        tp.Start(0);

        for (size_t i = 0; i < invCountExpected; ++i) {
            tp.Submit([&invCount]() -> void { invCount++; });
        }

        // Wait for the thread pool to finish its work
        tp.Wait();
    }

    EXPECT_EQ(invCount, invCountExpected);
}

TEST(ThreadPoolTests, PauseResumeSingleThread) {
    std::atomic_uint32_t invCount{0};

    uint32_t repCount{100};
    uint32_t invBunchSize{20};

    cpr::ThreadPool tp;
    tp.SetMinThreadNum(1);
    tp.SetMaxThreadNum(10);
    tp.Start(0);

    for (size_t i = 0; i < repCount; ++i) {
        EXPECT_EQ(invCount, i * invBunchSize);

        for (size_t e = 0; e < invBunchSize; ++e) {
            tp.Submit([&invCount]() -> void { invCount++; });
        }
        // Wait for the thread pool to finish its work
        tp.Wait();

        EXPECT_EQ(invCount, (i + 1) * invBunchSize);
    }
}

TEST(ThreadPoolTests, PauseResumeMultipleThreads) {
    std::atomic_uint32_t invCount{0};

    uint32_t repCount{100};
    uint32_t invBunchSize{20};

    cpr::ThreadPool tp;
    tp.SetMinThreadNum(1);
    tp.SetMaxThreadNum(10);
    tp.Start(0);

    for (size_t i = 0; i < repCount; ++i) {
        EXPECT_EQ(invCount, i * invBunchSize);

        for (size_t e = 0; e < invBunchSize; ++e) {
            tp.Submit([&invCount]() -> void { invCount++; });
        }
        // Wait for the thread pool to finish its work
        tp.Wait();

        EXPECT_EQ(invCount, (i + 1) * invBunchSize);
    }
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
