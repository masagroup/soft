package ecore

// EArrayList is an array of a dynamic size
type EArrayList struct {
	data []interface{}
}

// Add a new elemement to the ArrayList
func (arr *EArrayList) Add(elem interface{}) {
	arr.data = append(arr.data, elem)
}

func (arr *EArrayList) Get(index int) interface{} {
	return arr.data[index]
}

func (arr *EArrayList) Count() int {
	return len(arr.data)
}

func (arr *EArrayList) Iterate() chan interface{} {
	ch := make(chan interface{})
	go func() {
		defer close(ch)
		for i := 0; i < arr.Count(); i++ {
			ch <- arr.Get(i)
		}
	}()
	return ch
}
