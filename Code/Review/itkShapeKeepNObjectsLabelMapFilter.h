/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkShapeKeepNObjectsLabelMapFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShapeKeepNObjectsLabelMapFilter_h
#define __itkShapeKeepNObjectsLabelMapFilter_h

#include "itkInPlaceLabelMapFilter.h"
#include "itkLabelObjectAccessors.h"
#include "itkShapeLabelObjectAccessors.h"
#include "itkProgressReporter.h"

namespace itk
{
/** \class ShapeKeepNObjectsLabelMapFilter
 * \brief Keep N objects according to their shape attributes.
 *
 * The ShapeKeepNObjectsLabelMapFilter keeps N objects in a label collection image with the
 * highest (or lowest) attribute value. The attributes values are those of the ShapeLabelObject.
 *
 * This implementation was taken from the Insight Journal paper:
 * http://hdl.handle.net/1926/584  or
 * http://www.insight-journal.org/browse/publication/176
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa ShapeLabelObject, BinaryShapeKeepNObjectsImageFilter, LabelStatisticsKeepNObjectsImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template< class TImage >
class ITK_EXPORT ShapeKeepNObjectsLabelMapFilter:
  public InPlaceLabelMapFilter< TImage >
{
public:
  /** Standard class typedefs. */
  typedef ShapeKeepNObjectsLabelMapFilter Self;
  typedef InPlaceLabelMapFilter< TImage > Superclass;
  typedef SmartPointer< Self >            Pointer;
  typedef SmartPointer< const Self >      ConstPointer;

  /** Some convenient typedefs. */
  typedef TImage                              ImageType;
  typedef typename ImageType::Pointer         ImagePointer;
  typedef typename ImageType::ConstPointer    ImageConstPointer;
  typedef typename ImageType::PixelType       PixelType;
  typedef typename ImageType::IndexType       IndexType;
  typedef typename ImageType::LabelObjectType LabelObjectType;

  typedef typename LabelObjectType::AttributeType AttributeType;

  typedef typename Superclass::LabelObjectContainerType LabelObjectContainerType;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int, TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ShapeKeepNObjectsLabelMapFilter, InPlaceLabelMapFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
/*  itkConceptMacro(InputEqualityComparableCheck,
    (Concept::EqualityComparable<InputImagePixelType>));
  itkConceptMacro(IntConvertibleToInputCheck,
    (Concept::Convertible<int, InputImagePixelType>));
  itkConceptMacro(InputOStreamWritableCheck,
    (Concept::OStreamWritable<InputImagePixelType>));*/
/** End concept checking */
#endif

  /**
   * Set/Get the ordering of the objects. By default, the ones with the
   * highest value are kept. Turning ReverseOrdering to true make this filter
   * keep the objects with the smallest values
   */
  itkSetMacro(ReverseOrdering, bool);
  itkGetConstReferenceMacro(ReverseOrdering, bool);
  itkBooleanMacro(ReverseOrdering);

  /**
   * Set/Get the number of objects to keep
   */
  itkSetMacro(NumberOfObjects, unsigned long);
  itkGetConstReferenceMacro(NumberOfObjects, unsigned long);

  /**
   * Set/Get the attribute to use to select the object to keep.
   * Default is "Size".
   */
  itkGetConstMacro(Attribute, AttributeType);
  itkSetMacro(Attribute, AttributeType);

  void SetAttribute(const std::string & s)
  {
    this->SetAttribute( LabelObjectType::GetAttributeFromName(s) );
  }

protected:
  ShapeKeepNObjectsLabelMapFilter();
  ~ShapeKeepNObjectsLabelMapFilter() {}

  void GenerateData();

  template< class TAttributeAccessor >
  void TemplatedGenerateData(const TAttributeAccessor &)
  {
    // Allocate the output
    this->AllocateOutputs();

    ImageType *output = this->GetOutput();
    ImageType *output2 = this->GetOutput(1);

    // set the background value for the second output - this is not done in the
    // superclasses
    output2->SetBackgroundValue( output->GetBackgroundValue() );

    const LabelObjectContainerType & labelObjectContainer = output->GetLabelObjectContainer();
    typedef typename LabelObjectType::Pointer LabelObjectPointer;
    typedef std::vector< LabelObjectPointer > VectorType;

    ProgressReporter progress( this, 0, 2 * labelObjectContainer.size() );

    // get the label objects in a vector, so they can be sorted
    VectorType labelObjects;
    labelObjects.reserve( labelObjectContainer.size() );
    typename LabelObjectContainerType::const_iterator it = labelObjectContainer.begin();
    while ( it != labelObjectContainer.end() )
      {
      labelObjects.push_back(it->second);
      progress.CompletedPixel();
      it++;
      }

    // instantiate the comparator and sort the vector
    if ( m_NumberOfObjects < labelObjectContainer.size() )
      {
      typename VectorType::iterator end = labelObjects.begin() + m_NumberOfObjects;
      if ( m_ReverseOrdering )
        {
        Functor::LabelObjectReverseComparator< LabelObjectType, TAttributeAccessor > comparator;
        std::nth_element(labelObjects.begin(), end, labelObjects.end(), comparator);
        }
      else
        {
        Functor::LabelObjectComparator< LabelObjectType, TAttributeAccessor > comparator;
        std::nth_element(labelObjects.begin(), end, labelObjects.end(), comparator);
        }
      progress.CompletedPixel();

      // and remove the last objects of the map
      for ( typename VectorType::const_iterator it2 = end;
            it2 != labelObjects.end();
            it2++ )
        {
        output2->AddLabelObject(*it2);
        output->RemoveLabelObject(*it2);
        progress.CompletedPixel();
        }
      }
  }

  void PrintSelf(std::ostream & os, Indent indent) const;

  bool m_ReverseOrdering;

  unsigned long m_NumberOfObjects;
  AttributeType m_Attribute;
private:
  ShapeKeepNObjectsLabelMapFilter(const Self &); //purposely not implemented
  void operator=(const Self &);                  //purposely not implemented
};                                               // end of class
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkShapeKeepNObjectsLabelMapFilter.txx"
#endif

#endif
