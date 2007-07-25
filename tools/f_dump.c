/*

   F_Dump 2.00 (2005/09/09)

   Written by Olivier LAVIALE (gofromiel@gofromiel.com)

   This tool is for debuggin purpose only !! It will travel trough pools and
   puddles to dump memory allocations in several files.

*/

#include <exec/execbase.h>
#include "Feelin:Sources/Feelin/Private.h"

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>

#undef SysBase
#undef DOSBase
#undef UtilityBase

extern struct ExecBase             *SysBase;
struct in_FeelinBase               *FeelinBase;
#define UtilityBase                 FeelinBase -> Public.Utility

enum    {
	
		ARG_NAME,
		ARG_MODE,
		
		ARG_COUNT

		};
		
#define TEMPLATE                                "NAME,MODE/K"

#define FF_DUMP_CHUNKS                          (1 << 0)
#define FF_DUMP_PUDDLES                         (1 << 1)
						
#define TARGET                                  "swap:"

int main(void)
{
	struct FeelinPool *pool;
	struct FeelinPuddle *puddle;
	struct FeelinMemChunk *chunk;
	BPTR file;
	int32 args[ARG_COUNT];
	APTR rdargs;
	STRPTR str;

	bits32 flags = FF_DUMP_CHUNKS;
	
	args[ARG_NAME] = (int32) NULL;
	args[ARG_MODE] = (int32) "chunk";

	if ((FeelinBase = (APTR) FindName(&SysBase -> LibList,"feelin.library")) != NULL)
	{
		if ((str = AllocMem(1024,0)) == NULL)
		{
			return 0;
		}
 
		Execute("Delete >NIL: " TARGET "F_DUMP_#?",NULL,NULL);

		rdargs = ReadArgs(TEMPLATE,args,NULL);

		Printf("[2m[41m                                              [0m\n");
		Printf("[2m[41m !! This tool is for debuggin purpose only !! [0m\n");
		Printf("[2m[41m                                              [0m\n\n");

		if (!args[ARG_NAME])
		{
			Printf("(Use 'N' or 'NAME' to save specific pools)\n\n");
		}

		switch (*((STRPTR)(args[ARG_MODE])))
		{
			case 'a':
			{
				flags = FF_DUMP_CHUNKS | FF_DUMP_PUDDLES;
			}
			break;

			case 'c':
			{
				flags = FF_DUMP_CHUNKS;
			}
			break;
			 
			case 'p':
			{
				flags = FF_DUMP_PUDDLES;
			}
			break;
		}
 
		for (pool = FeelinBase -> pools ; pool ; pool = pool -> next)
		{
			STRPTR pool_name = (pool -> Semaphore.ss_Link.ln_Name) ? pool -> Semaphore.ss_Link.ln_Name : "private";

			if (args[ARG_NAME])
			{
				if (Stricmp((STRPTR)(args[ARG_NAME]),pool_name) != 0)
				{
					continue;
				}
			}
				 
			for (puddle = pool -> puddles ; puddle ; puddle = puddle -> next)
			{
				if (FF_DUMP_PUDDLES & flags)
				{
					//F_StrFmt(str,TARGET "F_DUMP_%s_puddle_%08lx",pool_name,puddle);
					F_StrFmt(str,TARGET "F_DUMP_puddle_%08lx",puddle);
				
					Printf("F_Dump: Pool (%lx)(%s) >> %s\n", (int32) pool, (int32) pool_name, (int32) FilePart(str));
 
					if ((file = Open((STRPTR)(str),MODE_NEWFILE)) != NULL)
					{
						Write(file,puddle->lower,(uint32)(puddle->upper) - (uint32)(puddle->lower));
						Close(file);
						SetComment(str,pool_name);
					}
				}

				if (FF_DUMP_CHUNKS & flags)
				{
					for (chunk = puddle -> chunks ; chunk ; chunk = chunk -> next)
					{
						F_StrFmt(str,TARGET "F_DUMP_chunk_%08.lx",chunk);
					
						Printf("F_Dump: Pool (%lx)(%s) >> %s\n", (int32) pool, (int32) pool_name, (int32) FilePart(str));

						if ((file = Open((STRPTR)(str),MODE_NEWFILE)) != NULL)
						{
							Write(file,(APTR)((ULONG)(chunk) + sizeof (struct FeelinMemChunk)),chunk -> size - sizeof (struct FeelinMemChunk));
							Close(file);
							
							SetComment(str,pool_name);
						}
						else
						{
							Printf("Unable to open file '%s'\n", (int32) str);
						}

						if (SetSignal(0L,SIGBREAKF_CTRL_C) & SIGBREAKF_CTRL_C)
						{
							Printf("*** Break\n");
							
							break;
						}
					}
				}
			
				if (SetSignal(0L,SIGBREAKF_CTRL_C) & SIGBREAKF_CTRL_C)
				{
					Printf("*** Break\n");
					
					break;
				}
			}
		}

		if (rdargs)
		{
			FreeArgs(rdargs);
		}
			
		FreeMem(str,1024);
	}
	else
	{
		Printf("Feelin is not running.\n");
	}

	return 0;
}
