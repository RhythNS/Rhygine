add_library(
	stb STATIC
	stb/stb_connected_components.h
	stb/stb_c_lexer.h
	stb/stb_divide.h
	stb/stb_ds.h
	stb/stb_dxt.h
	stb/stb_easy_font.h
	stb/stb_herringbone_wang_tile.h
	stb/stb_hexwave.h
	stb/stb_image.h
	stb/stb_image_resize.h
	stb/stb_image_write.h
	stb/stb_include.h
	stb/stb_leakcheck.h
	stb/stb_rect_pack.h
	stb/stb_sprintf.h
	stb/stb_textedit.h
	stb/stb_tilemap_editor.h
	stb/stb_truetype.h
	stb/stb_vorbis.c
	stb/stb_voxel_render.h
)

target_include_directories (stb PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/stb")
