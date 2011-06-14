project(ITK-Video-Core-Common)

# For Midas-stored testing data
include(MIDAS.cmake)
set( MIDAS_REST_URL "http://midas.kitware.com/api/rest" CACHE INTERNAL "")
set( MIDAS_KEY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test_data_hashes" CACHE INTERNAL "")

set( ITK-Video-Core-Common_LIBRARIES ITK-Video-Core-Common )
itk_module_impl()
