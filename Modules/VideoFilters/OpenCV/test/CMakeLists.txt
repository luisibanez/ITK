itk_module_test()
set(ITK-Video-Filters-OpenCVTests
  itkFaceDetectionFilterTest.cxx
)

CreateTestDriver(ITK-Video-Filters-OpenCV  "${ITK-Video-Filters-OpenCV-Test_LIBRARIES}" "${ITK-Video-Filters-OpenCVTests}")

#Face detection :
add_video_test(NAME ITKVideoTestFaceDetection
               COMMAND ITK-Video-Filters-OpenCVTestDriver itkFaceDetectionFilterTest
               #Input
               ${ITKVideo_Data_DIR}/faces3.png
               #Main Output
               ${CMAKE_CURRENT_BINARY_DIR}/Testing/Temporary/FaceDetectionFilterResult.png
               #Faces whithout extension :
               ${CMAKE_CURRENT_BINARY_DIR}/Testing/Temporary/Face_
               #Trainer
               ${ITKVideo_Data_DIR}/haarcascade_frontalface_alt2.xml)

