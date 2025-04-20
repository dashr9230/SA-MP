//  SA-MP Native SQLite Database Functions
//
//  (c) Copyright 2015, SA-MP Team
//

#if defined _sampdb_included
	#endinput
#endif
#define _sampdb_included
#pragma library sampdb

native DB:db_open(name[]);
native db_close(DB:db);
native DBResult:db_query(DB:db, query[]);
native db_free_result(DBResult:dbresult);
native db_num_rows(DBResult:dbresult);
native db_next_row(DBResult:dbresult);
native db_num_fields(DBResult:dbresult);
native db_field_name(DBResult:dbresult, field, result[], maxlength);
native db_get_field(DBResult:dbresult, field, result[], maxlength);
native db_get_field_int(DBResult:result, field = 0);
native Float:db_get_field_float(DBResult:result, field = 0);
native db_get_field_assoc(DBResult:dbresult, const field[], result[], maxlength);
native db_get_field_assoc_int(DBResult:result, const field[]);
native Float:db_get_field_assoc_float(DBResult:result, const field[]);
native db_get_mem_handle(DB:db);
native db_get_result_mem_handle(DBResult:result);

native db_debug_openfiles();
native db_debug_openresults();