set(ITK-Video-IO-Base_SRC
itkVideoIOBase.cxx
itkVideoIOFactory.cxx
)

add_library(ITK-Video-IO-Base ${ITK-Video-IO-Base_SRC})
target_link_libraries(ITK-Video-IO-Base ${ITK-Common_LIBRARIES} ${ITK-IO-Base} ${TEST_DEPENDS_LIBRARIES} ${ITK-TestKernel_LIBRARIES})
itk_module_target(ITK-Video-IO-Base)
