project(ITK-Video-IO-OpenCV)

find_package ( OpenCV REQUIRED )

set(ITK-Video-IO-OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} )
set(ITK-Video-IO-OpenCV_LIBRARIES ITK-Video-Core-Common ITK-Video-IO-OpenCV ${OpenCV_LIBS})

itk_module_impl()
