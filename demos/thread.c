#include <dos/dostags.h>
#include <dos/dosextens.h>
#include <dos/notify.h>
#include <libraries/feelin.h>

#include <proto/feelin.h>
#include <proto/exec.h>
#include <proto/dos.h>

struct FeelinBase *FeelinBase;

#ifdef __amigaos4__
struct FeelinIFace *IFeelin;
#endif

struct FS_THREAD_INIT                           { struct Task *Owner; };

///Thread_Main
F_THREAD_ENTRY(Thread_Main)
{
	IFEELIN F_Log(0,"Thread_Main() Thread (0x%08lx) Public (0x%08lx) - Owner (0x%08lx)",Thread,Public,((struct FS_THREAD_INIT *)UserData)->Owner);
 
	for (;;)
	{
		FThreadMsg *msg = (FThreadMsg *) IFEELIN F_Do(Thread, Public->id_Pop);
	  
		if (msg)
		{
			IFEELIN F_Log(0,"Msg 0x%08lx - Action 0x%08lx",msg,msg -> Action);

			switch (msg -> Action)
			{
				case FV_Thread_Hello:
				{
					msg -> Return = TRUE;
				}
				break;

				case FV_Thread_Bye:
				{
					IFEELIN F_Log(0,"bye");

					msg -> Return = TRUE;

					return msg;
				}
				break;
			}

			IEXEC ReplyMsg(msg);
		}
		else
		{
			IFEELIN F_Log(0,"waiting...");

			IFEELIN F_Do(Thread, Public->id_Wait, 0);
		}
	}
}
//+

int32 main(void)
{
	struct FS_THREAD_INIT msg;

	msg.Owner = IEXEC FindTask(NULL);

	if (F_FEELIN_OPEN)
	{
		FObject thread = ThreadObject,
			
			"FA_Thread_Entry",     Thread_Main,
			"FA_Thread_Name",      "test.thread",
			"FA_Thread_Priority",  "Low",
			"FA_Thread_UserData",   &msg,

			End;
		   
		if (thread)
		{
			IDOS_ Printf("Thread 0x%08lx - Priority %ld.\nUse Ctrl-C to terminate the demo.\n",thread,IFEELIN F_Get(thread, (uint32) "FA_Thread_Priority"));

			IEXEC Wait(SIGBREAKF_CTRL_C);

			IFEELIN F_DisposeObj(thread);
		}
		
		F_FEELIN_CLOSE;
	}
	return 0;
}

