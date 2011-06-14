set(ITK-Video-Core-Common_SRC
itkTemporalRegion.cxx
itkTemporalDataObject.cxx
itkTemporalProcessObject.cxx
)

add_library(ITK-Video-Core-Common ${ITK-Video-Core-Common_SRC})
target_link_libraries(ITK-Video-Core-Common ${ITK-Common_LIBRARIES})
itk_module_target(ITK-Video-Core-Common)
