/**
 * @file state_machine.c
 * @author ZC (387646983@qq.com)
 * @brief 
 * @version 0.1
 * @date 2025-06-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "state_machine.h"
#include "../../3rd/elab/common/elab_assert.h"


ELAB_TAG("StateMachine");
// 预留事件数组，用于定义状态机的基本事件类型。
// 每个事件由信号类型和附加数据组成，初始值为0。
sm_event_t sm_reserved_event[] =
{
	{ SM_EMPTY_SIG, 0 },// 空事件信号
	{ SM_ENTRY_SIG, 0 },// 状态进入信号
	{ SM_EXIT_SIG,  0 },// 状态退出信号
	{ SM_INIT_SIG,  0 },// 状态初始化信号
	{ SM_USER_SIG,  0 },// 用户自定义信号
};


/** 
 * @brief 状态机构造函数，用于初始化状态机对象。
 * 
 * @param me 指向状态机对象的指针。
 * @param init 初始状态处理函数。
 */
void fsm_ctor(sm_t *me, sm_state_handler_t init)
{
	elab_assert(0 != me);
	elab_assert(0 != init);

	me->state = 0;    // 初始化当前状态为0（未定义状态）。
	me->temp  = init; // 设置初始状态处理函数。
}

/**
 * @brief 状态机初始化函数，用于执行状态机的初始状态
 * 
 * @param me 指向状态机对象的指针。
 * @param e 事件对象指针
 * @return sm_ret_t 
 */
sm_ret_t  fsm_init(sm_t *me, sm_event_t *e)
{
	sm_ret_t ret;

	elab_assert(0 != me); 
	elab_assert(0 != me->temp);

	ret = (me->temp)(me, e);

	if(ret != SM_RET_TRAN)
	{
	return ret;
		
	}
	// 如果初始状态处理函数触发状态转换，则将当前状态设置为初始状态。
	SM_ENTRY(me, me->temp); 

	me->state = me->temp;
	return ret;
}

/**
 * @brief 状态机事件分发函数，用于处理事件并执行状态转换。
 * 
 * @param me 
 * @param e 
 */
void fsm_dispatch(sm_t *me, sm_event_t *e)
{
	sm_ret_t ret;

	elab_assert(me->state == me->temp); //验证状态是否稳定

	ret = (me->temp)(me, e);// 调用当前状态处理函数处理事件。


	if(SM_RET_TRAN == ret)// 如果状态处理函数返回状态转换信号
	{
		SM_EXIT(me, me->state);// 创建状态退出事件 执行退出事件
		SM_ENTRY(me, me->temp);// 创建状态进入事件 执行进入事件
		me->state = me->temp;// 更新状态机的当前状态为目标状态
	}
}

/**
 * @brief HSM根状态
 */
sm_ret_t hsm_top(sm_t *me, const sm_event_t *e)
{
	(void)me;
	(void)e;

	return SM_IGNORE();
}

static uint8_t hsm_find_path(sm_t *me,
						sm_state_handler_t t,
						sm_state_handler_t s,
						sm_state_handler_t path[SM_MAX_NEST_DEPTH])
{
	signed char ip = -1;
	signed char iq;
	sm_ret_t ret;

	/* (a) check source==target (transition to self) */
	if( s == t)
	{
		SM_EXIT(me, s);
		ip = 0;

		goto hsm_find_path_end;
	}

	SM_TRIG(me, t, SM_EMPTY_SIG);
	t = me->temp;

	/* (b) check source==target->super */
	if( s == t )
	{
		ip = 0;
		goto hsm_find_path_end;
	}

	SM_TRIG(me, s, SM_EMPTY_SIG);

	/* (c) check source->super==target->super */
	if(me->temp == t)
	{
		SM_EXIT(me, s);
		ip = 0;
		goto hsm_find_path_end;
	}

	/* (d) check source->super==target */
	if( me->temp == path[0] )
	{
		SM_EXIT(me, s);
		goto hsm_find_path_end;
	}

	/* (e) check rest of source==target->super->super..
	 * and store the entry path along the way
	 */
	ip = 1;
	iq = 0;
	path[1] = t;
	t = me->temp;

	/* find target->super->super... */
	ret = SM_TRIG(me, path[1], SM_EMPTY_SIG);
	while(SM_RET_SUPER == ret)
	{
		path[++ip] = me->temp;
		if(s == me->temp)
		{
			iq = 1;
			elab_assert(ip < SM_MAX_NEST_DEPTH);
			ip--;

			ret = SM_RET_HANDLED;
		}
		else
		{
			ret = SM_TRIG(me, me->temp, SM_EMPTY_SIG);
		}
	}

	/* the LCA not found yet? */
	if(0 == iq)
	{
		elab_assert(ip < SM_MAX_NEST_DEPTH);

		SM_EXIT(me, s);

		/* (f) check the rest of source->super
		 *                  == target->super->super...
		 */
		iq = ip;
		ret = SM_RET_IGNORE; /* LCA NOT found */
		do
		{
			s = path[iq];
			/* is this the LCA? */
			if(t == s)
			{
				ret = SM_RET_HANDLED;

				ip = iq - 1;
				iq = -1;
			}
			else
			{
				iq--; /* try lower superstate of target */
			}
		}while(iq >= 0);

		 /* LCA not found? */
		if( SM_RET_HANDLED != ret )
		{
			/* (g) check each source->super->...
			 * for each target->super...
			 */
			ret = SM_RET_IGNORE;
			do
			{
				if(SM_RET_HANDLED  == SM_EXIT(me, t))
				{
					SM_TRIG(me, t, SM_EMPTY_SIG);
				}
				t = me->temp;
				iq = ip;
				do
				{
					s = path[iq];
					if( t == s)
					{
						ip = iq -1;
						iq = -1;

						ret = SM_RET_HANDLED; /* break */
					}
					else
					{
						iq--;
					}
				}while(iq >= 0);
			}while(SM_RET_HANDLED != ret);
		}
	}

hsm_find_path_end:
	return ip;
}

