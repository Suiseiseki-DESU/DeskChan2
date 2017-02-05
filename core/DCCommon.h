#pragma once

#ifdef DC_CORE_LIBRARY
#define DC_CORE_DECL Q_DECL_EXPORT
#else
#define DC_CORE_DECL Q_DECL_IMPORT
#endif
