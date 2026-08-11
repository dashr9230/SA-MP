#ifndef _INCLUDE_FORMATTING_H
#define _INCLUDE_FORMATTING_H

// Templated AMX/Pawn formatter used by the server's native formatting helpers.
template <typename D, typename S>
size_t atcprintf(D *buffer, size_t maxlen, const S *format, AMX *amx, cell *params, int *param);

#endif // _INCLUDE_FORMATTING_H
