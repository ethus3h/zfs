/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/* Do not include this file directly. Please use <sys/trace.h> instead. */
#ifndef _SYS_TRACE_DBGMSG_INDIRECT
#error "trace_dbgmsg.h included directly"
#endif

/*
 * This file defines tracepoint events for use by the dbgmsg(),
 * dprintf(), and SET_ERROR() interfaces. These are grouped here because
 * they all provide a way to store simple messages in the debug log (as
 * opposed to events used by the DTRACE_PROBE interfaces which typically
 * dump structured data).
 *
 * This header is included inside the trace.h multiple inclusion guard,
 * and it is guarded above against direct inclusion, so it and need not
 * be guarded separately.
 */

/*
 * Generic support for four argument tracepoints of the form:
 *
 * DTRACE_PROBE4(...,
 *     const char *, ...,
 *     const char *, ...,
 *     int, ...,
 *     const char *, ...);
 */
/* BEGIN CSTYLED */
DECLARE_EVENT_CLASS(zfs_dprintf_class,
	TP_PROTO(const char *file, const char *function, int line,
	    const char *msg),
	TP_ARGS(file, function, line, msg),
	TP_STRUCT__entry(
	    __string(file, file)
	    __string(function, function)
	    __field(int,		line)
	    __string(msg, msg)
	),
	TP_fast_assign(
	    __assign_str(file, strchr(file, '/') ?
		strrchr(file, '/') + 1 : file)
	    __assign_str(function, function);
	    __entry->line		= line;
	    __assign_str(msg, msg);
	),
	TP_printk("%s:%d:%s(): %s", __get_str(file), __entry->line,
	    __get_str(function), __get_str(msg))
);
/* END CSTYLED */

#define	DEFINE_DPRINTF_EVENT(name) \
/* BEGIN CSTYLED */
DEFINE_EVENT(zfs_dprintf_class, name, \
	TP_PROTO(const char *file, const char *function, int line, \
	    const char *msg), \
	TP_ARGS(file, function, line, msg))
/* END CSTYLED */
DEFINE_DPRINTF_EVENT(zfs_zfs__dprintf);

/*
 * Generic support for four argument tracepoints of the form:
 *
 * DTRACE_PROBE4(...,
 *     const char *, ...,
 *     const char *, ...,
 *     int, ...,
 *     uintptr_t, ...);
 */
/* BEGIN CSTYLED */
DECLARE_EVENT_CLASS(zfs_set_error_class,
	TP_PROTO(const char *file, const char *function, int line,
	    uintptr_t error),
	TP_ARGS(file, function, line, error),
	TP_STRUCT__entry(
	    __string(file, file)
	    __string(function, function)
	    __field(int,		line)
	    __field(uintptr_t,		error)
	),
	TP_fast_assign(
	    __assign_str(file, strchr(file, '/') ?
		strrchr(file, '/') + 1 : file)
	    __assign_str(function, function);
	    __entry->line		= line;
	    __entry->error		= error;
	),
	TP_printk("%s:%d:%s(): error 0x%lx", __get_str(file), __entry->line,
	    __get_str(function), __entry->error)
);
/* END CSTYLED */

#ifdef TP_CONDITION
#define	DEFINE_SET_ERROR_EVENT(name) \
/* BEGIN CSTYLED */
DEFINE_EVENT_CONDITION(zfs_set_error_class, name, \
	TP_PROTO(const char *file, const char *function, int line, \
	    uintptr_t error), \
	TP_ARGS(file, function, line, error), \
	TP_CONDITION(error))
/* END CSTYLED */
#else
#define	DEFINE_SET_ERROR_EVENT(name) \
/* BEGIN CSTYLED */
DEFINE_EVENT(zfs_set_error_class, name, \
	TP_PROTO(const char *file, const char *function, int line, \
	    uintptr_t error), \
	TP_ARGS(file, function, line, error))
/* END CSTYLED */
#endif
DEFINE_SET_ERROR_EVENT(zfs_set__error);
