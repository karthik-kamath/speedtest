#ifndef RESULT_PRINTER_H
#define RESULT_PRINTER_H

#include "UnifiedSpeedTestResult.h"

class ResultPrinter {
   public:
    static void print(const UnifiedSpeedTestResult& result);
};

#endif