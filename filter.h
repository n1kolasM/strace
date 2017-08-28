/*
 * Copyright (c) 2017 Nikolay Marchuk <marchuk.nikolay.a@gmail.com>
 * Copyright (c) 2017 The strace developers.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef STRACE_FILTER_H
#define STRACE_FILTER_H

struct number_set;

struct filter;

struct filter_action;

struct bool_expression;

typedef int (*string_to_uint_func)(const char *);

void qualify_tokens(const char *str, struct number_set *set,
		    string_to_uint_func func, const char *name);
void qualify_syscall_tokens(const char *str, struct number_set *set,
			    const char *name);
bool is_traced(struct tcb *);

/* filter api */
struct filter* add_filter_to_array(struct filter **, unsigned int *nfilters,
				   const char *name);
void parse_filter(struct filter *, const char *str);
void run_filters(struct tcb *, struct filter *, unsigned int, bool *);
void free_filter(struct filter *);
void set_filters_qualify_mode(struct filter **, unsigned int *nfilters,
			      unsigned int filters_left);

/* filter action api */
struct filter *create_filter(struct filter_action *, const char *name);
struct filter_action *find_or_add_action(const char *);
void set_qualify_mode(struct filter_action *, unsigned int);

/* filter expression api */
struct bool_expression *create_expression();
bool run_expression(struct bool_expression *, bool *, unsigned int);
void set_expression_qualify_mode(struct bool_expression *, unsigned int);

#define DECL_FILTER(name)						\
extern void *								\
parse_ ## name ## _filter(const char *);				\
extern bool								\
run_ ## name ## _filter(struct tcb *, void *);				\
extern void								\
free_ ## name ## _filter(void *)					\
/* End of DECL_FILTER definition. */

DECL_FILTER(syscall);
#undef DECL_FILTER

#define DECL_FILTER_ACTION(name)					\
extern void								\
apply_ ## name(struct tcb *, void *)					\
/* End of DECL_FILTER_ACTION definition. */

DECL_FILTER_ACTION(trace);
DECL_FILTER_ACTION(raw);
DECL_FILTER_ACTION(abbrev);
DECL_FILTER_ACTION(verbose);
#undef DECL_FILTER_ACTION

#define DECL_FILTER_ACTION_PARSER(name)					\
extern void *								\
parse_ ## name(const char *);						\
/* End of DECL_FILTER_ACTION_PARSER definition. */

DECL_FILTER_ACTION_PARSER(null);
#undef DECL_FILTER_ACTION_PARSER

#endif /* !STRACE_FILTER_H */
