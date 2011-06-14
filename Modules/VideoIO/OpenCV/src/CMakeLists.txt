set(ITK-Video-IO-OpenCV_SRC
itkOpenCVVideoIO.cxx
itkOpenCVVideoIOFactory.cxx
)

add_library(ITK-Video-IO-OpenCV ${ITK-Video-IO-OpenCV_SRC})
target_link_libraries(ITK-Video-IO-OpenCV
  ${ITK-Common_LIBRARIES}
  ${ITK-IO-Base} ITK-Video-IO-Base
  ${TEST_DEPENDS_LIBRARIES}
  ${ITK-TestKernel_LIBRARIES}
  )

itk_module_target(ITK-Video-IO-OpenCV)
