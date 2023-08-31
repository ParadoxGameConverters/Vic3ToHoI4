#pragma once

#include <mutex>
#include <atomic>

class ProgressManager
{
   std::atomic<int> progress_ = 0;
   std::mutex mutex_;

  public:
   static void AddProgress(int additionalProgress);
   static void SetProgress(int totalProgress);
};