project(Video-Filters-OpenCV)

find_package ( OpenCV REQUIRED )

set(Video-Filters-OpenCV_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS} )
itk_module_impl()
