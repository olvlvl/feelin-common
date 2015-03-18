#ifndef FEELIN_CONF_H
#define FEELIN_CONF_H

#define F_NEW_WIDGET_MODE

/* 2007/07/10

New values for the Widget class attribute Mode :

	* Button (release)

		mouse button down or key pressed : Selected = TRUE, Pressed = TRUE
		mouse button up or key up (after mbd or key down) :
			* ouside : Selected = FALSE
			* inside : Selected = FALSE; Pressed = FALSE

	* Toggle (toggle)

		mouse button down or key pressed : Selected = (Selected == TRUE) ? FALSE : TRUE

	* Touch (immediate)

		mouse button down or key pressed : Selected = TRUE

	* Inert (none)

************************************************************************************************/


#define F_NEW_GETELEMENTBYID

/* 2007/07/22

The Object class now defines the GetObjectById virtual method, which can  be
implemented by subclasses.

The method is used to retreive an object from an application,  a  family,  a
group ... by its Id.

************************************************************************************************/

#define F_NEW_LISTENERS

/* 2007/07/20

Listeners are a new way  of  managing  attributes  modifications.  They  are
managed  by  the  Object class and are triggered each time an attribute gets
modified.

To add a listener on an attribute we  use  the  AddListener  method  on  the
object which attribute should be listened to :

	IFEELIN F_Do(Obj, FM_AddListener, STRPTR attribute_name, struct Hook *call_back, bits32 flags);

The  call_back  function  will   be   called   each   time   the   attribute
'attribute_name' is modified :

struct FS_Object_Listener
{
	uint32								Id;
	uint32								Value;
	FClassAttribute						*Attribute;
};

F_HOOKM(uint32, call_back, FS_Object_Listener)
{
	return 0;
}

Because the attribute's id is available in the listerner  message,  one  can
add multiple listener using a single call_back.

***

The purpose of the listerner is to get rid of the CallHook and CallHookEntry
methods used in former notifications created by the Notify method.

************************************************************************************************/

#define F_NEW_WIDGET_PUBLIC

/* 2007/07/10

Add pointers to FElementPublic and FAreaPublic to FWidgetPublic. Thus,  each
subclass  won't  have  to  waste  space  for them it their local object data
space.

New macros are now available to use them e.g. _widget_render, _widget_x1, or
_widget_app...

************************************************************************************************/

//#define F_NEW_APPLICATION_PUBLICIZE

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
servent  � d�finir l'aspet des objets. Dans ce but, la classe FC_Element
va �tre cr�e. Cette sous classe  de  FC_Object  servira  �  traiter  les
donn�es  de l'attribut FA_Style et impl�mentera les m�thodes FM_SetStyle
et FM_GetStyle. L'attribut FA_Class de FC_Object  va  �tre  supprim�  et
r�impl�ment� par FC_Element � la mani�re de ce que fait CSS.

Par exemple, on ne fournira plus tous les attributs de style pour  cr�er
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


Dans le fichier de pr�f�rence, on trouve:

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

Si on utilise des classes plut�t que des attributs, on ne  devrait  plus
avoir  �  surcharger  des FA_Back... de toute fa�on, comme sous HTML, on
peut d�finir FA_Style de nouveau pour surcharger classe et styles.

Adieu donc les attributs de style individuels, TOUT passe par l'attribut
FA_Style.




Dans  un  soucis  de  granularit�,  et  aussi  pour  all�ger  FC_Object,
FC_Element  g�rera  s�rement les attributs FA_ID et FA_Persist ainsi que
les m�thodes FM_LoadPersistentAttributes et  FM_SavePersistentAttributes
� sa place.


*************
MISE EN PLACE
*************

	INITIALISATION
	**************

		* Chaque objet d�finie sa classe e.g. "button" ou "slider".

		:??: Est-ce que les classes se surchargent ou non ? La classe  n'est
		elle   pas  celle  de  l'objet  final,  auquel  cas  son  style  est
		enti�rement d�fini dans une d�claration ?

		*  Le  contenu  des  styles  d�finis  par  l'attribut  FA_Style   se
		surchargent.

		Au final on devrait se retrouver avec le nom d'une classe  �  partir
		de  laquelle,  plus  tard,  on  pourra charger des styles qui seront
		surcharg�s par ceux d�finis par l'attribut FA_Style.

	SETUP
	*****

		La m�thode  FM_Setup  actuellement  impl�ment�e  par  FC_Frame  sera
		d�plac�e   dans   FC_Element.   Elle   sera   s�rement   renom�e  en
		FM_Element_Setup.. parce qu'il est temps de ranger tout �a :-)

		Lors du setup, FC_Element r�soud les  propi�t�s  cr�es  lors  de  la
		phase  d'initialisation,  en  n'oubliant  pas  de charger ceux de la
		classe. D'ailleurs, il faudrait un format de  structure  commun  aux
		pr�f�rences et � FC_Element pour que FC_Element n'est pas � re-cr�er
		des resources d�j� existantes.


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

Ce serait peut-�tre mieux que FPreferenceStyle soit fait un objet. Comme �a,
on aurait pas � farfouiller dedans.

[????]

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

		* que toutes les items soient d�finies dans une piscine
		* que les items soient utilisable en tant que r�f�rences
		* que les items des menus et des barres d'outils soient les m�mes


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
