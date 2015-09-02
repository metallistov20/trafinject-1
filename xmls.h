#ifndef _XMLS_H_
#define _XMLS_H_

char * aLastToken[3];

#define print_element_names(x) _print_element_names(__func__, (x))

#define find_named_element(x,y) _find_named_element(__func__, (x), (y))

#endif /* _XMLS_H_ */
