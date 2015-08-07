#pragma once

#include <QElapsedTimer>

class QElapsedTimer;

class TickAlignedLoop {
public:
  TickAlignedLoop();

  uint64_t wait(uint64_t hz = 100);
  uint64_t loopCount() const;

private:
  QElapsedTimer m_elapsed;
  uint64_t m_runtime;
  uint64_t m_alltime;
  uint64_t m_loop;
};

