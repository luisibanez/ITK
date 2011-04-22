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

#include "itkVideoSource.h"
#include "itkImageRegionIterator.h"

namespace itk
{
namespace test
{
/** \class DummyVideoSource
 * Provide dummy implementation of VideoSource that just sets all pixels to 1
 */
template<class TOutputVideoStream>
class DummyVideoSource : public VideoSource<TOutputVideoStream>
{
public:

  /** Standard class typedefs */
  typedef TOutputVideoStream                         OutputVideoStreamType;
  typedef DummyVideoSource< OutputVideoStreamType >  Self;
  typedef VideoSource< OutputVideoStreamType >       Superclass;
  typedef SmartPointer< Self >                       Pointer;
  typedef SmartPointer< const Self >                 ConstPointer;
  typedef WeakPointer< const Self >                  ConstWeakPointer;

  typedef typename TOutputVideoStream::FrameType OutputFrameType;
  typedef typename OutputFrameType::RegionType   OutputFrameSpatialRegionType;

  itkNewMacro(Self);

  itkTypeMacro(DummyVideoSource, VideoSource);

protected:

  /** Constructor */
  DummyVideoSource()
    {
    this->TemporalProcessObject::m_UnitInputNumberOfFrames = 1;
    this->TemporalProcessObject::m_UnitOutputNumberOfFrames = 1;
    this->TemporalProcessObject::m_FrameSkipPerOutput = 1;
    this->TemporalProcessObject::m_InputStencilCurrentFrameIndex = 1;
    }

  /** Override ThreadedGenerateData to set all pixels in the requested region
   * to 1 */
  virtual void ThreadedGenerateData(
                const OutputFrameSpatialRegionType& outputRegionForThread,
                int threadId)
    {

    // Print out your threadId
    std::cout << "Working on thread " << threadId << std::endl;

    OutputVideoStreamType* video = this->GetOutput();
    typename OutputVideoStreamType::TemporalRegionType requestedTemporalRegion =
      video->GetRequestedTemporalRegion();
    unsigned long startFrame = requestedTemporalRegion.GetFrameStart();
    unsigned long frameDuration = requestedTemporalRegion.GetFrameDuration();

    // Just as a check, throw an exception if the duration isn't equal to the
    // unit output size
    if (frameDuration != this->TemporalProcessObject::m_UnitOutputNumberOfFrames)
      {
      itkExceptionMacro(<< "Trying to generate output of non-unit size. Got: "
                        << frameDuration << " Expected: "
                        << this->TemporalProcessObject::m_UnitOutputNumberOfFrames);
      }

    for (unsigned long i = startFrame; i < startFrame + frameDuration; ++i)
      {
      OutputFrameType* frame = video->GetFrame(i);
      itk::ImageRegionIterator<OutputFrameType> iter(frame, outputRegionForThread);
      while(!iter.IsAtEnd())
        {
        // Set the pixel to 1
        iter.Set(1);
        ++iter;
        }
      }
    }
};


} // end namespace test
} // end namespace itk


// Set up typedefs
const unsigned int Dimension =                   2;
typedef unsigned char                            PixelType;
typedef itk::Image< PixelType, Dimension >       FrameType;
typedef itk::VideoStream< FrameType >            VideoType;
typedef itk::test::DummyVideoSource< VideoType > VideoSourceType;

/**
 * Create a new empty frame
 */
FrameType::Pointer CreateEmptyFrame()
{
  FrameType::Pointer out = FrameType::New();

  FrameType::RegionType largestRegion;
  FrameType::SizeType sizeLR;
  FrameType::IndexType startLR;
  startLR.Fill(0);
  sizeLR[0] = 50;
  sizeLR[1] = 40;
  largestRegion.SetSize(sizeLR);
  largestRegion.SetIndex(startLR);
  out->SetLargestPossibleRegion(largestRegion);

  FrameType::RegionType requestedRegion;
  FrameType::SizeType sizeReq;
  FrameType::IndexType startReq;
  startReq.Fill(2);
  sizeReq[0] = 20;
  sizeReq[1] = 10;
  requestedRegion.SetSize(sizeReq);
  requestedRegion.SetIndex(startReq);
  out->SetRequestedRegion(requestedRegion);

  out->Allocate();

  return out;
}

