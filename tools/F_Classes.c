;/*
   F_Create.rexx EXE F_Classes
   QUIT
   _________________________________________________________________________

   F_Classes 2.00 (2005/05/30)

   Written by Olivier LAVIALE (gofromiel@gofromiel.com)

   This tool is for debuggin purpose only  !!  It  will  print  all  classes
   currently available in memory.
*/

#include <exec/execbase.h>
#include "Feelin:Sources/Feelin/Private.h"

#undef SysBase
#undef DOSBase

#include <proto/exec.h>
#include <proto/dos.h>

extern struct ExecBase             *SysBase;
struct in_FeelinBase               *FeelinBase;

struct MyArgs
{
   LONG                             ShowMethods;
   LONG                             ShowAttributes;
   LONG                             ShowValues;
   LONG                             dummy;
};

void main(void)
{
   if (!FindName(&SysBase->LibList,"feelin.library"))
   {
      Printf("Feelin is not running.\n");

      return;
   }

   if (FeelinBase = (struct in_FeelinBase *) OpenLibrary("feelin.library",FV_FEELIN_VERSION))
   {
      struct MyArgs args[] = { FALSE, FALSE, FALSE, 0 };
      APTR rdargs;

      if (rdargs = ReadArgs("M=METHODS/S,A=ATTRIBUTES/S,V=VALUES/S",(LONG *)(&args),NULL))
      {
         struct in_FeelinClass *Class;
         ULONG nc=0,ns=0,nl=0;

         F_CLASSES_LOCK;

         for (Class = (struct in_FeelinClass *) F_CLASSES_HEAD ; Class ; Class = (struct in_FeelinClass *)(Class->public.Next))
         {
            nc++;

            Printf("(0x%08lx) %-18.s ",Class,Class->public.Name);
            if (Class->id)          {    Printf("(%06lx)",Class->id); }
            else                    {    Printf("(static)"); ns++; };
            Printf(" [1mUsed[0m %4ld - [1mSize[0m %4ld",Class->public.UserCount,Class->public.LODSize);
            if (Class->module)      {    Printf(" - [1mBase[0m 0x%08lx",Class->module); nl++; }
            else                         Printf("                  ");
            if (Class->public.Super)     Printf(" - [1mSuper[0m %s",Class->public.Super->Name);
            Printf("\n");

            if (SetSignal(0L,SIGBREAKF_CTRL_C) & SIGBREAKF_CTRL_C)
            {
               Printf("*** Break\n"); break;
            }

            if (args->ShowMethods)
            {
               FClassMethod *entry = Class->public.Methods;

               if (entry)
               {
                  for ( ; entry->Function ; entry++)
                  {
                     Printf("   >> Method 0x%08lx (0x%08lx)",entry->ID,entry->Function);
                     
                     if (_method_name(entry))
                     {
                        Printf(" - %s",_method_name(entry));
                     }
                     Printf("\n");
                  }
               }
               else
               {
                  Printf("   >> No Methods\n");
               }
            }
         
            if (args->ShowAttributes)
            {
               if (Class->public.Attributes)
               {
                    FClassAttribute *node;

                    for (F_CLASS_EACH_ATTRIBUTE(Class->public.Attributes, node))
                    {
                        Printf("   >> Attribute 0x%08lx (%s)\n", node->ID, _attribute_name(node));
                     
                        if (args->ShowValues && node->Values)
                        {
                            FClassAttributeValue *val;

                            for (F_CLASS_EACH_VALUE(node->Values, val))
                            {
                                Printf("      >> Value (%s)(%ld)\n", _value_name(val), val->Value);
                            }
                        }
                    }
               }
               else
               {
                  Printf("   >> No Attributes\n");
               }
            }
         }

         Printf("\nTOTAL: %ld [1mClasses[0m (%ld static, %ld externals)\n",nc,ns,nl);

         F_CLASSES_UNLOCK;

         FreeArgs(rdargs);
      }
      else
      {
         PrintFault(IoErr(),NULL);
      }
      F_FEELIN_CLOSE;
   }
}
