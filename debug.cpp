#include "debug.h"

#include "boost/date_time/posix_time/posix_time.hpp"

std::ostream& now(std::ostream& out) {
  static bool first_call = true;
  static boost::posix_time::ptime start;
  if (first_call) {
    start = boost::posix_time::microsec_clock::local_time();
    first_call = false;
  }
  boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
  boost::posix_time::time_duration elapsed = now - start;
  return out << elapsed.total_milliseconds() << " ";
}

// vim:ruler:cindent:shiftwidth=2:expandtab:
