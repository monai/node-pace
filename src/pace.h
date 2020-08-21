#ifndef pace_h
#define pace_h

#define PACE_CALL_BASE(call_ret_val, exp_ret_val, the_call, ret_val) \
  call_ret_val = (the_call);                                         \
  if (call_ret_val == exp_ret_val) {                                 \
    return ret_val;                                                  \
  }

#define PACE_CALL_WITH_STATUS(the_call)    \
  do {                                     \
    int status;                            \
    PACE_CALL_BASE(status, 0, the_call, 0) \
  } while (0)
#define PACE_CALL_RET_ZERO(call_ret_val, the_call) \
  PACE_CALL_BASE(call_ret_val, 0, the_call, 0)

#endif
