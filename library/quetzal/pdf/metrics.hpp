#ifndef _METRICS_H_
#define _METRICS_H_

#include "pdf.hpp"

   class Metrics
   {
      public:

         static int* get(quetzal::pdf::PDF::Font font);

      private:

         Metrics();
   };

#endif
