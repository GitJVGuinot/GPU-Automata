#include "engine.h"

#ifndef __TASKMANAGER_H__
#define __TASKMANAGER_H__ 1

/**
 * @class TaskManager
 *
 * @brief Manages a pool of threads for executing asynchronous tasks.
 *
 * The TaskManager class provides functionality to manage a pool of threads for executing asynchronous tasks.
 * It supports enqueueing tasks with different arguments and returns their results using std::future.
 */
class TaskManager
{
public:
  /**
   * @brief Gets the singleton instance of the TaskManager.
   *
   * @return A pointer to the singleton instance of the TaskManager.
   */
  static TaskManager *Instance();

  /**
   * @brief Free all resources
   * 
   * We use it to wait for all threads to stop before the object is destroyed.
   */
  void free();

  /**
   * @brief Enqueues a task with specified arguments for execution.
   *
   * It's used to enqueue a task to a queue, making sure only one tasks can be
   * added at a time. It takes in a function and its arguments and passess and
   * deduces the return type returning a std::future of what the return type
   * is assumed to be.
   *
   * @tparam Function Type of the task function.
   * @tparam Args Types of the task function arguments.
   *
   * @param func Task function.
   * @param args Task function arguments.
   *
   * @return A std::future representing the result of the enqueued task.
   */
  template <typename Function, typename... Args>
  decltype(auto) enqueue(Function &&func, Args &&...args)
  {
    using ReturnType = std::invoke_result_t<Function, Args...>;
    auto task = std::make_shared<std::packaged_task<ReturnType()>>(std::bind(std::forward<Function>(func), std::forward<Args>(args)...));
    auto result = task->get_future();
    mutex_.lock();
    tasks_.emplace([task]() mutable
                   { (*task)(); });
    mutex_.unlock();
    condition_.notify_one();

    return result;
  }

private:
  std::vector<std::thread> threads_;             ///< Vector of threads in the thread pool.
  std::queue<std::packaged_task<void()>> tasks_; ///< Queue of packaged tasks.
  std::mutex mutex_;                             ///< Mutex for synchronization.
  std::condition_variable condition_;            ///< Condition variable for signaling tasks.
  boolean stop_;                                 ///< Flag to stop the task manager.

  /**
   * @brief Constructor for the TaskManager class.
   *
   * It asks for the number of threads in the system and we make use of half
   * taking into account other programs.
   *
   * @param numThreads Number of threads in the system.
   */
  TaskManager(s32 numThreads = (std::thread::hardware_concurrency() / 2));

  /**
   * @brief Destructor for the TaskManager class.
   */
  ~TaskManager();

  /**
   * @brief Worker function executed by each thread in the pool.
   *
   * It takes tasks from the queue, one at the time and executes them and
   * removes them from the queue.
   */
  void worker();
};

/**
 * @brief Macro for accessing the singleton instance of the TaskManager.
 */
#define TM (TaskManager::Instance())

#endif /* __TASKMANAGER_H__ */