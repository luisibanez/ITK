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
#ifndef __itkVideoSource_txx
#define __itkVideoSource_txx

namespace itk
{

//-CONSTRUCTOR PRINT-----------------------------------------------------------

//
// Constructor
//
template<class TOutputVideoStream>
VideoSource<TOutputVideoStream>::VideoSource()
{
  typename OutputVideoStreamType::Pointer output =
    static_cast< OutputVideoStreamType* >( this->MakeOutput(0).GetPointer() );
  this->ProcessObject::SetNumberOfRequiredOutputs(1);
  this->ProcessObject::SetNthOutput( 0, output.GetPointer() );
}

//-PUBLIC METHODS--------------------------------------------------------------

//
// GetOutput()
//
template<class TOutputVideoStream>
typename VideoSource< TOutputVideoStream >::OutputVideoStreamType*
VideoSource<TOutputVideoStream>::GetOutput()
{
  // Make sure there is at least 1 output
  if (this->GetNumberOfOutputs() < 1)
    {
    itkWarningMacro("No outputs set");
    return NULL;
    }

  // Return the output
  return this->GetOutput(0);
}

//
// GetOutput(idx)
//
template<class TOutputVideoStream>
TOutputVideoStream*
VideoSource<TOutputVideoStream>::GetOutput(unsigned int idx)
{
  OutputVideoStreamType* out = dynamic_cast< OutputVideoStreamType* >
                                (this->TemporalProcessObject::GetOutput(idx));

  // Make sure there is at least 1 output
  if (out == NULL)
    {
    itkWarningMacro("dynamic_cast to output type failed");
    }

  return out;
}

//
// GraftOutput
//
template<class TOutputVideoStream>
void
VideoSource<TOutputVideoStream>::GraftOutput(TOutputVideoStream* graft)
{
  this->GraftNthOutput(0, graft);
}

//
// GraftNthOutput
//
template<class TOutputVideoStream>
void
VideoSource<TOutputVideoStream>::
GraftNthOutput(unsigned int idx, TOutputVideoStream* graft)
{
  if (idx >= this->GetNumberOfOutputs() )
    {
    itkExceptionMacro(<< "Requested to graft output " << idx
                      << " but this VideoSource only has "
                      << this->GetNumberOfOutputs() << " Outputs.");
    }
  if (!graft)
    {
    itkExceptionMacro("Cannot graft from a NULL pointer");
    }

  // we use the process object method since all our outputs may not be of the
  // same type
  DataObject* output = this->ProcessObject::GetOutput(idx);
  output->Graft(graft);
}

//
// MakeOutput
//
template<class TOutputVideoStream>
DataObject::Pointer
VideoSource<TOutputVideoStream>::MakeOutput(unsigned int idx)
{
  return static_cast< DataObject* >( OutputVideoStreamType::New().GetPointer() );
}


//-PROTECTED METHODS-----------------------------------------------------------

//
// GenerateOutputRequestedRegion
//
template<class TOutputVideoStream>
void
VideoSource<TOutputVideoStream>::
GenerateOutputRequestedTemporalRegion(TemporalDataObject* output)
{
  // Check if requested temporal region unset
  bool resetNumFrames = false;
  TemporalRegion outputRequest = output->GetRequestedTemporalRegion();
  if (!outputRequest.GetFrameDuration())
    {
    resetNumFrames = true;
    }

  // Call superclass's version - this will set the requested temporal region
  Superclass::GenerateOutputRequestedTemporalRegion(this->GetOutput());

  // Make sure the output has enough buffers available for the entire output
  // only if this request has just been matched to the largest possible spatial
  // region. This should only happen for filters at the end of the pipeline
  // since mid-pipeline filters will have their outputs' requested temporal
  // regions set automatically.
  unsigned long requestDuration =
    this->GetOutput()->GetRequestedTemporalRegion().GetFrameDuration();
  if (resetNumFrames && this->GetOutput()->GetNumberOfBuffers() < requestDuration)
    {
    this->GetOutput()->SetNumberOfBuffers(requestDuration);
    }
}

//
// AllocateOutputs
//
template<class TOutputVideoStream>
void
VideoSource<TOutputVideoStream>::AllocateOutputs()
{
  // Get the output
  OutputVideoStreamType* output = this->GetOutput();

  // Get a list of unbuffered requested frames
  TemporalRegion unbufferedRegion = output->GetUnbufferedRequestedTemporalRegion();

  // We don't touch the buffered temporal region here because that is handled
  // by the default implementation of GenerateData in TemporalProcessObject

  // If there are no unbuffered frames, return now
  unsigned long numFrames = unbufferedRegion.GetFrameDuration();
  if (numFrames == 0)
    {
    return;
    }

  // Initialize any empty frames (which will set region values from cache)
  output->InitializeEmptyFrames();

  // Loop through the unbuffered frames and set the buffered region to match
  // the requested region then allocate the data
  unsigned long startFrame = unbufferedRegion.GetFrameStart();
  for (unsigned long i = startFrame; i < startFrame + numFrames; ++i)
    {
    OutputFrameType* frame = output->GetFrame(i);
    frame->SetBufferedRegion(output->GetFrame(i)->GetRequestedRegion());
    frame->Allocate();
    }
}

//
// TemporalStreamingGenerateData
//
template<class TOutputVideoStream>
void
VideoSource<TOutputVideoStream>::
TemporalStreamingGenerateData()
{
  // Call a method that can be overriden by a subclass to allocate
  // memory for the filter's outputs
  this->AllocateOutputs();

  // Call a method that can be overridden by a subclass to perform
  // some calculations prior to splitting the main computations into
  // separate threads
  this->BeforeThreadedGenerateData();

  // Set up the multithreaded processing
  ThreadStruct str;
  str.Filter = this;

  this->GetMultiThreader()->SetNumberOfThreads( this->GetNumberOfThreads() );
  this->GetMultiThreader()->SetSingleMethod(this->ThreaderCallback, &str);

  // multithread the execution
  this->GetMultiThreader()->SingleMethodExecute();

  // Call a method that can be overridden by a subclass to perform
  // some calculations after all the threads have completed
  this->AfterThreadedGenerateData();
}

//
// ThreadedGenerateData
//
template<class TOutputVideoStream>
void
VideoSource<TOutputVideoStream>::
ThreadedGenerateData(
  const typename TOutputVideoStream::SpatialRegionType& outputRegionForThread,
  int threadId)
{
  itkExceptionMacro( << "itk::ERROR: " << this->GetNameOfClass()
                     << "(" << this << "): "
                     << "Subclass should override this method!!!");
}

//
// SplitRequestedSpatialRegion -- Copied mostly from ImageSource
//
// Note: This implementation bases the spatial region split on the requested
// spatial region for the current Head frame of the output. This could
// potentially cause issues if frames are different sized.
//
template<class TOutputVideoStream>
int
VideoSource<TOutputVideoStream>::
SplitRequestedSpatialRegion(int i, int num,
  typename TOutputVideoStream::SpatialRegionType& splitRegion)
{
  // Get the output pointer and a pointer to the first output frame
  OutputVideoStreamType* outputPtr = this->GetOutput();
  unsigned long currentFrame = outputPtr->GetRequestedTemporalRegion().GetFrameStart();
  OutputFrameType* framePtr = outputPtr->GetFrame(currentFrame);

  const typename TOutputVideoStream::SizeType & requestedRegionSize =
    framePtr->GetRequestedRegion().GetSize();

  int splitAxis;
  typename TOutputVideoStream::IndexType splitIndex;
  typename TOutputVideoStream::SizeType splitSize;

  // Initialize the splitRegion to the output requested region
  splitRegion = framePtr->GetRequestedRegion();
  splitIndex = splitRegion.GetIndex();
  splitSize = splitRegion.GetSize();

  // split on the outermost dimension available
  splitAxis = framePtr->GetImageDimension() - 1;
  while ( requestedRegionSize[splitAxis] == 1 )
    {
    --splitAxis;
    if ( splitAxis < 0 )
      { // cannot split
      itkDebugMacro("  Cannot Split");
      return 1;
      }
    }

  // determine the actual number of pieces that will be generated
  typename TOutputVideoStream::SizeType::SizeValueType range = requestedRegionSize[splitAxis];
  int valuesPerThread = Math::Ceil< int >(range / (double)num);
  int maxThreadIdUsed = Math::Ceil< int >(range / (double)valuesPerThread) - 1;

  // Split the region
  if ( i < maxThreadIdUsed )
    {
    splitIndex[splitAxis] += i * valuesPerThread;
    splitSize[splitAxis] = valuesPerThread;
    }
  if ( i == maxThreadIdUsed )
    {
    splitIndex[splitAxis] += i * valuesPerThread;
    // last thread needs to process the "rest" dimension being split
    splitSize[splitAxis] = splitSize[splitAxis] - i * valuesPerThread;
    }

  // set the split region ivars
  splitRegion.SetIndex(splitIndex);
  splitRegion.SetSize(splitSize);

  itkDebugMacro("  Split Piece: " << splitRegion);

  return maxThreadIdUsed + 1;
}

//
// ThreaderCallback -- Copied from ImageSource
//
template<class TOutputVideoStream>
ITK_THREAD_RETURN_TYPE
VideoSource<TOutputVideoStream>::
ThreaderCallback(void* arg)
{
  ThreadStruct *str;
  int           total, threadId, threadCount;

  threadId = ( (MultiThreader::ThreadInfoStruct *)( arg ) )->ThreadID;
  threadCount = ( (MultiThreader::ThreadInfoStruct *)( arg ) )->NumberOfThreads;

  str = (ThreadStruct *)( ( (MultiThreader::ThreadInfoStruct *)( arg ) )->UserData );

  // execute the actual method with appropriate output region
  // first find out how many pieces extent can be split into.
  typename TOutputVideoStream::SpatialRegionType splitRegion;
  total = str->Filter->SplitRequestedSpatialRegion(threadId, threadCount, splitRegion);

  if ( threadId < total )
    {
    str->Filter->ThreadedGenerateData(splitRegion, threadId);
    }
  // else
  //   {
  //   otherwise don't use this thread. Sometimes the threads dont
  //   break up very well and it is just as efficient to leave a
  //   few threads idle.
  //   }

  return ITK_THREAD_RETURN_VALUE;
}


} // end namespace itk

#endif
