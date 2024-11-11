#include "progress_manager.h"

#include "external/commonItems/Log.h"
#include "external/fmt/include/fmt/format.h"



static ProgressManager instance_;

void ProgressManager::AddProgress(int progress)
{
   instance_.progress_ += progress;
   // if we don't have the lock, it means that another thread is outputting updated progress.
   // we don't need every single percent, so just skip updating in this case.
   if (instance_.mutex_.try_lock())
   {
      Log(LogLevel::Progress) << fmt::format("{} %", instance_.progress_.load());
      instance_.mutex_.unlock();
   }
}

void ProgressManager::SetProgress(int progress)
{
   instance_.progress_ = progress;
   // if we don't have the lock, it means that another thread is outputting updated progress.
   // we don't need every single percent, so just skip updating in this case.
   if (instance_.mutex_.try_lock())
   {
      Log(LogLevel::Progress) << fmt::format("{} %", instance_.progress_.load());
      instance_.mutex_.unlock();
   }
}