void hsm_ctor(sm_t *me, sm_state_handler_t init)
{
    elab_assert(0 != me);
    elab_assert(0 != init);

    me->state = hsm_top;  // 初始化状态为顶层状态
    me->temp  = init;     // 初始化临时状态为传入的初始状态
}
void hsm_init(sm_t *me, sm_event_t *e)
{
	sm_ret_t ret;
	int8_t ip;

	sm_state_handler_t path[SM_MAX_NEST_DEPTH];// 用于存储状态路径

	sm_state_handler_t t = me->state;// 当前状态

	elab_assert(0 != me);
	elab_assert(0 != me->temp);
	elab_assert(hsm_top == t);// 确保当前状态为顶层状态
	ret = (me->temp)(me, e);// 调用初始状态处理函数
	elab_assert(SM_RET_TRAN == ret); // 确保初始状态处理函数返回状态切换

	do
	{
		ip = 0;// 初始化路径索引
		// 将临时状态存储到路径数组的第一个位置
		path[0] = me->temp;
		 // 触发临时状态的处理函数，发送空信号 SM_EMPTY_SIG
		SM_TRIG(me, me->temp,SM_EMPTY_SIG);
		while( t != me->temp )
		{// 将当前临时状态存储到路径数组的下一个位置
			path[++ip] = me->temp;
			// 触发当前临时状态的处理函数，发送空信号 SM_EMPTY_SIG
			SM_TRIG(me, me->temp,SM_EMPTY_SIG);
			t = me->temp;  // 更新 t 的值为当前临时状态
			elog_debug("t = %p, me->temp = %p", t, me->temp);  // 打印 t 和 me->temp 的值
		}
		 // 将路径数组的第一个状态赋值给临时状态
		me->temp = path[0];
		// 确保路径索引未超过最大深度限制
		elab_assert(ip < SM_MAX_NEST_DEPTH);

		do
		{
			SM_ENTRY(me, path[ip--]);

		}while(ip >= 0);

		t = path[0];
		elog_debug("hsm_5");
	}while(SM_RET_TRAN == SM_TRIG(me, t, SM_INIT_SIG));
elog_debug("hsm_6");
	me->temp = t;
	me->state = me->temp;
}


void hsm_dispatch(sm_t *me, sm_event_t *e)
{
	sm_state_handler_t t = me->state;
	sm_state_handler_t s;

	sm_ret_t ret;

	// 状态必须稳定
	elab_assert(me->state == me->temp);

	/* process the event hierarchically... */
	// 事件递归触发, 直到某个状态处理该事件
	do
	{
		s = me->temp;
		ret = s(me, e); 	// 调用状态处理函数
		if(SM_RET_UNHANDLED == ret)
		{
			ret = SM_TRIG(me, s, SM_EMPTY_SIG);
		}
	}while(SM_RET_SUPER == ret);

	// 如果发生状态转换
	if(SM_RET_TRAN == ret)
	{
		sm_state_handler_t path[SM_MAX_NEST_DEPTH];
		signed char ip = -1;

		path[0] = me->temp; 	// 状态转换的目的状态
		path[1] = t; 			// 状态转换的源状态

		/* exit current state to transition source s... */
		for( ; s != t; t = me->temp)
		{
			ret = SM_EXIT(me, t);
			if(SM_RET_HANDLED == ret)
			{
				SM_TRIG(me, t, SM_EMPTY_SIG);
			}
		}

		ip = hsm_find_path(me, path[0], s, path);

		for(; ip>=0; ip--)
		{
			SM_ENTRY(me, path[ip]);
		}

		t = path[0];
		me->temp = t;

		/* drill into the target hierarchy... */
		while( SM_RET_TRAN == SM_TRIG(me, t, SM_INIT_SIG) )
		{
			ip = 0;
			path[0] = me->temp;

			SM_TRIG(me, me->temp, SM_EMPTY_SIG);
			while(t != me->temp)
			{
				path[++ip] = me->temp;
				SM_TRIG(me, me->temp, SM_EMPTY_SIG);
			}
			me->temp = path[0];

			elab_assert(ip < SM_MAX_NEST_DEPTH);

			do
			{
				SM_ENTRY(me, path[ip--]);
			}while(ip >= 0);

			t = path[0];
		}// end: while( SM_RET_TRAN == SM_TRIG(me, t, SM_INIT_SIG) )
	} // end: if(SM_RET_TRAN == ret)

	me->temp = t;
	me->state = t;
}

