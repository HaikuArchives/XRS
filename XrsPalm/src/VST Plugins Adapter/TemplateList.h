/******************************************************************************
/
/	File:			TemplateList.h
/
/	Description:	TList class provides storage for template as List.
/					Not thread safe.
/
/	Copyright 1993-98, Be Incorporated
/   Changed into a template by Georges-Edouard Berenger 2000
/	Note the major difference between the orginal class and this template:
/	Here, the destructor *will* delete the given objects!!!
/
******************************************************************************/

#ifndef	_TEMPLATE_LIST_H

#include <List.h>

template <class T> class TList {

public:
				TList (int32 itemsPerBlock = 20) : fList (itemsPerBlock) {}
				TList (const TList& init) : fList (init.fList) {}
virtual			~TList ();

		TList	&operator=(const TList &from)
			{ fList = from; return this; }

/* Adding and removing items. */
		bool	AddItem (T *item)
			{ return fList.AddItem (item); }

		bool	AddItem (T *item, int32 atIndex)
			{ return fList.AddItem (item, atIndex); }

		bool	AddList (TList *newItems)
			{ return fList.AddList (newItems); }

		bool	AddList (TList *newItems, int32 atIndex)
			{ return fList.AddList (newItems, atIndex); }

		bool	RemoveItem (T *item)
			{ return fList.RemoveItem (item); }

		T		*RemoveItem (int32 index)
			{ return (T*) fList.RemoveItem (item); }

		bool	RemoveItems (int32 index, int32 count)
			{ return fList.RemoveItems (index, count); }

		bool	ReplaceItem (int32 index, T *newItem)
			{ return fList.RemoveItems (index, newItem); }

		void	MakeEmpty ()
			{ fList.MakeEmpty (); }
			
/* Reordering items. */
		void	SortItems (int (*cmp)(const T *, const T *))
			{ fList.SortItems (cmp); }

		bool	SwapItems (int32 indexA, int32 indexB)
			{ return fList.SwapItems (indexA, indexB); }

		bool	MoveItem(int32 fromIndex, int32 toIndex)
			{ return fList.MoveItem (fromIndex, toIndex); }


/* Retrieving items. */
		T		*ItemAt (int32 n) const
			{ return (T*) fList.ItemAt (n); }

		T		*ItemAtFast (int32 n) const
			{ return (T*) fList.ItemAtFast (n); }

		T		*FirstItem () const
			{ return (T*) fList.FirstItem (); }

		T		*LastItem () const
			{ return (T*) fList.LastItem (); }

		T		*Items () const
			{ return (T*) fList.Items (); }


/* Querying the list. */
		bool	HasItem (T *item) const
			{ return fList.HasItem (item); }

		int32	IndexOf (T *item) const
			{ return fList.IndexOf (item); }

		int32	CountItems () const
			{ return fList.CountItems (); }

		bool	IsEmpty () const
			{ return fList.IsEmpty (); }


/* Iterating over the list. */
		void	DoForEach (bool (*func)(T *))
			{ return fList.DoForEach (func); }

		void	DoForEach (bool (*func)(T *, T *), T * item)
			{ return fList.DoForEach (func, item); }

		BList & GetBList ()
			{ return fList; }
private:
	BList		fList;
};

template <class T>
TList<T>::~TList ()
{
	for (int i = fList.CountItems () - 1; i >= 0; i--)
		delete ItemAtFast (i);
	MakeEmpty ();
}

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

#endif /* _TEMPLATE_LIST_H */
