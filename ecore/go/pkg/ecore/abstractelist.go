package ecore

type abstractEList interface {
	doAdd(elem interface{})

	doAddAll(list EList)

	doInsert(index int, elem interface{})

	doInsertAll(index int, list EList)

	doSet(index int, elem interface{})
}
