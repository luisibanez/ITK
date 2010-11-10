/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDeformationFieldTransform.h,v $
  Language:  C++
  Date:      $Date: $
  Version:   $Revision: $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDeformationFieldTransform_h
#define __itkDeformationFieldTransform_h

#include "itkTransform.h"

#include "itkImage.h"
#include "itkVectorInterpolateImageFunction.h"

namespace itk
{

/** \class DeformationFieldTransform
 *
 * \ingroup Transforms
 *
 */
template
  <class TScalar, unsigned int NDimensions>
class ITK_EXPORT DeformationFieldTransform :
  public Transform<TScalar, NDimensions, NDimensions>
{
public:
  /** Standard class typedefs. */
  typedef DeformationFieldTransform                         Self;
  typedef Transform<TScalar, NDimensions, NDimensions>      Superclass;
  typedef SmartPointer<Self>                                Pointer;
  typedef SmartPointer<const Self>                          ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro( DeformationFieldTransform, Transform );

  /** New macro for creation of through a Smart Pointer */
  itkNewMacro( Self );

  /** InverseTransform type. */
  typedef typename Superclass::InverseTransformBasePointer  InverseTransformBasePointer;

  /** Scalar type. */
  typedef typename Superclass::ScalarType  ScalarType;

  /** Jacobian type. */
  typedef typename Superclass::JacobianType  JacobianType;

  /** Standard coordinate point type for this class. */
  typedef typename Superclass::InputPointType   InputPointType;
  typedef typename Superclass::OutputPointType  OutputPointType;

  /** Standard vector type for this class. */
  typedef typename Superclass::InputVectorType   InputVectorType;
  typedef typename Superclass::OutputVectorType  OutputVectorType;

  /** Dimension of the domain spaces. */
  itkStaticConstMacro( Dimension, unsigned int, NDimensions );

  /** Define the deformation field type and corresponding interpolator type. */
  typedef Image<OutputVectorType,
    itkGetStaticConstMacro( Dimension )> DeformationFieldType;
  typedef VectorInterpolateImageFunction
    <DeformationFieldType, ScalarType> InterpolatorType;

  /** Get/Set the deformation field. */
  itkGetObjectMacro( DeformationField, DeformationFieldType );
  itkSetObjectMacro( DeformationField, DeformationFieldType );

  /** Get/Set the inverse deformation field. */
  itkGetObjectMacro( InverseDeformationField, DeformationFieldType );
  itkSetObjectMacro( InverseDeformationField, DeformationFieldType );

  /** Get/Set the interpolator. */
  itkGetObjectMacro( Interpolator, InterpolatorType );
  itkSetObjectMacro( Interpolator, InterpolatorType );

  bool GetInverse( Self *inverse ) const;

  /** Return an inverse of this transform. */
  virtual InverseTransformBasePointer GetInverseTransform() const;

  /** Compute the position of point in the new space */
  virtual OutputPointType TransformPoint( const InputPointType& thisPoint ) const;

  /** This transform is not linear. */
  virtual bool IsLinear() const { return false; }

  /**
   * Compute the jacobian with respect to the parameters.  Since there are
   * no parameters for this transform, the Jacobian shouldn't be requested.
   */
  virtual JacobianType & GetJacobian( const InputPointType & ) const;

protected:
  DeformationFieldTransform();
  virtual ~DeformationFieldTransform();
  void PrintSelf( std::ostream& os, Indent indent ) const;

  /** The deformation field and its inverse (if it exists). */
  typename DeformationFieldType::Pointer            m_DeformationField;
  typename DeformationFieldType::Pointer            m_InverseDeformationField;

  /** The interpolator. */
  typename InterpolatorType::Pointer                m_Interpolator;

private:
  DeformationFieldTransform( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

};

} // end namespace itk

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkDeformationFieldTransform+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkDeformationFieldTransform.txx"
#endif

#endif // __itkDeformationFieldTransform_h
