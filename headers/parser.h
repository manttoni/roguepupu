#ifndef PARSER_H
#define PARSER_H

t_fungus	*parse_fungi(char *raw, int *count);
t_item		*parse_items(char *raw, char *type, int *count);
t_creature	*parse_creatures(char *raw, char *type, int *count);
t_area		*parse_area(char *raw);

#endif
