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
#ifndef __itkParametricPath_h
#define __itkParametricPath_h

#include "itkPath.h"
#include "itkContinuousIndex.h"
#include "itkOffset.h"
#include "itkVector.h"

namespace itk
{
/** \class ParametricPath
 * \brief  Represent a parametric path through ND Space
 *
 * This virtual class is intended to represent a parametric path through an
 * image.   A parametric path maps a single floating-point 1D parameter (usually
 * designated as either time or arc-length) to a floating-point ND point in
 * continuous image index space. This mapping is done via the abstract
 * Evaluate() method, which must be overridden in all instantiable subclasses.
 * Parametric paths are required to be continuous.  They may be open or form a
 * closed loop. A parametric path may cross itself several times, although the
 * end point is constrained to have a unique spatial location unless it is
 * shared with and only with the starting point (a path tracing the number "9,"
 * starting at the bottom and ending in the middle of the right side, would
 * therefore be illegal).  Classic applications of this class include the
 * representation of contours in 2D images and path smoothing.  Another use of a
 * path is to guide the movement of an iterator through an image.
 *
 * \sa EllipseParametricPath
 * \sa PolyLineParametricPath
 * \sa FourierSeriesPath
 * \sa OrthogonallyCorrectedParametricPath
 * \sa ChainCodePath
 * \sa Path
 * \sa ContinuousIndex
 * \sa Index
 * \sa Offset
 * \sa Vector
 *
 * \ingroup PathObjects
 */
template< unsigned int VDimension >
class ITK_EXPORT ParametricPath:public
  Path< double, ContinuousIndex< double, VDimension >, VDimension >
{
public:
  /** Standard class typedefs. */
  typedef ParametricPath                                                    Self;
  typedef Path< double, ContinuousIndex< double, VDimension >, VDimension > Superclass;
  typedef SmartPointer< Self >                                              Pointer;
  typedef SmartPointer< const Self >                                        ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ParametricPath, Path);

  /** Input type */
  typedef typename Superclass::InputType InputType;

  /** Output type */
  typedef typename Superclass::OutputType OutputType;

  /** All paths must be mapable to index space */
  typedef ContinuousIndex< double, VDimension > ContinuousIndexType;
  typedef Index<  VDimension >                  IndexType;
  typedef Offset< VDimension >                  OffsetType;
  typedef Vector< double, VDimension >          VectorType;

  /** Return the nearest index to the parametric path at the specified location.
   * This is a wrapper to Evaluate(). */
  virtual IndexType EvaluateToIndex(const InputType & input) const;

  /** Increment the input variable passed by reference such that the ND index of
   * the path  moves to its next vertex-connected (8-connected in 2D) neighbor.
   * Return the Index-space offset of the path from its prior input to its new
   * input.  If the path is unable to increment, input is not changed and an
   * offset of Zero is returned. Children are not required to implement bounds
   * checking.
   *
   * This is a fairly slow, iterative algorithm that numerically converges to
   * the next index along the path, in a vertex-connected (8-connected in 2D)
   * fashion.  When possible, children of this class should overload this
   * function with something more efficient.
   *
   * WARNING:  This default implementation REQUIRES that the ND endpoint of
   * the path be either unique or coincident only with the startpoint, since it
   * uses the endpoint as a stopping condition. */
  virtual OffsetType IncrementInput(InputType & input) const;

  /** Evaluate the first derivative of the ND output with respect to the 1D
    * input.  This is a very simple and naive numerical derivative, and it
    * sould be overloaded with a proper closed-form derivative function in
    * all children.  Nevertheless, users who need to create their own parametric
    * classes for their private research need not reimplement this function if
    * their work does not need the derivative operator. */
  virtual VectorType EvaluateDerivative(const InputType & input) const;

  itkSetMacro(DefaultInputStepSize, InputType)
  itkGetConstReferenceMacro(DefaultInputStepSize, InputType)
protected:
  ParametricPath();
  ~ParametricPath(){}
  void PrintSelf(std::ostream & os, Indent indent) const;

  /** Default 1D input increment amount to trace along the path.  Also, the
   * value used by the defualt implementation of EvaluateDerivative() for
   * numerically approximating the derivative with the change over a single
   * default-sized step.  (NOTE that the default implementation of
   * EvaluateDerivative() should never be used in practice, but users or lazzy
   * developers may nevertheless unwisely choose to do so anyway.)  For integer-
   * input-types, 1 is probably the correct value.  For double-input-types,
   * either 1 or 0.1 are probably good values.  This value should be set in the
   * constructor of all instantiable children.  Values set in child constructors
   * overwrite values set in parent constructors. */
  InputType m_DefaultInputStepSize;
private:
  ParametricPath(const Self &); //purposely not implemented
  void operator=(const Self &); //purposely not implemented
};

// There are non-templated subclasses of ParametricPath
// (ex. OrthogonallyCorrected2DParametricPath which is a subclass of
// ParametricPath<2>).  We need force all consumers of
// ParametericPath<2> to get it from the ITKCommon library.
// itkParametricPath.cxx will put ParametricPath<2> in the the
// ITKCommon library. Everyone else using a ParametericPath<2> will
// load it from the dll.
#if ( defined( _WIN32 ) || defined( WIN32 ) ) && !defined( ITKSTATIC )
#ifndef ITKCommon_EXPORTS
template class __declspec(dllimport) ParametricPath< 2 >;
#endif
#endif
} // namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_ParametricPath(_, EXPORT, TypeX, TypeY)     \
  namespace itk                                                  \
  {                                                              \
  _( 1 ( class EXPORT ParametricPath< ITK_TEMPLATE_1 TypeX > ) ) \
  namespace Templates                                            \
  {                                                              \
  typedef ParametricPath< ITK_TEMPLATE_1 TypeX >                 \
  ParametricPath##TypeY;                                       \
  }                                                              \
  }

#if ITK_TEMPLATE_EXPLICIT
#include "Templates/itkParametricPath+-.h"
#endif

#if ITK_TEMPLATE_TXX
#include "itkParametricPath.txx"
#endif

#endif
