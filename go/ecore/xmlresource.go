package ecore

import (
	"encoding/xml"
	"fmt"
	"io"

	"golang.org/x/net/html/charset"
)

type XMLResource struct {
	*EResourceImpl
}

func NewXMLResource() *XMLResource {
	r := new(XMLResource)
	r.EResourceImpl = NewEResourceImpl()
	r.SetInterfaces(r)
	return r
}

func (r *XMLResource) DoLoad(rd io.Reader) {
	d := xml.NewDecoder(rd)
	d.CharsetReader = charset.NewReaderLabel
	for {
		t, tokenErr := d.Token()
		if tokenErr != nil {
			if tokenErr == io.EOF {
				break
			}
			// handle error
		}
		switch t := t.(type) {
		case xml.StartElement:
			fmt.Println("StartElement:" + t.Name.Local)
		case xml.EndElement:
			fmt.Println("EndElement:" + t.Name.Local)
		case xml.CharData:
			c := string([]byte(t))
			fmt.Println("CharData:" + c)
		case xml.Comment:
			c := string([]byte(t))
			fmt.Println("Comment:" + c)
		case xml.ProcInst:
			fmt.Println("ProcInst")
		case xml.Directive:
			c := string([]byte(t))
			fmt.Println("Directive:" + c)
		}
	}
}
