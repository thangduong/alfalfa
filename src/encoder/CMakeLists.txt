
# Copyright 2017 Robo Systems
#
# CMakeLists.txt
#
# Cmake file to build the Command Manager.  Add new
#   files to the list of command_manager_source_files below

set(encoder_source_files costs.cc 
    encode_inter.cc
	encode_intra.cc
	encode_tree.cc
	encoder.cc
	reencode.cc
	safe_references.cc
	serializer.cc
	size_estimation.cc
	variance.cc
	variance_sse2.cc
    )

add_library(encoder_lib ${encoder_source_files})
target_link_libraries(encoder_lib messages)

