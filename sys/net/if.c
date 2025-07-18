
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * Copyright (c) 1980 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 * SCCSID: @(#)if.c	3.0	4/21/86
 *	Based on: @(#)if.c	6.7 (Berkeley) 6/8/85
 */

/**
 * @file if.c
 * @brief Network interface support routines.
 *
 * This file provides initialization, lookup, watchdog management
 * and ioctl handling for network interfaces.
 */

#include <errno.h>
#include <sys/dir.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/protosw.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/systm.h>
#include <sys/user.h>

#include <net/af.h>
#include <net/if.h>
#include <netinet/in_systm.h>
#ifdef PUP
#include <netinet/ether.h>
#endif PUP

int ifqmaxlen = IFQ_MAXLEN;

/*
 * Network interface utility routines.
 *
 * Routines with ifa_ifwith* names take sockaddr *'s as
 * parameters.
 */

/**
 * @brief Initialize all configured network interfaces.
 *
 * Iterates over the global interface list and calls each driver's
 * initialization routine. The routine also ensures that the send
 * queue length is properly set and starts the interface timer.
 */
ifinit() {
  register struct ifnet *ifp;

  for (ifp = ifnet; ifp; ifp = ifp->if_next)
    if (ifp->if_init) {
      (*ifp->if_init)(ifp->if_unit);
      if (ifp->if_snd.ifq_maxlen == 0)
        ifp->if_snd.ifq_maxlen = ifqmaxlen;
    }
  if_slowtimo();
}

#ifdef vax
/*
 * Call each interface on a Unibus reset.
 */

/**
 * @brief Reset Unibus devices.
 *
 * Invoked during a Unibus reset to allow each interface driver
 * to reinitialize its hardware state.
 *
 * @param uban Unibus adapter number being reset.
 */
ifubareset(uban) int uban;
{
  register struct ifnet *ifp;

  for (ifp = ifnet; ifp; ifp = ifp->if_next)
    if (ifp->if_reset)
      (*ifp->if_reset)(ifp->if_unit, uban);
}
#endif

/*
 * Attach an interface to the
 * list of "active" interfaces.
 */

/**
 * @brief Add an interface to the system list.
 *
 * The interface is linked onto the global @c ifnet list making it
 * available for packet routing and ioctl operations.
 *
 * @param ifp Pointer to the interface structure to attach.
 */
if_attach(ifp) struct ifnet *ifp;
{
  register struct ifnet **p = &ifnet;

  while (*p)
    p = &((*p)->if_next);
  *p = ifp;
}

/*
 * Locate an interface based on a complete address.
 */
/*ARGSUSED*/

/**
 * @brief Find an interface matching an exact address.
 *
 * Searches the interface list for an address that matches @p addr or,
 * for broadcast capable interfaces, the broadcast address.
 *
 * @param addr Socket address to look up.
 * @return Pointer to the matching interface address or @c NULL.
 */
struct ifaddr *ifa_ifwithaddr(addr)
struct sockaddr *addr;
{
  register struct ifnet *ifp;
  register struct ifaddr *ifa;

#define equal(a1, a2)                                                          \
  (bcmp((caddr_t)((a1)->sa_data), (caddr_t)((a2)->sa_data), 14) == 0)
  for (ifp = ifnet; ifp; ifp = ifp->if_next)
    for (ifa = ifp->if_addrlist; ifa; ifa = ifa->ifa_next) {
      if (ifa->ifa_addr.sa_family != addr->sa_family)
        continue;
      if (equal(&ifa->ifa_addr, addr))
        return (ifa);
      if ((ifp->if_flags & IFF_BROADCAST) && equal(&ifa->ifa_broadaddr, addr))
        return (ifa);
    }
  return ((struct ifaddr *)0);
}

/*
 * Find an interface on a specific network.  If many, choice
 * is first found.
 */

/**
 * @brief Locate an interface on the same network as @p addr.
 *
 * @param addr Address whose network portion should match.
 * @return Pointer to the first matching interface address or @c NULL.
 */
struct ifaddr *ifa_ifwithnet(addr)
register struct sockaddr *addr;
{
  register struct ifnet *ifp;
  register struct ifaddr *ifa;
  register u_int af = addr->sa_family;
  register int (*netmatch)();

  if (af >= AF_MAX)
    return (0);
  netmatch = afswitch[af].af_netmatch;
  for (ifp = ifnet; ifp; ifp = ifp->if_next)
    for (ifa = ifp->if_addrlist; ifa; ifa = ifa->ifa_next) {
      if (ifa->ifa_addr.sa_family != addr->sa_family)
        continue;
      if ((*netmatch)(&ifa->ifa_addr, addr))
        return (ifa);
    }
  return ((struct ifaddr *)0);
}

/*
 * Find an interface using a specific address family
 */

/**
 * @brief Find an interface for the given address family.
 *
 * @param af Address family identifier.
 * @return Pointer to the first interface with that family or @c NULL.
 */
struct ifaddr *ifa_ifwithaf(af)
register int af;
{
  register struct ifnet *ifp;
  register struct ifaddr *ifa;

  for (ifp = ifnet; ifp; ifp = ifp->if_next)
    for (ifa = ifp->if_addrlist; ifa; ifa = ifa->ifa_next)
      if (ifa->ifa_addr.sa_family == af)
        return (ifa);
  return ((struct ifaddr *)0);
}

/*
 * Mark an interface down and notify protocols of
 * the transition.
 * NOTE: must be called at splnet or eqivalent.
 */
static
    /**
     * @brief Bring an interface down.
     *
     * Clears the @c IFF_UP flag and notifies protocols that the
     * interface has transitioned to the down state.
     *
     * @param ifp Interface to mark as down.
     */
    if_down(ifp) register struct ifnet *ifp;
{
  register struct ifaddr *ifa;

  ifp->if_flags &= ~IFF_UP;
  for (ifa = ifp->if_addrlist; ifa; ifa = ifa->ifa_next)
    pfctlinput(PRC_IFDOWN, (caddr_t)&ifa->ifa_addr);
}

/*
 * Handle interface watchdog timer routines.  Called
 * from softclock, we decrement timers (if set) and
 * call the appropriate interface routine on expiration.
 */

/**
 * @brief Periodic interface timer maintenance.
 *
 * Decrements each interface's watchdog timer and invokes the
 * driver-specific watchdog routine when a timer expires.
 */
if_slowtimo() {
  register struct ifnet *ifp;
#ifdef pdp11
  extern int hz;
#endif pdp11

  for (ifp = ifnet; ifp; ifp = ifp->if_next) {
    if (ifp->if_timer == 0 || --ifp->if_timer)
      continue;
    if (ifp->if_watchdog)
      (*ifp->if_watchdog)(ifp->if_unit);
  }
  timeout(if_slowtimo, (caddr_t)0, hz / IFNET_SLOWHZ);
}

/*
 * Map interface name to
 * interface structure pointer.
 */

/**
 * @brief Look up an interface by name.
 *
 * @param name Null terminated name such as "qe0".
 * @return Pointer to the matching interface or @c NULL.
 */
struct ifnet *ifunit(name)
register char *name;
{
  register char *cp;
  register struct ifnet *ifp;
  int unit;

  for (cp = name; cp < name + IFNAMSIZ && *cp; cp++)
    if (*cp >= '0' && *cp <= '9')
      break;
  if (*cp == '\0' || cp == name + IFNAMSIZ || cp == name)
    return ((struct ifnet *)0);
  unit = *cp - '0';
  for (ifp = ifnet; ifp; ifp = ifp->if_next) {
    if (bcmp(ifp->if_name, name, (unsigned)(cp - name)))
      continue;
    if (unit == ifp->if_unit)
      break;
  }
  return (ifp);
}

/*
 * Interface ioctls.
 */

/**
 * @brief Handle network interface ioctl requests.
 *
 * Dispatches control commands for interface configuration and
 * passes unrecognized operations to the protocol layer.
 *
 * @param so   Socket associated with the request.
 * @param cmd  Ioctl command code.
 * @param data Pointer to command specific data.
 * @return 0 on success or an errno on failure.
 */
ifioctl(so, cmd, data) struct socket *so;
int cmd;
caddr_t data;
{
  register struct ifnet *ifp;
  register struct ifreq *ifr;
#ifdef pdp11
  struct ifreq uifr; /* user's ifreq structure */
  int err;

  if (copyin(data, (caddr_t)&uifr, sizeof(uifr))) {
    return (EFAULT);
  }
#endif
  switch (cmd) {

  case SIOCGIFCONF:
#ifdef vax
    return (ifconf(cmd, data));
#else pdp11
  {
    register int error;
    if (error = ifconf(cmd, (caddr_t)&uifr))
      return (error);
    if (copyout((caddr_t)&uifr, data, sizeof(struct ifconf)))
      return (EFAULT);
    return (0);
  }
#endif

  case SIOCSARP:
  case SIOCDARP:
    if (!suser())
      return (u.u_error);
    /* FALL THROUGH */
  case SIOCGARP:
#ifdef vax
    return (arpioctl(cmd, data));
#else pdp11
    return (arpioctl(cmd, (caddr_t)&uifr));
#endif
  }
#ifdef vax
  ifr = (struct ifreq *)data;
#else pdp11
  ifr = (struct ifreq *)&uifr;
#endif
  ifp = ifunit(ifr->ifr_name);
  if (ifp == 0)
    return (ENXIO);
  switch (cmd) {

  case SIOCGIFFLAGS:
    ifr->ifr_flags = ifp->if_flags;
    break;

  case SIOCSIFFLAGS:
    if (ifp->if_flags & IFF_UP && (ifr->ifr_flags & IFF_UP) == 0) {
      int s = splimp();
      if_down(ifp);
      splx(s);
    }
    ifp->if_flags =
        (ifp->if_flags & IFF_CANTCHANGE) | (ifr->ifr_flags & ~IFF_CANTCHANGE);
    break;

  default:
    if (so->so_proto == 0)
      return (EOPNOTSUPP);
#ifdef vax
    return ((*so->so_proto->pr_usrreq)(so, PRU_CONTROL, cmd, data, ifp));
#else pdp11
    err = (*so->so_proto->pr_usrreq)(so, PRU_CONTROL, cmd, (caddr_t)&uifr, ifp);
    if (err)
      return (err);
    break;
#endif
  }
#ifdef pdp11
  if (copyout((caddr_t)&uifr, data, sizeof(uifr))) {
    return (EFAULT);
  }
#endif
  return (0);
}

/*
 * Return interface configuration
 * of system.  List may be used
 * in later ioctl's (above) to get
 * other information.
 */
/*ARGSUSED*/

/**
 * @brief Copy interface configuration to user space.
 *
 * @param cmd  Ioctl command (unused).
 * @param data Buffer describing the destination and available length.
 * @return 0 on success or an errno on failure.
 */
ifconf(cmd, data) int cmd;
caddr_t data;
{
  register struct ifconf *ifc = (struct ifconf *)data;
  register struct ifnet *ifp = ifnet;
  register struct ifaddr *ifa;
  register char *cp, *ep;
  struct ifreq ifr, *ifrp;
  int space = ifc->ifc_len, error = 0;

  ifrp = ifc->ifc_req;
  ep = ifr.ifr_name + sizeof(ifr.ifr_name) - 2;
  for (; space > sizeof(ifr) && ifp; ifp = ifp->if_next) {
    bcopy(ifp->if_name, ifr.ifr_name, sizeof(ifr.ifr_name) - 2);
    for (cp = ifr.ifr_name; cp < ep && *cp; cp++)
      ;
    *cp++ = '0' + ifp->if_unit;
    *cp = '\0';
    if ((ifa = ifp->if_addrlist) == 0) {
      bzero((caddr_t)&ifr.ifr_addr, sizeof(ifr.ifr_addr));
      error = copyout((caddr_t)&ifr, (caddr_t)ifrp, sizeof(ifr));
      if (error)
        break;
      space -= sizeof(ifr), ifrp++;
    } else
      for (; space > sizeof(ifr) && ifa; ifa = ifa->ifa_next) {
        ifr.ifr_addr = ifa->ifa_addr;
        error = copyout((caddr_t)&ifr, (caddr_t)ifrp, sizeof(ifr));
        if (error)
          break;
        space -= sizeof(ifr), ifrp++;
      }
  }
  ifc->ifc_len -= space;
  return (error);
}
