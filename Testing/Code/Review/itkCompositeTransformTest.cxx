/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkCompositeTransformTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>

#include "itkAffineTransform.h"
#include "itkCompositeTransform.h"
#include "itkDeformationFieldTransform.h"

const double epsilon = 1e-10;

//namespace
//{

template <typename TPoint>
bool samePoint( const TPoint & p1, const TPoint & p2 )
  {
  bool pass=true;
  for ( unsigned int i = 0; i < TPoint::PointDimension; i++ )
    {
    if( vcl_fabs( p1[i] - p2[i] ) > epsilon )
      pass=false;
    }
  return pass;
  }

//}

int itkCompositeTransformTest(int ,char *[] )
{
  #define NDIMENSIONS 2
  #define TSCALAR double

  typedef  itk::Matrix<double,2,2>   Matrix2Type;
  typedef  itk::Vector<double,2>     Vector2Type;


  /* Create composite transform */
  typedef itk::CompositeTransform<TSCALAR, NDIMENSIONS> CompositeType;
  CompositeType::Pointer compositeTransform = CompositeType::New();

  /* Add an affine transform */
  typedef itk::AffineTransform<TSCALAR, NDIMENSIONS> AffineType;
  AffineType::Pointer affine = AffineType::New();
  Matrix2Type matrix2;
  Vector2Type vector2;
  matrix2[0][0] = 1;
  matrix2[0][1] = 2;
  matrix2[1][0] = 3;
  matrix2[1][1] = 4;
  vector2[0] = 5;
  vector2[1] = 6;
  affine->SetMatrix(matrix2);
  affine->SetOffset(vector2);

  compositeTransform->PushFrontTransform( affine );

  /* Test that we have one tranform in the queue */
  if( compositeTransform->GetNumberOfComposingTransforms() != 1 )
    {
    std::cout << "Failed adding transform to queue." << std::endl;
    return EXIT_FAILURE;
    }

  /* Setup test point and truth value for tests */
  CompositeType::InputPointType  testPoint;
  CompositeType::OutputPointType outputPoint, affineTruth;
  testPoint[0] = 2;
  testPoint[1] = 3;
  affineTruth[0] = 13;
  affineTruth[1] = 24;

  /* Test transforming the point with just the affine transform */
  outputPoint = compositeTransform->TransformPoint( testPoint );

  if( !samePoint( outputPoint, affineTruth) )
      {
      std::cout << "Failed transforming point with single transform."
                << std::endl;
      return EXIT_FAILURE;
      }

  /* Create a deformation field transform and add it. */
  typedef itk::DeformationFieldTransform<TSCALAR, NDIMENSIONS>
      DeformationTransformType;
  DeformationTransformType::Pointer deformationTransform =
      DeformationTransformType::New();
  typedef DeformationTransformType::DeformationFieldType FieldType;
  FieldType::Pointer field = FieldType::New(); //Image type

  FieldType::SizeType size;
  FieldType::IndexType start;
  FieldType::RegionType region;
  size.Fill( 30 );
  start.Fill( 0 );
  region.SetSize( size );
  region.SetIndex( start );
  field->SetRegions( region );
  field->Allocate();

  DeformationTransformType::OutputVectorType zeroVector;
  zeroVector.Fill( 0 );
  field->FillBuffer( zeroVector );

  /*
   * Set a deformation field that has only one point with
   * a non-zero vector to help with testing. Choose the
   * point to match the output of the affine transform
   * when transforming the test point used above. */
  FieldType::IndexType nonZeroFieldIndex;
  nonZeroFieldIndex[0] = affineTruth[0];
  nonZeroFieldIndex[1] = affineTruth[1];
  TSCALAR data[] = {4,-5};
  DeformationTransformType::OutputVectorType nonZeroFieldVector(data);
  field->SetPixel( nonZeroFieldIndex, nonZeroFieldVector );

  deformationTransform->SetDeformationField( field );

  /* Test the deformable transform on its own. */
  DeformationTransformType::OutputPointType deformOutput, deformTruth;
  deformTruth[0] = affineTruth[0] + nonZeroFieldVector[0];
  deformTruth[1] = affineTruth[1] + nonZeroFieldVector[1];
  deformOutput = deformationTransform->TransformPoint( affineTruth );
  std::cout << "deformationTransform: " << std::endl
            << deformationTransform << std::endl
            << "testPoint transformed by just deformation transform: "
            << deformOutput << std::endl;
  if( !samePoint( deformOutput, deformTruth ) )
      {
      std::cout << "Failed transforming point with deformable transform."
                << std::endl;
      return EXIT_FAILURE;
      }

  /* Add the transform to the composite transform */
  compositeTransform->PushBackTransform( deformationTransform );

  std::cout << "Composite Transform:" << std::endl << compositeTransform;

  /* Test that we now have two tranforms in the queue */
  if( compositeTransform->GetNumberOfComposingTransforms() != 2 )
    {
    std::cout << "Failed adding deformation transform to queue." << std::endl;
    return EXIT_FAILURE;
    }

  /* Test transformation of a point by both transforms.
   * Use the same test point as above for affine-only test.
   * We've setup the test so that the affine transform should
   * transform the test point to the one point in the deformation
   * field that is non-zero.
   */
  CompositeType::OutputPointType compositeTruth;
  compositeTruth[0] = affineTruth[0] + nonZeroFieldVector[0];
  compositeTruth[1] = affineTruth[1] + nonZeroFieldVector[1];

  outputPoint = compositeTransform->TransformPoint( testPoint );
  std::cout << "Transform point with composite transform: " << std::endl
            << "  Test point: " << std::endl << testPoint << std::endl
            << "  Truth: " << std::endl << compositeTruth << std::endl
            << "  Output: " << std::endl << outputPoint << std::endl;

  if( !samePoint( outputPoint, compositeTruth ) )
      {
      std::cout << "Failed transforming point with composite transform."
                << std::endl;
      return EXIT_FAILURE;
      }

  /* Test IsLinear() */

  /* The deformation field transform should always return false for IsLinear*/
  if( deformationTransform->IsLinear() )
    {
    std::cout << "DeformationFieldTransform returned 'true' for IsLinear()."
      " Expected 'false'." << std::endl;
    return EXIT_FAILURE;
    }

  if( compositeTransform->IsLinear() )
    {
    std::cout << "compositeTransform returned 'true' for IsLinear()."
      " Expected 'false'." << std::endl;
    return EXIT_FAILURE;
    }

  /* Test inverse transform */

  /* We haven't set an inverse deformation field for the inverse deformation
   * transform, so we should get a false return here */
  CompositeType::Pointer inverseTransform = CompositeType::New();
  if( compositeTransform->GetInverse( inverseTransform ) )
    {
    std::cout << "Expected GetInverse() to fail." << std::endl;
    return EXIT_FAILURE;
    }
  std::cout << "inverseTransform:" << std::endl
            << inverseTransform << std::endl;

  /* Add an inverse deformation field */
  FieldType::Pointer inverseField = FieldType::New();
  DeformationTransformType::OutputVectorType inverseFieldVector;
  FieldType::IndexType inverseFieldIndex;
  /* Use same initializers that we used above */
  inverseField->SetRegions( region );
  inverseField->Allocate();
  inverseField->FillBuffer( zeroVector );
  inverseFieldVector = -nonZeroFieldVector;
  inverseFieldIndex[0] = compositeTruth[0];
  inverseFieldIndex[1] = compositeTruth[1];
  inverseField->SetPixel( inverseFieldIndex, inverseFieldVector );

  deformationTransform->SetInverseDeformationField( inverseField );

  if( !compositeTransform->GetInverse( inverseTransform ) )
    {
    std::cout << "Expected GetInverse() to succeed." << std::endl;
    return EXIT_FAILURE;
    }

  /* Transform the result of forward transformation back to
   * the original test point */
  CompositeType::OutputPointType inverseTruth, inverseOutput;
  inverseTruth = testPoint;
  inverseOutput = inverseTransform->TransformPoint( compositeTruth );
  std::cout << "Transform point with inverse composite transform: "
            << std::endl
            << "  Test point: " << compositeTruth << std::endl
            << "  Truth: " << inverseTruth << std::endl
            << "  Output: " << inverseOutput << std::endl;
  if( !samePoint( inverseOutput, inverseTruth ) )
    {
    std::cout << "Failed transform point with inverse composite transform."
              << std::endl;
    return EXIT_FAILURE;
    }
  /* Get inverse transform again, but using other accessor. */
  CompositeType::Pointer inverseTransform2;
  inverseTransform2 = dynamic_cast<CompositeType*>
    ( compositeTransform->GetInverseTransform().GetPointer() );
  if( ! inverseTransform2 )
    {
    std::cout << "Failed calling GetInverseTransform()." << std::endl;
    return EXIT_FAILURE;
    }
  inverseOutput = inverseTransform2->TransformPoint( compositeTruth );
  if( !samePoint( inverseOutput, inverseTruth ) )
    {
    std::cout << "Failed transform point with 2nd inverse composite transform."
              << std::endl;
    return EXIT_FAILURE;
    }

  /* Test GetJacobian - simplistic testing for now */
  CompositeType::JacobianType jacobian;
  CompositeType::InputPointType inputPoint;
  inputPoint[0]=1;
  inputPoint[1]=2;
  jacobian = compositeTransform->GetJacobian( inputPoint );

  /* Test parameter get/set - simplistic testing for now */
  CompositeType::ParametersType parameters, fixedParameters;

  parameters = compositeTransform->GetParameters();
  compositeTransform->SetParameters( parameters );

  fixedParameters = compositeTransform->GetFixedParameters();

  unsigned int nParameters = compositeTransform->GetNumberOfParameters();
  std::cout << "Number of parameters: " << nParameters << std::endl;

  return EXIT_SUCCESS;
}
