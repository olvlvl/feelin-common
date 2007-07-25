/*

$VER: F_Alloc 2.00 (2005/08/05)

   Written by Olivier LAVIALE (gofromiel@gofromiel.com)

   This tool is for debuggin purpose only !! It travels  trough  pools  and
   puddles to print every memory allocated by F_New() or F_NewP().

*/

#include <exec/execbase.h>

#include "Feelin:Sources/Feelin/Private.h"

#undef SysBase
#undef DOSBase
				
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>

struct in_FeelinBase *FeelinBase;

#ifdef __amigaos4__
struct FeelinIFace                 *IFeelin;
#endif

enum    {
	
		ARG_NAME,
		ARG_TREE,
		
		ARG_COUNT

		};
	
#define TEMPLATE                                "NAME=N,TREE=T/S"

int main(void)
{
	if (!FindName(&SysBase->LibList,"feelin.library"))
	{
		Printf("Feelin is not running.\n");
	}
	else if ((FeelinBase = (struct in_FeelinBase *) OpenLibrary("feelin.library",FV_FEELIN_VERSION)) != NULL)
	{
		FPool *pool;
		FPuddle *puddle;
		FMemChunk *chunk;
		uint32 s1,s2,s3,n1=0,n2=0,n3=0,f=0;
		uint32 args[ARG_COUNT];
		APTR  rdargs;
		
		uint32 spreading=0;
		uint32 i;
		
		args[ARG_NAME] = NULL;
		args[ARG_TREE] = FALSE;
	  
/*** read arguments ********************************************************/

		if ((rdargs = ReadArgs(TEMPLATE,(int32 *)(&args),NULL)) != NULL)
		{
			FreeArgs(rdargs);
		}

		Printf("[2m[41m                                              [0m\n");
		Printf("[2m[41m !! This tool is for debuggin purpose only !! [0m\n");
		Printf("[2m[41m                                              [0m\n\n");

		if (args[ARG_TREE] == FALSE)
		{
			Printf("Use 'TREE' or 'T' switch for more informations\n");
				   
		}
		if (args[ARG_NAME] == NULL)
		{
			Printf("Use 'NAME' or 'N' to select a pool\n");
		}
	
		Printf("\n");

		s1 = 0;

		for (pool = FeelinBase->pools ; pool ; pool = pool->next)
		{
			if (args[ARG_NAME])
			{
				if (Stricmp(pool->Semaphore.ss_Link.ln_Name, (STRPTR) args[ARG_NAME]) != 0) continue;
			}
			
			if (args[ARG_TREE]) Printf("[33mPool[0m 0x%08lx %s (%s) ", (int32) pool, (int32) ((pool->Semaphore.ss_Link.ln_Name) ? pool->Semaphore.ss_Link.ln_Name : "Private"), (int32) ((pool->Semaphore.ss_Owner) ? "locked" : "unlocked"));

			s2 = 0;

			if ((puddle = pool->puddles) != NULL)
			{
				ULONG pf = 0;
	 
				if (args[ARG_TREE]) Printf("\n");

				for ( ; puddle ; puddle = puddle->next)
				{
					if (args[ARG_TREE])
					{
						Printf("   [33mPuddle[0m 0x%08.lx - [1mSize[0m %ld\n", (int32) puddle, puddle->size - sizeof (struct FeelinPuddle));
					}

					s3 = 0; pf += puddle->free;

					for (chunk = puddle->chunks ; chunk ; chunk = chunk->next)
					{
						//if (args[ARG_TREE]) Printf("      [33mChunk[0m 0x%08.lx (0x%08.lx) - Next 0x%08.lx - Size %ld%s\n",chunk,(ULONG)(chunk) + sizeof (struct FeelinMemChunk),chunk->next,chunk->size,(chunk == puddle->middle) ? " (middle)" : "");

						n3++; s3 += chunk->size;

						if (SetSignal(0L,SIGBREAKF_CTRL_C) & SIGBREAKF_CTRL_C)
						{
						   IDOS_ Printf("*** Break\n");
						   
						   return 0;
						}
					}

					if (args[ARG_TREE]) Printf("   [1mUsed[0m %ld - [1mFree[0m %ld\n",s3,puddle->free);

					n2++; s2 += s3; f += puddle->free;
				}
				if (args[ARG_TREE]) Printf("[1mUsed[0m %ld - [1mFree[0m %ld\n\n",s2,pf);
			}
			else
			{
			   if (args[ARG_TREE]) Printf(" is empty\n");
			}

			n1++; s1 += s2;
		}

		if (args[ARG_TREE]) Printf("\n");

		if ((f + s1) != 0)
		{
			Printf("[1mUsed[0m %ld bytes (%ld%%), [1mFree[0m %ld bytes (%ld%%), [1mTotal[0m %ld bytes.\n",s1,s1 * 100 / (s1+f),f,f * 100 / (s1+f),s1+f);
			Printf("in %ld [1mPools[0m, %ld [1mPuddles[0m and %ld [1mChunks[0m.",n1,n2,n3);
		}
		
		if (args[ARG_NAME] != NULL && n2 != 0)
		{
			for (i = 0 ; i < FV_MEMORY_HASH_SIZE + 1; i ++)
			{
				if (FeelinBase->hash_puddles[i])
				{
					spreading++;
				}
			}
		
			Printf(" Puddles spreading %ld%%.",spreading * 100 / n2);
		}
	
		Printf("\n");
 

		F_FEELIN_CLOSE;
	}
	return 0;
}
