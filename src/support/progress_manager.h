#pragma once

#include <atomic>
#include <mutex>


class ProgressManager
{
   std::atomic<int> progress_ = 0;
   std::mutex mutex_;

  public:
   static void AddProgress(int additional_progress);
   static void SetProgress(int total_progress);
};