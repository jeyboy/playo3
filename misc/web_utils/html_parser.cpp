#include "html_parser.h"

HtmlSet & HtmlSet::find(HtmlSelector * selector, HtmlSet & set) {
    for(HtmlSet::Iterator tag = begin(); tag != end(); tag++) {
        if ((*tag) -> validTo(selector)) {
            if (selector -> next && !(*tag) -> children().isEmpty())
                (*tag)-> children().find(selector -> next, set);
            else
                set.append((*tag));
        }
        else if (!selector -> _direct && !(*tag) -> children().isEmpty())
            (*tag)-> children().find(selector, set);
    }

    return set;
}
