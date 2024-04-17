#include "engine.h"

#ifndef __ERROR_LOG_H__
#define __ERROR_LOG_H__ 1

/**
 * @class ErrorLog
 *
 * @brief A class for managing error logs.
 */
class ErrorLog
{
public:
  /**
   * @struct Error
   *
   * @brief Structure representing an error entry.
   */
  struct Error
  {
    std::string function_ = "\0"; ///< Name of the function where the error occurred.
    std::string error_ = "\0";    ///< Description of the error.
    std::string line_ = "\0";     ///< Line number where the error occurred.
  };

  /**
   * @brief Constructor for the ErrorLog class.
   */
  ErrorLog();

  /**
   * @brief Destructor for the ErrorLog class.
   */
  ~ErrorLog();

  /**
   * @brief Add a new error entry to the end of the queue.
   *
   * @param error Description of the error.
   * @param function Name of the function where the error occurred.
   * @param line Line number where the error occurred.
   */
  void addError(std::string error = "\0", std::string function = "\0", std::string line = "\0");

  /**
   * @brief Get the last error from the queue and remove it.
   *
   * @return The last error entry.
   */
  Error getError();

private:
  Error *queue_; ///< Array storing error entries.
  size_t size_;  ///< Number of errors in the queue.
};

#endif /* __ERROR_LOG_H__ */
