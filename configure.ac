dnl Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_CONFIG_MACRO_DIR([m4])
AC_INIT([psmisc],[m4_esyscmd(misc/git-version-gen .tarball-version)],[],[],[],[https://www.gitlab.com/psmisc/psmisc])
AM_INIT_AUTOMAKE([foreign 1.11 subdir-objects -Wall -Wno-portability tar-pax no-dist-gzip dist-xz])
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([src/comm.h])
AC_CONFIG_HEADERS([config.h])

dnl Checks for programs.
AC_USE_SYSTEM_EXTENSIONS(_GNU_SOURCE)
AC_PROG_CXX
AC_PROG_LN_S
AC_PROG_CC

AC_DEFUN([PSMISC_PROG_PO4A], [
  AC_REQUIRE([AM_NLS])
  AC_CHECK_PROGS([PO4A], [po4a])
  AS_IF([test "$USE_NLS" = "yes" && test -n "$PO4A"], [
    USE_PO4A=yes
  ], [
    USE_PO4A=no
  ])
  AC_SUBST([USE_PO4A])
])
PSMISC_PROG_PO4A

dnl checks for options
# SELinux support - off by default
DL_LIB=
AC_SUBST([WITH_SELINUX])
AC_ARG_ENABLE([selinux],
  [AS_HELP_STRING([--enable-selinux], [Enable Security-Enhanced Linux features])],
  [enable_selinux=$enableval],
  [enable_selinux="no"])
if test "$enable_selinux" = "yes"; then
  AC_DEFINE([WITH_SELINUX], [1], [Use Security-Enhanced Linux features])
  AC_SEARCH_LIBS([dlopen], [dl], [],
		 [AC_MSG_ERROR([dynamic linking unavailable, circumvent with --disable-selinux])])
  if test "x$ac_cv_search_dlopen" != "xnone required"; then
      DL_LIB="$ac_cv_search_dlopen"
  fi
fi
AC_SUBST([DL_LIB])

# AppArmor support - off by default
DL_LIB=
AC_SUBST([WITH_AppArmor])
AC_ARG_ENABLE([apparmor],
  [AS_HELP_STRING([--enable-apparmor], [Enable AppArmor features])],
  [enable_apparmor=$enableval],
  [enable_apparmor="no"])
if test "$enable_apparmor" = "yes"; then
  AC_DEFINE([WITH_APPARMOR], [1], [Use AppArmor features])
  AC_SEARCH_LIBS([dlopen], [dl], [],
		 [AC_MSG_ERROR([dynamic linking unavailable, circumvent with --disable-apparmor])])
  AC_CHECK_HEADER([sys/apparmor.h], [],
		  [AC_MSG_ERROR([Need AppArmor header files, circumvent with --disable-apparmor])])
  if test "x$ac_cv_search_dlopen" != "xnone required"; then
      DL_LIB="$ac_cv_search_dlopen"
  fi
fi
AC_SUBST([DL_LIB])

AC_CHECK_DECLS([SYS_statx],
  [has_syscall_statx="yes"],
  [has_syscall_statx="no"],
  [[#include <sys/syscall.h>]]
)

# Check for linux specific statx(2) system call
AC_CHECK_HEADERS([sys/syscall.h])
AC_CHECK_FUNCS([statx])
AC_CHECK_DECLS([STATX_TYPE])

AC_SUBST([WITH_STATX])
AC_ARG_ENABLE([statx],
  [AS_HELP_STRING([--disable-statx], [Do not use linux specific statx(2) system call as replacement for stat(2), lstat(2), and fstat(2)])],
  [], [enable_statx="yes"])
if test "$enable_statx" = "yes"; then
  AC_DEFINE([WITH_STATX], [1], [Use statx system call])
  if test "$has_syscall_statx" = "no" && "$have_statx" = "no"; then
      AC_MSG_ERROR([statx() and SYS_statx not found, use --disable-statx to not use statx])
  fi
 fi
 
# Enable hardened compile and link flags
AC_ARG_ENABLE([harden_flags],
  [AS_HELP_STRING([--disable-harden-flags], [disable hardened compilier and linker flags])],
  [enable_harden_flags=$enableval],
  [enable_harden_flags="yes"])

dnl ipv4 only option
AC_SUBST([WITH_IPV6])
AC_ARG_ENABLE([ipv6],
  [AS_HELP_STRING([--disable-ipv6], [Disable IPv6 checks (for uClibc)])],
  [enable_ipv6=$enableval],
  [enable_ipv6="yes"])
if test "$enable_ipv6" = "yes"; then
  AC_DEFINE([WITH_IPV6], [1], [Use IPv6 checks])
fi

dnl Checks for libraries.
AC_CHECK_LIB([tinfo], [tgetent], [TERMCAP_LIB=-ltinfo],
  [ AC_CHECK_LIB([ncurses], [tgetent], [TERMCAP_LIB=-lncurses],
    [ AC_CHECK_LIB([termcap], [tgetent], [TERMCAP_LIB=-ltermcap], 
      [ AC_MSG_ERROR([Cannot find tinfo, ncurses or termcap libraries])
	  ] )
	] )
  ] )
AC_SUBST([TERMCAP_LIB])

dnl Checks for header files.
AC_HEADER_DIRENT

AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([arpa/inet.h fcntl.h langinfo.h libintl.h limits.h locale.h mntent.h netdb.h netinet/in.h stdlib.h string.h sys/ioctl.h sys/socket.h termios.h unistd.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_CHECK_MEMBERS([struct stat.st_rdev])
AC_TYPE_UID_T

# Check that harden CFLAGS and LDFLAGS will compile
AS_IF([test "$enable_harden_flags" = "yes"],
  HARDEN_CFLAGS="-g -O2 -fstack-protector-strong -Wformat -Werror=format-security"
  [HARDEN_LDFLAGS="-Wl,-z,relro"]
  [ AC_MSG_CHECKING([compiler supports harden flags])
  save_harden_cflags="$CFLAGS"
  CFLAGS="$CFLAGS $HARDEN_CFLAGS"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM(,,)],
      [AC_MSG_RESULT([yes])],
     [AC_MSG_RESULT([no]); HARDEN_CFLAGS='']
    )
  CFLAGS="$save_harden_cflags"],
  [HARDEN_CFLAGS=""
   HARDEN_LDFLAGS=""])
AC_SUBST([HARDEN_CFLAGS])
AC_SUBST([HARDEN_LDFLAGS])

dnl Check for language stuff
AM_GNU_GETTEXT_VERSION([0.21])
AM_GNU_GETTEXT([external])

dnl Must be after the gettext stuff as it sets build_cpu
AC_CHECK_MEMBERS([struct user_regs_struct.orig_eax,
		struct user_regs_struct.eax,
		struct user_regs_struct.ebx,
		struct user_regs_struct.ecx,
		struct user_regs_struct.edx,
		struct user_regs_struct.orig_rax,
		struct user_regs_struct.rax,
		struct user_regs_struct.rdi,
		struct user_regs_struct.rsi,
		struct user_regs_struct.rdx], [],[],
    [#include <bits/types.h>
     #include <sys/user.h>])
AC_CHECK_MEMBERS([struct pt_regs.orig_gpr3,
		struct pt_regs.gpr], [],[], [#include <linux/ptrace.h>])
AC_CHECK_MEMBERS([struct pt_regs.uregs],[],[], [#include <asm/ptrace.h>])
AC_CHECK_MEMBERS([struct user_pt_regs.regs],[],[], [#include <asm/ptrace.h>])
AC_CHECK_MEMBERS([struct pt_regs.regs,
                  struct pt_regs.cp0_status],[],[], [#include <asm/ptrace.h>])
AC_CHECK_MEMBERS([struct pt_regs.orig_d0,
                  struct pt_regs.d0],[],[], [#include <asm/ptrace.h>])

case ${host_os} in
  gnu*)
    want_fuser=no
    ;;
  *)
    want_fuser=yes
	;;
esac
AM_CONDITIONAL(WANT_FUSER, test $want_fuser = yes)

AM_CONDITIONAL(WANT_PEEKFD_I386,
 test $ac_cv_member_struct_user_regs_struct_orig_eax = yes &&
   test $ac_cv_member_struct_user_regs_struct_eax = yes &&
   test $ac_cv_member_struct_user_regs_struct_ebx = yes &&
   test $ac_cv_member_struct_user_regs_struct_ecx = yes &&
   test $ac_cv_member_struct_user_regs_struct_edx = yes )
AM_CONDITIONAL(WANT_PEEKFD_X86_64,
 test $ac_cv_member_struct_user_regs_struct_orig_rax = yes &&
   test $ac_cv_member_struct_user_regs_struct_rax = yes &&
   test $ac_cv_member_struct_user_regs_struct_rdi = yes &&
   test $ac_cv_member_struct_user_regs_struct_rsi = yes &&
   test $ac_cv_member_struct_user_regs_struct_rdx = yes )
AM_CONDITIONAL(WANT_PEEKFD_PPC,
   test $ac_cv_member_struct_pt_regs_orig_gpr3 = yes &&
   test $ac_cv_member_struct_pt_regs_gpr = yes )
AM_CONDITIONAL(WANT_PEEKFD_ARM,
   test $ac_cv_member_struct_pt_regs_uregs = yes)
AM_CONDITIONAL(WANT_PEEKFD_ARM64,
   test $ac_cv_member_struct_user_pt_regs_regs = yes)
AM_CONDITIONAL(WANT_PEEKFD_MIPS,
   test $build_cpu = mipsel &&
   test $ac_cv_member_struct_pt_regs_regs = yes &&
   test $ac_cv_member_struct_pt_regs_cp0_status = yes)
AM_CONDITIONAL(WANT_PEEKFD_M68K,
   test $ac_cv_member_struct_pt_regs_orig_d0 = yes &&
   test $ac_cv_member_struct_pt_regs_d0 = yes)

dnl Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_GETMNTENT
AC_PROG_GCC_TRADITIONAL
AC_FUNC_LSTAT
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_CHECK_FUNCS([atexit getmntent memset nl_langinfo regcomp rpmatch setlocale socket strcasecmp strchr strdup strerror strpbrk strrchr strtoul])

dnl Checks for Large File System
AC_SYS_LARGEFILE

if test x"$DEJAGNU" = x
then
  DEJAGNU="/dev/null"
fi
AC_SUBST(DEJAGNU)
	
AC_CONFIG_FILES([Makefile
		 doc/Makefile
		 icons/Makefile
		 po/Makefile.in
		 man-po/Makefile
		 testsuite/Makefile])
AC_OUTPUT
