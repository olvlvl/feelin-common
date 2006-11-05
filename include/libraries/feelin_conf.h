#ifndef FEELIN_CONF_H
#define FEELIN_CONF_H

#define F_NEW_HANDLERS

/*

	Death to embeded structures !!

	Using embended structure was easy and quick but TOO dangerous because if
	an handler (usualy embended in some LOD) was trashed, the whole list was
	corrupted. For now on, these kind of  handlers  are  private.  They  are
	created with a taglist, providing flexibility and expandibility.

	FSignalHandler and FEventHandler are now  dead  and  gone.  The  methods
	formaly used to "add" the handlers also create it now with the help of a
	taglist.

	Only an "handle" is returned, which cannot be disposed with  F_Dispose()
	to limit risks.

*/

#define F_NEW_TYPES

/*

	New types and organisation.

*/

/************************************************************************************************
*** ALIGN ***************************************************************************************
************************************************************************************************/

#define F_NEW_AREA_ALIGN

/*

	Usualy, if an object in a group was smaller then the available  size  it
	was  centered.  The new FA_Area_Align attribute can now be used to align
	objects to the 'start', the 'center'  (default)  or  the  'end'  of  the
	group:

	|[]    |
	|  []  |
	|    []|

************************************************************************************************/

/************************************************************************************************
*** ATOMS ***************************************************************************************
************************************************************************************************/

#define F_NEW_ATOMS

/*

	An  *atom  table*  is  a  system  wide  table  that  store  strings.  An
	application  places  a  string  in an *atom table* and receives a unique
	structure called an atom, that can be used to access the  string.  Thus,
	instead  of  comparing  strings,  one  can simply compare the pointer of
	their atoms.

*/

//#define F_NEW_ATOMS_AMV

/*

	Add atoms to attributes, methods and values.

	>> should be tested more because it  doesn't  seams  to  improve  things
	much, the dynamic system is already very fast.

************************************************************************************************/

/************************************************************************************************
*** ELEMENT *************************************************************************************
************************************************************************************************/

#define F_NEW_ELEMENT

/*

	Add the Element class.

 * SETUP & CLEANUP

	The FM_Setup and FM_Cleanup methods,  renamed  as  FM_Element_Setup  and
	FM_Element_Cleanup,  are  now implemented by the class, and no longer by
	the Frame class.

	POUR CSS


		deux listes  publiques  :  celle  obtenue  depuis  FC_Preference  en
		fonction  de  la  classe,  pas  de  copie, on utilise directement le
		pointeur.

		seconde liste constituée avec les overrides de  FA_Style.  Tous  les
		FA_Style définis dans la taglist initiale sont pris en compte, et se
		surchargent.

		>> associer les chaines avec un simple "%s%s" ou la décomposer  pour
		quelle soit prête.



************************************************************************************************/

#ifdef F_NEW_ELEMENT
#define F_NEW_ELEMENT_ID
#endif

/*

	Move the 'ID' attribute from the Object class to the Element class.  The
	'ID' was kept in the Object class to reference the application object in
	XML  application,  which  is   useless   since   the   global   variable
	'application' can be used instead.

************************************************************************************************/

#ifdef F_NEW_ELEMENT_ID
//#define F_NEW_APPLICATION_PUBLICIZE
#endif

/*

	Add the PublicizeElement, PrivatizeElement and GetElementById methods to
	the Application class.

	The PublicizeElement method is used by  the  Element  class  during  the
	GlobalConnect  method  to publicize elements with Id. On the other hand,
	the PrivatizeElement method is used during the  GlobalDisconnect  method
	to privatize formaly publicized elements.

	Finaly,  the  GetElementById  method  is  used  to  retreive  publicized
	elements.

	NOTE: Publicized elements are invoked with the  LoadPersistentAttributes
	and  SavePersistentAttributes  methods  if  their  Persist  attribute is
	defined.

************************************************************************************************/

/************************************************************************************************
*** PERSISTENT **********************************************************************************
************************************************************************************************/

#define F_NEW_PERSISTENT

/*



************************************************************************************************/

/************************************************************************************************
*** STYLES **************************************************************************************
************************************************************************************************/

#define F_NEW_STYLES

