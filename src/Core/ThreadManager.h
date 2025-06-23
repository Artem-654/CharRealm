#pragma once
#include <thread>
#include <atomic>
#include <functional>
#include "Interfaces/IMainState.h"
#include "Console.h"

class ThreadManager {
private:
    std::thread inputThread;
    std::thread logicThread;
    std::thread renderThread;
    std::atomic<bool> running;

public:
    ThreadManager();
    ~ThreadManager();

    void run(IMainState& state, Console& console);

    void stop();
    void join();
};
