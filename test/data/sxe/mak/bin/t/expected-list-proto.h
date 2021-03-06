/*
 * Generated by genxface.pl - DO NOT EDIT OR COMMIT TO SOURCE CODE CONTROL!
 */
#ifdef __cplusplus
extern "C" {
#endif

List List_clear(List list) ;
List List_concat(List list, List list2) ;
void List_delete(List list) ;
unsigned List_defined(List list, unsigned index) ;
List_type List_get(List list, unsigned index) ;
List List_grep(List list, unsigned (*predicate)(void *)) ;
unsigned List_grep_length(List list, unsigned (*predicate)(void *)) ;
char * List_join_alloc(List list, char * delim) ;
char * strn_copy(char * buf, const char * str, unsigned size) ;
char * List_join_strncpy(List list, char * delim, unsigned size, char * buf) ;
unsigned List_length(List list) ;
List List_map(List list, void * (*func)(void *)) ;
List List_new(void) ;
List_type List_pop(List list) ;
unsigned List_push(List list, List_type elem) ;
List List_reverse(List list) ;
List_type List_set(List list, unsigned index, List_type elem) ;
List_type List_shift(List list) ;
List List_sort(List list, int (*cmp)(const void *, const void *)) ;
List List_split_strdup(char * delim, char * str) ;
List List_split_strncpy(char * delim, unsigned length, char * buf, char * str) ;
List List_split_strtok(char * delim, char * str) ;
List List_vnew(unsigned number, List_type elem, ...) ;
unsigned List_vpush(List list, unsigned number, List_type elem, ...) ;

#ifdef __cplusplus
}
#endif
