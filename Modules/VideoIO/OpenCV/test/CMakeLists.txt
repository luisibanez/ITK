itk_module_test()
set(ITK-Video-IO-OpenCVTests
  itkOpenCVVideoIOTest.cxx
  itkOpenCVVideoIOFactoryTest.cxx
)

CreateTestDriver(ITK-Video-IO-OpenCV  "${ITK-Video-IO-OpenCV-Test_LIBRARIES}" "${ITK-Video-IO-OpenCVTests}")

# OpenCVVideoIO:
add_video_test(NAME OpenCVVideoIOTest
               COMMAND ITK-Video-IO-OpenCVTestDriver itkOpenCVVideoIOTest
               # Video Input:
               MIDAS{inde-circulation.avi.md5}
               # Non-Video Input:
               ${ITKVideo_Data_DIR}/Input/TestParameters.txt
               # Output:
               ${ITK_TEST_OUTPUT_DIR}/OpenCVVideoIOTest_Out.avi
               # Webcam Output:
               ${ITK_TEST_OUTPUT_DIR}/OpenCVVideoIOTest_CameraOut.mha
               # Width Height NumFrames FpS
               320 240 1396 14.985
               )

# OpenCVVideoIOFactory:
add_video_test(NAME OpenCVVideoIOFactoryTest
               COMMAND ITK-Video-IO-OpenCVTestDriver itkOpenCVVideoIOFactoryTest
               # Video Input:
               MIDAS{inde-circulation.avi.md5}
               # Output:
               ${ITK_TEST_OUTPUT_DIR}/OpenCVVideoIOTest_Out.avi
               # Webcam Number:
               0
               )