/**
 * Test the basic functionality of temporal data objects
 */
int itkVideoSourceTest( int argc, char* argv[] )
{

  //////
  // Test Instantiation
  //////

  VideoSourceType::Pointer videoSource = VideoSourceType::New();


  //////
  // Test Graft
  //////

  // Create a VideoStream
  VideoType::Pointer video = VideoType::New();
  itk::TemporalRegion largestRegion;
  itk::TemporalRegion requestedRegion;
  itk::TemporalRegion bufferedRegion;
  largestRegion.SetFrameStart(0);
  largestRegion.SetFrameDuration(10);
  requestedRegion.SetFrameStart(2);
  requestedRegion.SetFrameDuration(5);
  bufferedRegion.SetFrameStart(1);
  bufferedRegion.SetFrameDuration(8);
  video->SetLargestPossibleTemporalRegion(largestRegion);
  video->SetRequestedTemporalRegion(requestedRegion);
  video->SetBufferedTemporalRegion(bufferedRegion);
  FrameType::Pointer frame;
  for (unsigned long i = bufferedRegion.GetFrameStart();
       i < bufferedRegion.GetFrameStart() + bufferedRegion.GetFrameDuration();
       ++i)
    {
    frame = CreateEmptyFrame();
    video->SetFrame(i, frame);
    }

  // Graft video onto output of VideoSource
  videoSource->GraftOutput(video);

  // Check that graft worked
  VideoType::Pointer videoOut = videoSource->GetOutput();
  if (videoOut->GetLargestPossibleTemporalRegion() != video->GetLargestPossibleTemporalRegion() ||
      videoOut->GetRequestedTemporalRegion() != video->GetRequestedTemporalRegion() ||
      videoOut->GetBufferedTemporalRegion() != video->GetBufferedTemporalRegion())
    {
    std::cerr << "Graft failed to copy meta information" << std::endl;
    return EXIT_FAILURE;
    }
  if (videoOut->GetFrameBuffer() != video->GetFrameBuffer())
    {
    std::cerr << "Graft failed to assign frame buffer correctly" << std::endl;
    return EXIT_FAILURE;
    }


  //////
  // Test ThreadedGenerateDataSystem
  //////

  // Reset videoSource
  videoSource = VideoSourceType::New();

  // Set the requested regions on videoSource's output
  VideoType::Pointer output = videoSource->GetOutput();
  output->SetRequestedTemporalRegion(requestedRegion);
  output->InitializeEmptyFrames();
  FrameType::RegionType spatialRegion = frame->GetRequestedRegion();
  output->SetAllRequestedSpatialRegions(spatialRegion);

  // Call update to set the requested spatial region to 1 for each requested
  // frame
  videoSource->Update();

  // Check the pixel values of the output
  unsigned long frameStart = requestedRegion.GetFrameStart();
  unsigned long numFrames = requestedRegion.GetFrameDuration();
  for (unsigned long i = frameStart; i < frameStart + numFrames; ++i)
    {
    FrameType* frame = videoSource->GetOutput()->GetFrame(i);
    itk::ImageRegionIterator<FrameType> iter( frame, frame->GetRequestedRegion() );
    while (!iter.IsAtEnd())
      {
      if (iter.Get() != 1)
        {
        std::cerr << "Pixel not set correctly" << std::endl;
        return EXIT_FAILURE;
        }
      ++iter;
      }

    // Make sure (0,0) which was outside the requested spatial region didn't
    // get set
    FrameType::IndexType idx;
    idx.Fill(0);
    if (frame->GetPixel(idx) == 1)
      {
      std::cerr << "Pixel outside requested spatial region set to 1" << std::endl;
      return EXIT_FAILURE;
      }
    }


  //////
  // Return Successfully
  //////
  return EXIT_SUCCESS;

}
