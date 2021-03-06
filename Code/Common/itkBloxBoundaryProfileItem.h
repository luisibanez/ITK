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
#ifndef __itkBloxBoundaryProfileItem_h
#define __itkBloxBoundaryProfileItem_h

#include "itkBloxBoundaryPointItem.h"

namespace itk
{
template< unsigned int TImageDimension >
class ITK_EXPORT BloxBoundaryProfileItem:public BloxItem
{
public:
  /** Run-time type information (and related methods). */
  itkTypeMacro(BloxBoundaryProfileItem, BloxItem);

  /** The point type used to store the position of the boundary profile */
  typedef Point< double, TImageDimension > PositionType;

  /** The type of vector used to store the gradient of the BoundaryPointItem
   */
  typedef CovariantVector< double, TImageDimension > GradientType;

  /** Vector type */
  typedef vnl_vector< double > VectorType;

  /** The type of boundary point item we store pointers to */
  typedef BloxBoundaryPointItem< TImageDimension > BPItemType;

  /** Set the position of the first boundary point in physical space */
  void SetBoundaryPoint(BPItemType *point);

  /** Set and get lower intensity estimates */
  void SetLowerIntensity(double lowerIntensity);

  double GetLowerIntensity(void);

  /** Set and get upper intensity estimates */
  void SetUpperIntensity(double upperIntensity);

  double GetUpperIntensity(void);

  /** Set and get mean estimates */
  void SetMean(double mean);

  double GetMean(void);

  /** Set and get the length of profile */
  void SetProfileLength(unsigned int profileLength);

  unsigned int GetProfileLength(void);

  /** Set and get mean normalized by profile length */
  void SetMeanNormalized(void);

  double GetMeanNormalized(void);

  /** Set and get standard deviation */
  void SetStandardDeviation(double standardDeviation);

  double GetStandardDeviation(void);

  /** Set and get standard deviation normalized by profile length */
  void SetStandardDeviationNormalized(void);

  double GetStandardDeviationNormalized(void);

  /** Set and get optimal boundary location */
  void SetOptimalBoundaryLocation(VectorType spatialFunctionOriginVector,
                                  VectorType orientation);

  PositionType GetOptimalBoundaryLocation(void);

  /** Set and get the gradient of the boundary profile * */
  void SetGradient(GradientType *gradient);

  GradientType * GetGradient();

  void SetGradient2(GradientType gradient) { m_Gradient2 = gradient; }
  GradientType GetGradient2() { return m_Gradient2; }

  BloxBoundaryProfileItem();
  ~BloxBoundaryProfileItem();
private:
  /** Lower estimated intensity */
  double m_LowerIntensity;

  /** Upper estimated intensity */
  double m_UpperIntensity;

  /** Length of sampling profile */
  unsigned int m_ProfileLength;

  /** Mean location of the boundary along sampling profile */
  double m_Mean;

  /** Mean normalized by the profile length */
  double m_MeanNormalized;

  /** Width of blurred boundary */
  double m_StandardDeviation;

  /** Width of blurred boundary normalized by sampling profile */
  double m_StandardDeviationNormalized;

  /** The boundary point to construct a profile from */
  BPItemType *m_BoundaryPoint;

  /** The position of the estimated boundary location */
  PositionType m_OptimalBoundaryLocation;

  /** The gradient of the boundary point (non-normalized) */
  GradientType *m_Gradient;

  GradientType m_Gradient2;
};
} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_BloxBoundaryProfileItem(_, EXPORT, TypeX, TypeY)     \
  namespace itk                                                           \
  {                                                                       \
  _( 1 ( class EXPORT BloxBoundaryProfileItem< ITK_TEMPLATE_1 TypeX > ) ) \
  namespace Templates                                                     \
  {                                                                       \
  typedef BloxBoundaryProfileItem< ITK_TEMPLATE_1 TypeX >                 \
  BloxBoundaryProfileItem##TypeY;                                       \
  }                                                                       \
  }

#if ITK_TEMPLATE_EXPLICIT
#include "Templates/itkBloxBoundaryProfileItem+-.h"
#endif

#if ITK_TEMPLATE_TXX
#include "itkBloxBoundaryProfileItem.txx"
#endif

#endif
