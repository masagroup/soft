// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2019 MASA Group
//
// *****************************************************************************

package ecore

type treeIterator struct {
	object      interface{}
	data        []EIterator
	root        bool
	getChildren func(interface{}) EIterator
}

func newTreeIterator(object interface{}, root bool, getChildren func(interface{}) EIterator) *treeIterator {
	return &treeIterator{object: object, root: root, getChildren: getChildren}
}

func newEAllContentsIterator(object EObject) *treeIterator {
	return &treeIterator{object: object, root: false, getChildren: func(i interface{}) EIterator {
		o, _ := i.(EObject)
		if o != nil {
			return o.EContents().Iterator()
		}
		return nil
	}}
}

func (it *treeIterator) HasNext() bool {
	if it.data == nil && !it.root {
		return it.hasAnyChildren()
	} else {
		return it.hasMoreChildren()
	}
}

func (it *treeIterator) hasAnyChildren() bool {
	current := it.getChildren(it.object)
	it.data = append(it.data, current)
	return current.HasNext()
}

func (it *treeIterator) hasMoreChildren() bool {
	return it.data == nil || len(it.data) != 0 && it.data[len(it.data)-1].HasNext()
}

func (it *treeIterator) Next() interface{} {
	if it.data == nil {
		// Yield that mapping, create a stack, and add it to the stack.
		current := it.getChildren(it.object)
		it.data = append(it.data, current)
		if it.root {
			return it.object
		}
	}

	// Get the top iterator, retrieve it's result, and record it as the one to which
	// remove will be delegated.
	current := it.data[len(it.data)-1]
	result := current.Next()

	// If the result about to be returned has children...
	iterator := it.getChildren(result)
	if iterator.HasNext() {
		// Add iterator to the stack.
		it.data = append(it.data, iterator)
	} else {
		// While the current iterator has no next...
		for !current.HasNext() {
			// Pop it from the stack.
			it.data = it.data[:len(it.data)-1]

			// If the stack is empty, we're done.
			if len(it.data) == 0 {
				break
			}

			// Get the next one down and then test it for has next.
			current = it.data[len(it.data)-1]
		}
	}
	return result
}
