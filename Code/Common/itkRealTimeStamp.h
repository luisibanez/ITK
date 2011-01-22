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
#ifndef __itkRealTimeStamp_h
#define __itkRealTimeStamp_h

#include "itkIntTypes.h"

namespace itk
{
/** \class RealTimeStamp
 * \brief The RealTimeStamp is a data structure for representing time with high
 * precision and a large dynamic range.
 *
 * This class represents time typically for applications that need to mark the
 * time of acquisition of data with high precision (microseconds) and a large
 * dynamic range (years).
 *
 * \sa RealTimeClock
 *
 */

class RealTimeStamp
{
public:

  double GetTimeInMicroSeconds() const;
  double GetTimeInSeconds() const;
  double GetTimeInHours() const;
  double GetTimeInDays() const;

  class RealTimeInterval;

  /** Arithmetic operations between RealTimeInterval and RealTimeStamp. */
  RealTimeInterval operator-( const RealTimeStamp & ) const;
  RealTimeStamp operator+( const RealTimeInterval & ) const;
  RealTimeStamp operator-( const RealTimeInterval & ) const;

  const RealTypeStamp & operator-=( const RealTimeInterval & );

private:

  /** Number of Seconds since... */
  uint64_t   m_Seconds;
  uint64_t   m_MicroSeconds;

};
} // end of namespace itk

#endif  // __itkRealTimeStamp_h
