/*
 * See Copyright Notice in qnode.h
 */

#ifndef __QMSG_H__
#define __QMSG_H__

#include "qactor.h"
#include "qlist.h"
#include "qtype.h"

struct qmailbox_t;

/* 
 * s_* means server thread -> worker thread
 * w_* means worker thread -> server thread
 * t_* means worker thread -> worker thread
 * other msg can use between both side
 * NOTE: actor MUST be create in server thread and send to worker thread
 * */
enum {
  s_start = 1,
  spawn   = 2,
  QMAX_MSG_TYPE
};

enum {
  SMSG_FLAG = 1,        /* server-worker message flag */
  WMSG_FLAG = 2,        /* worker-server message flag */
  TMSG_FLAG = 3,        /* worker-worker message flag */
  MSG_FLAG  = 4,        /* both side message flag */
};

/* define messages between worker-thread and main-thread */
typedef struct qmsg_t {
  qlist_t entry;

  unsigned short flag;
  unsigned int type;

  qtid_t sender_id;
  qtid_t receiver_id;

  union {
    struct {
      struct qactor_t *actor;
    } s_start;

    struct {
      qid_t aid;
      qid_t parent;
      lua_State *state;
      qactor_t *actor;
    } spawn;
  } args;
} qmsg_t;

/* handler for server thread msg */
typedef int (*smsg_handler)(struct qthread_t *thread, struct qmsg_t *msg);

/* handler for worker thread msg */
typedef int (*wmsg_handler)(struct qserver_t *server, struct qmsg_t *msg);

qmsg_t* qmsg_new(qtid_t sender_id, qtid_t receiver_id);
qmsg_t* qmsg_clone(qmsg_t *msg);

#define qmsg_is_smsg(msg)         ((msg)->flag == SMSG_FLAG || (msg)->flag == MSG_FLAG)
#define qmsg_is_wmsg(msg)         ((msg)->flag == WMSG_FLAG || (msg)->flag == MSG_FLAG)

#define qmsg_invalid_type(type)   ((type) <= 0 || (type) >= QMAX_MSG_TYPE)

#define qmsg_init_sstart(msg, actor)              \
do {                                              \
  qlist_entry_init(&(msg->entry));                \
  msg->type = s_start;                            \
  msg->flag = SMSG_FLAG;                          \
  msg->args.s_start.actor = (actor);              \
} while (0)

#define qmsg_init_spawn(msg, aid, parent, state)  \
do {                                              \
  qlist_entry_init(&(msg->entry));                \
  msg->type = spawn;                              \
  msg->flag = MSG_FLAG;                           \
  msg->args.spawn.aid = (aid);                    \
  msg->args.spawn.parent = (parent);              \
  msg->args.spawn.state = (state);                \
  msg->args.spawn.actor = NULL;                   \
} while (0)

#endif  /* __QMSG_H__ */
