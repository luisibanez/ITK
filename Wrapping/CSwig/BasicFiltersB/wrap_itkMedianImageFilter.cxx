/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include "itkMedianImageFilter.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"
#include "itkCSwigImages.h"

//=================================
//THIS FILE GENERATED WITH MakeConsistentWrappedClasses.sh
//=================================
namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkMedianImageFilter);
  namespace wrappers
  {
    //===========2D Wrapped Filters==============
    ITK_WRAP_OBJECT2(MedianImageFilter, image::F2 , image::F2 , itkMedianImageFilterF2F2  );
    ITK_WRAP_OBJECT2(MedianImageFilter, image::D2 , image::D2 , itkMedianImageFilterD2D2  );
    ITK_WRAP_OBJECT2(MedianImageFilter, image::UC2, image::UC2, itkMedianImageFilterUC2UC2);
    ITK_WRAP_OBJECT2(MedianImageFilter, image::US2, image::US2, itkMedianImageFilterUS2US2);
    ITK_WRAP_OBJECT2(MedianImageFilter, image::UI2, image::UI2, itkMedianImageFilterUI2UI2);
    ITK_WRAP_OBJECT2(MedianImageFilter, image::SC2, image::SC2, itkMedianImageFilterSC2SC2);
    ITK_WRAP_OBJECT2(MedianImageFilter, image::SS2, image::SS2, itkMedianImageFilterSS2SS2);
    ITK_WRAP_OBJECT2(MedianImageFilter, image::SI2, image::SI2, itkMedianImageFilterSI2SI2);

    //===========3D Wrapped Filters==============
    ITK_WRAP_OBJECT2(MedianImageFilter, image::F3 , image::F3 , itkMedianImageFilterF3F3  );
    ITK_WRAP_OBJECT2(MedianImageFilter, image::D3 , image::D3 , itkMedianImageFilterD3D3  );
    ITK_WRAP_OBJECT2(MedianImageFilter, image::UC3, image::UC3, itkMedianImageFilterUC3UC3);
    ITK_WRAP_OBJECT2(MedianImageFilter, image::US3, image::US3, itkMedianImageFilterUS3US3);
    ITK_WRAP_OBJECT2(MedianImageFilter, image::UI3, image::UI3, itkMedianImageFilterUI3UI3);
    ITK_WRAP_OBJECT2(MedianImageFilter, image::SC3, image::SC3, itkMedianImageFilterSC3SC3);
    ITK_WRAP_OBJECT2(MedianImageFilter, image::SS3, image::SS3, itkMedianImageFilterSS3SS3);
    ITK_WRAP_OBJECT2(MedianImageFilter, image::SI3, image::SI3, itkMedianImageFilterSI3SI3);
  }
}
#endif
