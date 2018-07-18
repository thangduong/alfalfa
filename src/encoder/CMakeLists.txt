
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