/*

Il faut vraiment que  je  distingue,  parmis  les  attributs,  ceux  qui
servent  à définir l'aspet des objets. Dans ce but, la classe FC_Element
va être crée. Cette sous classe  de  FC_Object  servira  à  traiter  les
données  de l'attribut FA_Style et implémentera les méthodes FM_SetStyle
et FM_GetStyle. L'attribut FA_Class de FC_Object  va  être  supprimé  et
réimplémenté par FC_Element à la manière de ce que fait CSS.

Par exemple, on ne fournira plus tous les attributs de style pour  créer
un boutton comme on le fait aujourd'hui :

		TextObject,

			FA_Back,                "$button-back",
			FA_Font,                "$button-font",
			FA_Frame,               "$button-frame",
			FA_ColorScheme,         "$button-scheme",
			FA_SetMax,              FV_SetHeight,
			FA_InputMode,           FV_InputMode_Release,
			FA_Text,                {TEXTE},
			FA_Text_PreParse,       "$button-preparse",

		End;

On utilisera l'attribut FA_Class (ou "class" en XML)

		TextObject,

			FA_Element_Class,               "button",

			FA_SetMax,              FV_SetHeight,
			FA_InputMode,           FV_InputMode_Release,
			FA_Text,                {TEXTE},

		End;


Dans le fichier de préférence, on trouve:

	button
	{
		back:               fill;
		font:               inherit;
		frame:              <frame id="13" padding="1px 6px" />;
		scheme:             inherit;
		preparse:           <align=center>;
	}

Il faut vraiment que je me tourne vers le CSS et que je cesse de  penser
en terme d'attributs (e.g. "$button-back").

Si on utilise des classes plutôt que des attributs, on ne  devrait  plus
avoir  à  surcharger  des FA_Back... de toute façon, comme sous HTML, on
peut définir FA_Style de nouveau pour surcharger classe et styles.

Adieu donc les attributs de style individuels, TOUT passe par l'attribut
FA_Style.




Dans  un  soucis  de  granularité,  et  aussi  pour  alléger  FC_Object,
FC_Element  gérera  sûrement les attributs FA_ID et FA_Persist ainsi que
les méthodes FM_LoadPersistentAttributes et  FM_SavePersistentAttributes
à sa place.


*************
MISE EN PLACE
*************

	INITIALISATION
	**************

		* Chaque objet définie sa classe e.g. "button" ou "slider".

		:??: Est-ce que les classes se surchargent ou non ? La classe  n'est
		elle   pas  celle  de  l'objet  final,  auquel  cas  son  style  est
		entièrement défini dans une déclaration ?

		*  Le  contenu  des  styles  définis  par  l'attribut  FA_Style   se
		surchargent.

		Au final on devrait se retrouver avec le nom d'une classe  à  partir
		de  laquelle,  plus  tard,  on  pourra charger des styles qui seront
		surchargés par ceux définis par l'attribut FA_Style.

	SETUP
	*****

		La méthode  FM_Setup  actuellement  implémentée  par  FC_Frame  sera
		déplacée   dans   FC_Element.   Elle   sera   sûrement   renomée  en
		FM_Element_Setup.. parce qu'il est temps de ranger tout ça :-)

		Lors du setup, FC_Element résoud les  propiétés  crées  lors  de  la
		phase  d'initialisation,  en  n'oubliant  pas  de charger ceux de la
		classe. D'ailleurs, il faudrait un format de  structure  commun  aux
		préférences et à FC_Element pour que FC_Element n'est pas à re-créer
		des resources déjà existantes.


************************************************************************************************/

#ifdef F_NEW_STYLES
#define F_NEW_STYLES_PREFERENCE
#endif

/*

Implements the FM_Preference_GetStyle method in FC_Preference.

	FPreferenceStyle * F_Do(Obj, FM_Preference_GetStyle, FObject Object);

Find all matching CSS declarations  for  a  given  object,  composites  them
together  (using class hierarchy to override properties), and then creates a
FPreferenceStyle representing the composite appearance. ( A cache is used to
avoid creating a new style for same objects).


typedef FeelinPreferenceProperty
{
	struct FeelinPreferenceProperty *Next;
	FAtom                          *Atom;
	STRPTR                          Value;
	uint32                          NumericValue;
	bits32                          NumericType;
}
FPreferenceProperty;

typedef FeelinPreferenceStyle
{
	FPreferenceProperty            *Properties;
}
FPreferenceStyle;

[????]

Ce serait peut-être mieux que FPreferenceStyle soit fait un objet. Comme ça,
on aurait pas à farfouiller dedans.

[????]

************************************************************************************************/

#define F_NEW_STYLES_EXTENDED

/*

	* FA_Element_Class and FA_Element_Style are used to create the preference style.

	* FClass is extended for 'local style data' support.

	* the FM_Element_CreateStyle (name under construction) method  is  added
	to create shared decoded structures.

************************************************************************************************/



#define F_NEW_ADJUST








//#define F_USE_PICASSO96

/* If F_USE_PICASSO96 is not set, Feelin will use CyberGraphics ;  for  OS4,
this  define  has  been  included  directly  in the makefile, so it may stay
slashed */

#define F_NEW_GLOBALCONNECT

/* Add the 'GlobalConnect' and 'GlobalDisconnect'  methods  to  the  Element
class. These methods are invoked after objects are added to the application.
The purpose of these new methods is to have a pointer to the application and
the window the objects belongs to available before the 'Setup' and 'Cleanup'
methods, because an object not setuped still belongs to an application. */

//#define F_NEW_CONNECT

/* Connection was not really usefull because objects were connected  by  the
family  class  *before*  adding  them  to the family, doing thus it from the
deepest child to the root. Resulting in objects adding children without even
knowing who their parent was.

The new connection system works differently:

	- children are added to the family without connecting.
	- children are connected *only* if their parent is already connected.
	- children are connected when their parent is connected.

Methods are modified:

	Element.TreeConnect(Application, Window, Parent)

		Now when an object is connected is knows about the whole application
		tree.  Formaly, objects were aware of the application and the window
		they belong to only during the Element.Setup() method !

	Element.TreeDisconnect(void)

Other modifications:

	Application, Window and Parent  pointers  are  handled  by  the  Element
	class. The Parent is no longer handled by the Area class !

	The TreeConnect() and  TreeDisconnect()  method  of  the  Element  class
	replace the Connect() and Disconnect() method of the Object class. Thus,
	only Elements may have a parent. This is because the Application connect
	itself to the AppServer object.

*/

/* MENUS

	ce que je veux:

		* que toutes les items soient définies dans une piscine
		* que les items soient utilisable en tant que références
		* que les items des menus et des barres d'outils soient les mêmes


	<ItemsPool id="project">

		<Item id="item.project.open"    Label="Open"    Accel="O" />
		<Item id="item.project.save"    Label="Save"    Accel="S" />
		<Item id="item.project.saveas"  Label="Save As" Accel="A" />

	</ItemsPool>

	<MenuPopup Pool="project">
		<Item Reference="item.project.open" />
		<Item Reference="item.project.save" />
		<Item Reference="item.project.saveas" />
	</MenuPopup>


*/

#endif
