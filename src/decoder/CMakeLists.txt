
set(decoder_source_files  
	dct.cc
	decoder.cc
	frame.cc
	frame_pool.cc
	loopfilter.cc
	macroblock.cc
	modemv_data.cc
	player.cc
	prediction.cc
	probability_tables.cc
	quantization.cc
	raster_handle.cc
	safe_raster.cc
	tokens.cc
	transform.cc
	tree.cc
	uncompressed_chunk.cc
	vp8_prob_data.cc
    )

add_library(decoder_lib ${encoder_source_files})
target_link_libraries(decoder_lib messages)

