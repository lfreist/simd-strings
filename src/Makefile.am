# project_name/src/Makefile.am
lib_LIBRARIES = libsimdstr.a
libsimdstr_a_SOURCES = ${src_files}
libsimdstr_a_CFLAGS = -I$(top_srcdir)/include
all-local:
	@mkdir -p $(top_builddir)/build
	@mv libsimdstr.a $(top_builddir)/build/

