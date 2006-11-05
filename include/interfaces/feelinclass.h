#ifndef FEELINCLASS_INTERFACE_DEF_H
#define FEELINCLASS_INTERFACE_DEF_H

/*
** This file was machine generated by idltool 51.6.
** Do not edit
*/ 

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef EXEC_EXEC_H
#include <exec/exec.h>
#endif
#ifndef EXEC_INTERFACES_H
#include <exec/interfaces.h>
#endif

#ifndef FEELIN_TYPES_H
#include <feelin/types.h>
#endif

struct FeelinClassIFace
{
	struct InterfaceData Data;

	ULONG APICALL (*Obtain)(struct FeelinClassIFace *Self);
	ULONG APICALL (*Release)(struct FeelinClassIFace *Self);
	void APICALL (*Expunge)(struct FeelinClassIFace *Self);
	struct Interface * APICALL (*Clone)(struct FeelinClassIFace *Self);
	APTR reserved1;

	struct TagItem * APICALL (*F_Query)(struct FeelinClassIFace *Self, uint32 Which, struct FeelinBase * Feelin);
};

#endif /* FEELINCLASS_INTERFACE_DEF_H */