#ifndef _NFNETLINK_H
#define _NFNETLINK_H


#include <linux/netlink.h>
#include <linux/capability.h>
#include <net/netlink.h>
#include <linux/types.h>
#include <linux/netfilter/nfnetlink_compat.h>

// User space contents start
enum nfnetlink_groups {
        NFNLGRP_NONE,
#define NFNLGRP_NONE                    NFNLGRP_NONE
        NFNLGRP_CONNTRACK_NEW,
#define NFNLGRP_CONNTRACK_NEW           NFNLGRP_CONNTRACK_NEW
        NFNLGRP_CONNTRACK_UPDATE,
#define NFNLGRP_CONNTRACK_UPDATE        NFNLGRP_CONNTRACK_UPDATE
        NFNLGRP_CONNTRACK_DESTROY,
#define NFNLGRP_CONNTRACK_DESTROY       NFNLGRP_CONNTRACK_DESTROY
        NFNLGRP_CONNTRACK_EXP_NEW,
#define NFNLGRP_CONNTRACK_EXP_NEW       NFNLGRP_CONNTRACK_EXP_NEW
        NFNLGRP_CONNTRACK_EXP_UPDATE,
#define NFNLGRP_CONNTRACK_EXP_UPDATE    NFNLGRP_CONNTRACK_EXP_UPDATE
        NFNLGRP_CONNTRACK_EXP_DESTROY,
#define NFNLGRP_CONNTRACK_EXP_DESTROY   NFNLGRP_CONNTRACK_EXP_DESTROY
        NFNLGRP_NFTABLES,
#define NFNLGRP_NFTABLES                NFNLGRP_NFTABLES
        __NFNLGRP_MAX,
};
#define NFNLGRP_MAX     (__NFNLGRP_MAX - 1)

/* General form of address family dependent message.
 */
struct nfgenmsg {
        __u8  nfgen_family;             /* AF_xxx */
        __u8  version;          /* nfnetlink version */
        __be16    res_id;               /* resource id */
};

#define NFNETLINK_V0    0

/* netfilter netlink message types are split in two pieces:
 * 8 bit subsystem, 8bit operation.
 */

#define NFNL_SUBSYS_ID(x)       ((x & 0xff00) >> 8)
#define NFNL_MSG_TYPE(x)        (x & 0x00ff)

/* No enum here, otherwise __stringify() trick of MODULE_ALIAS_NFNL_SUBSYS()
 * won't work anymore */
#define NFNL_SUBSYS_NONE                0
#define NFNL_SUBSYS_CTNETLINK           1
#define NFNL_SUBSYS_CTNETLINK_EXP       2
#define NFNL_SUBSYS_QUEUE               3
#define NFNL_SUBSYS_ULOG                4
#define NFNL_SUBSYS_OSF                 5
#define NFNL_SUBSYS_IPSET               6
#define NFNL_SUBSYS_ACCT                7
#define NFNL_SUBSYS_CTNETLINK_TIMEOUT   8
#define NFNL_SUBSYS_CTHELPER            9
#define NFNL_SUBSYS_NFTABLES            10
#define NFNL_SUBSYS_NFT_COMPAT          11
#define NFNL_SUBSYS_COUNT               12

/* Reserved control nfnetlink messages */
#define NFNL_MSG_BATCH_BEGIN            NLMSG_MIN_TYPE
#define NFNL_MSG_BATCH_END              NLMSG_MIN_TYPE+1

// User space contents end


struct nfnl_callback {
	int (*call)(struct sock *nl, struct sk_buff *skb, 
		    const struct nlmsghdr *nlh,
		    const struct nlattr * const cda[]);
	int (*call_rcu)(struct sock *nl, struct sk_buff *skb, 
		    const struct nlmsghdr *nlh,
		    const struct nlattr * const cda[]);
	int (*call_batch)(struct sock *nl, struct sk_buff *skb,
			  const struct nlmsghdr *nlh,
			  const struct nlattr * const cda[]);
	const struct nla_policy *policy;	/* netlink attribute policy */
	const u_int16_t attr_count;		/* number of nlattr's */
};

struct nfnetlink_subsystem {
	const char *name;
	__u8 subsys_id;			/* nfnetlink subsystem ID */
	__u8 cb_count;			/* number of callbacks */
	const struct nfnl_callback *cb;	/* callback for individual types */
	int (*commit)(struct sk_buff *skb);
	int (*abort)(struct sk_buff *skb);
};

int nfnetlink_subsys_register(const struct nfnetlink_subsystem *n);
int nfnetlink_subsys_unregister(const struct nfnetlink_subsystem *n);

int nfnetlink_has_listeners(struct net *net, unsigned int group);
struct sk_buff *nfnetlink_alloc_skb(struct net *net, unsigned int size,
				    u32 dst_portid, gfp_t gfp_mask);
int nfnetlink_send(struct sk_buff *skb, struct net *net, u32 portid,
		   unsigned int group, int echo, gfp_t flags);
int nfnetlink_set_err(struct net *net, u32 portid, u32 group, int error);
int nfnetlink_unicast(struct sk_buff *skb, struct net *net, u32 portid,
		      int flags);

void nfnl_lock(void);
void nfnl_unlock(void);

#define MODULE_ALIAS_NFNL_SUBSYS(subsys) \
	MODULE_ALIAS("nfnetlink-subsys-" __stringify(subsys))

#endif	/* _NFNETLINK_H */
