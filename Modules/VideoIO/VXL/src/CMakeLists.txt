set(ITK-Video-IO-VXL_SRC
itkVXLVideoIO.cxx
itkVXLVideoIOFactory.cxx
)

add_library(ITK-Video-IO-VXL ${ITK-Video-IO-VXL_SRC})
target_link_libraries(ITK-Video-IO-VXL ${ITK-Common_LIBRARIES} ${ITK-IO-Base} ITK-Video-IO-Base ${TEST_DEPENDS_LIBRARIES} ${ITK-TestKernel_LIBRARIES})
itk_module_target(ITK-Video-IO-VXL)
