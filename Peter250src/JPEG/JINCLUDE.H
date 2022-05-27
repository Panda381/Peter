
#define JCONFIG_INCLUDED	/* so that jpeglib.h doesn't do it again */

#define MEMZERO(target,size)	MemFill((void *)(target), (size), 0)
#define MEMCOPY(dest,src,size)	MemCopy((void *)(dest), (const void *)(src), (size_t)(size))

#define SIZEOF(object)	((size_t) sizeof(object))
