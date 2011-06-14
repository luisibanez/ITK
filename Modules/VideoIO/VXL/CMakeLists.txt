project(ITK-Video-IO-VXL)

find_package ( VXL REQUIRED )
include( ${VXL_CMAKE_DIR}/UseVXL.cmake )

set(ITK-Video-IO-VXL_INCLUDE_DIRS ${VXL_INCLUDE_DIRS} )
set(ITK-Video-IO-VXL_LIBRARIES ITK-Video-IO-VXL ITK-Video-Core-Common vcl vidl avcodec avformat)
itk_module_impl()
