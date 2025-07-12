#include "progress_manager.h"

#include <external/commonItems/Log.h>
#include <external/fmt/include/fmt/format.h>



static ProgressManager instance;

void ProgressManager::AddProgress(int progress)
{
   instance.progress_ += progress;
   // if we don't have the lock, it means that another thread is outputting updated progress.
   // we don't need every single percent, so just skip updating in this case.
   if (instance.mutex_.try_lock())
   {
      Log(LogLevel::Progress) << fmt::format("{} %", instance.progress_.load());
      instance.mutex_.unlock();
   }
}

void ProgressManager::SetProgress(int progress)
{
   instance.progress_ = progress;
   // if we don't have the lock, it means that another thread is outputting updated progress.
   // we don't need every single percent, so just skip updating in this case.
   if (instance.mutex_.try_lock())
   {
      Log(LogLevel::Progress) << fmt::format("{} %", instance.progress_.load());
      instance.mutex_.unlock();
   }
}