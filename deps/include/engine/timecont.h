/// @author Javier Guinot Almenar
/// @file TimeCont.h

#include "engine.h"

/////////////////////////
#ifndef __TIME_CONT_H__
#define __TIME_CONT_H__ 1
/////////////////////////

/**
 * @class TimeCont
 *
 * @brief Represents a high-resolution timer for measuring elapsed time with various precisions.
 */
class TimeCont
{
public:
  /**
   * @enum Precision
   *
   * @brief Enumeration for time precision units.
   */
  enum class Precision
  {
    minutes = 0,  ///< Minutes precision.
    seconds,      ///< Seconds precision.
    milliseconds, ///< Milliseconds precision.
    microseconds, ///< Microseconds precision.
    nanoseconds,  ///< Nanoseconds precision.
    picoseconds,  ///< Picoseconds precision.
  };

  /**
   * @brief Starts the timer.
   */
  void startTime();

  /**
   * @brief Continues the timer if it was previously stopped.
   */
  void continueTime();

  /**
   * @brief Stops the timer.
   */
  void stopTime();

  /**
   * @brief Gets the elapsed time between the current and last time point with the specified precision.
   *
   * The next functions are only avaliable for the enums precisions. Each time
   * are called take the new value for last_time_ and calculate the time
   * between current_time_ & last_time_
   *
   * @param precision The desired precision for the elapsed time.
   *
   * @return Elapsed time in the specified precision.
   */
  size_t getElapsedTime(Precision precision);

  /**
   * @brief Prints the elapsed time between the current and last time point with the specified precision.
   *
   * @param precision The desired precision for the elapsed time.
   * @param str Optional string to prepend to the printed output.
   */
  void printElapsedTime(Precision precision, byte *str = nullptr);

  /**
   * @brief Delays the execution of the program for the specified number of milliseconds.
   *
   * @param milliseconds The duration of the delay in milliseconds.
   */
  static inline void Delay(s32 milliseconds) { std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds)); }

  /**
   * @brief Example usage of the TimeCont class to measure elapsed time with different precisions.
   */
  static inline void Example()
  {
    TimeCont timer;
    timer.startTime();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    timer.printElapsedTime(Precision::minutes);
    timer.printElapsedTime(Precision::seconds);
    timer.printElapsedTime(Precision::milliseconds);
    timer.printElapsedTime(Precision::microseconds);
    timer.printElapsedTime(Precision::nanoseconds);
    timer.printElapsedTime(Precision::picoseconds);

    fprintf(stdout, "\n");

    fprintf(stdout, "Time: %zd\n", timer.getElapsedTime(Precision::minutes));
    fprintf(stdout, "Time: %zd\n", timer.getElapsedTime(Precision::seconds));
    fprintf(stdout, "Time: %zd\n", timer.getElapsedTime(Precision::milliseconds));
    fprintf(stdout, "Time: %zd\n", timer.getElapsedTime(Precision::microseconds));
    fprintf(stdout, "Time: %zd\n", timer.getElapsedTime(Precision::nanoseconds));
    fprintf(stdout, "Time: %zd\n", timer.getElapsedTime(Precision::picoseconds));
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> current_time_; ///< Current time point.
  std::chrono::time_point<std::chrono::high_resolution_clock> last_time_;    ///< Last time point.
  std::chrono::time_point<std::chrono::high_resolution_clock> stopped_time_; ///< Time point when the timer was stopped.

  boolean stopped_ = false; ///< Flag indicating whether the timer is stopped.
};

////////////////////////////
#endif /* __TIME_CONT_H__ */
       ////////////////////////////